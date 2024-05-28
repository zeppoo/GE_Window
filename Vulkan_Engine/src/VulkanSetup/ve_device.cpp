#include "ve_device.hpp"
#include "ve_swapChain.hpp"
#include <set>


namespace ve
{
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicDevice, VkSurfaceKHR surface) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicDevice, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicDevice, i, surface, &presentSupport);
            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }

            i++;
        }

        return indices;
    }

    ve_device::ve_device(ve::ve_configuration &config) : config{config} {}

    void ve_device::pickPhysicalDevice() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(config.vkInstance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(config.vkInstance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            if (isDeviceSuitable(device)) {
                config.physicDevice = device;
                break;
            }
        }

        if (config.physicDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }

    void ve_device::createLogicalDevice()
    {
        // Find queue families
        QueueFamilyIndices indices = findQueueFamilies(config.physicDevice, config.surface);

        // Ensure queue families are complete before proceeding
        if (!indices.isComplete()) {
            throw std::runtime_error("failed to find all necessary queue families!");
        }

        // Unique queue families
        std::set<uint32_t> uniqueQueueFamilies = {
                indices.graphicsFamily.value(),
                indices.presentFamily.value()
        };

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        float queuePriority = 1.0f;

        // Create queue create info structures
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        // Device features
        VkPhysicalDeviceFeatures deviceFeatures{};

        // Device create info
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>((config.deviceExtensions).size());
        createInfo.ppEnabledExtensionNames = (config.deviceExtensions).data();

        // Validation layers
        if (config.enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(config.validationLayers.size());
            createInfo.ppEnabledLayerNames = config.validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        // Create the logical device
        if (vkCreateDevice(config.physicDevice, &createInfo, nullptr, &config.logicDevice) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        // Retrieve the queue handles
        vkGetDeviceQueue(config.logicDevice, indices.graphicsFamily.value(), 0, &config.graphicsQueue);
        vkGetDeviceQueue(config.logicDevice, indices.presentFamily.value(), 0, &config.presentQueue);
    }


    void ve_device::createCommandPool() {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(config.physicDevice, config.surface);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(config.logicDevice, &poolInfo, nullptr, &config.commandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }
    }


    void ve_device::createCommandBuffer() {
        config.commandBuffers.resize(config.MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = config.commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t) config.commandBuffers.size();

        if (vkAllocateCommandBuffers(config.logicDevice, &allocInfo, config.commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void ve_device::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = config.renderPass;

        // Ensure imageIndex is within bounds
        if (imageIndex >= config.swapChainFramebuffers.size()) {
            throw std::runtime_error("imageIndex out of bounds!");
        }

        renderPassInfo.framebuffer = config.swapChainFramebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = config.swapChainExtent;

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, config.graphicsPipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(config.swapChainExtent.width);
        viewport.height = static_cast<float>(config.swapChainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = config.swapChainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    bool ve_device::checkDeviceExtensionSupport(VkPhysicalDevice physicDevice) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(physicDevice, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(physicDevice, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions((config.deviceExtensions).begin(), (config.deviceExtensions).end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    bool ve_device::isDeviceSuitable(VkPhysicalDevice physicDevice) {
        QueueFamilyIndices indices = findQueueFamilies(physicDevice, config.surface);

        bool extensionsSupported = checkDeviceExtensionSupport(physicDevice);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicDevice, config.surface);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && extensionsSupported && swapChainAdequate;
    }

    void ve_device::createSyncObjects() {
        config.imageAvailableSemaphores.resize(config.MAX_FRAMES_IN_FLIGHT);
        config.renderFinishedSemaphores.resize(config.MAX_FRAMES_IN_FLIGHT);
        config.inFlightFences.resize(config.MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < config.MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(config.logicDevice, &semaphoreInfo, nullptr, &config.imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(config.logicDevice, &semaphoreInfo, nullptr, &config.renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(config.logicDevice, &fenceInfo, nullptr, &config.inFlightFences[i]) != VK_SUCCESS) {

                throw std::runtime_error("failed to create synchronization objects for a frame!");
                }
        }
    }

    void ve_device::drawFrame() {
        vkWaitForFences(config.logicDevice, 1, &config.inFlightFences[config.currentFrame], VK_TRUE, UINT64_MAX);
        vkResetFences(config.logicDevice, 1, &config.inFlightFences[config.currentFrame]);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(config.logicDevice, config.swapChain, UINT64_MAX, config.imageAvailableSemaphores[config.currentFrame], VK_NULL_HANDLE, &imageIndex);

        vkResetCommandBuffer(config.commandBuffers[config.currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
        recordCommandBuffer(config.commandBuffers[config.currentFrame], imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {config.imageAvailableSemaphores[config.currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &config.commandBuffers[config.currentFrame];

        VkSemaphore signalSemaphores[] = {config.renderFinishedSemaphores[config.currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(config.graphicsQueue, 1, &submitInfo, config.inFlightFences[config.currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {config.swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        vkQueuePresentKHR(config.presentQueue, &presentInfo);

        config.currentFrame = (config.currentFrame + 1) % config.MAX_FRAMES_IN_FLIGHT;
    }

}