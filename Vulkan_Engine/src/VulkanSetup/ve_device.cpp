#include "ve_device.hpp"
#include "ve_swapChain.hpp"
#include "ve_shaderBuffers.hpp"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <set>

namespace ve
{
  void pickPhysicalDevice()
  {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);
    if (deviceCount == 0)
    {
      throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

    for (const auto &device: devices)
    {
      if (isDeviceSuitable(device))
      {
        physicDevice = device;
        break;
      }
    }

    if (physicDevice == VK_NULL_HANDLE)
    {
      throw std::runtime_error("failed to find a suitable GPU!");
    }
  }

  void createLogicalDevice()
  {
    // Find queue families
    QueueFamilyIndices indices = findQueueFamilies(physicDevice, surface);

    // Ensure queue families are complete before proceeding
    if (!indices.isComplete())
    {
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
    for (uint32_t queueFamily: uniqueQueueFamilies)
    {
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
    createInfo.enabledExtensionCount = static_cast<uint32_t>((deviceExtensions).size());
    createInfo.ppEnabledExtensionNames = (deviceExtensions).data();

    // Validation layers
    if (enableValidationLayers)
    {
      createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
      createInfo.ppEnabledLayerNames = validationLayers.data();
    } else
    {
      createInfo.enabledLayerCount = 0;
    }

    // Create the logical device
    if (vkCreateDevice(physicDevice, &createInfo, nullptr, &logicDevice) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create logical device!");
    }

    // Retrieve the queue handles
    vkGetDeviceQueue(logicDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(logicDevice, indices.presentFamily.value(), 0, &presentQueue);
  }

  void createCommandPool()
  {
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicDevice, surface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(logicDevice, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create command pool!");
    }
  }

  void createCommandBuffer()
  {
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(logicDevice, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to allocate command buffers!");
    }
  }

  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
  {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChainExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) swapChainExtent.width;
    viewport.height = (float) swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      throw std::runtime_error("failed to record command buffer!");
    }
  }

  void createSyncObjects()
  {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
      if (vkCreateSemaphore(logicDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) !=
          VK_SUCCESS ||
          vkCreateSemaphore(logicDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) !=
          VK_SUCCESS ||
          vkCreateFence(logicDevice, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
      {

        throw std::runtime_error("failed to create synchronization objects for a frame!");
      }
    }
  }

  void drawFrame()
  {
    vkWaitForFences(logicDevice, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(logicDevice, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      recreateSwapChain();
      return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("failed to acquire swap chain image!");
    }

    updateUniformBuffer(currentFrame);

    vkResetFences(logicDevice, 1, &inFlightFences[currentFrame]);

    vkResetCommandBuffer(commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
    recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
      throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
      framebufferResized = false;
      recreateSwapChain();
    } else if (result != VK_SUCCESS) {
      throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
  }

  static bool checkDeviceExtensionSupport(VkPhysicalDevice physicDevice)
  {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physicDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions((deviceExtensions).begin(), (deviceExtensions).end());

    for (const auto &extension: availableExtensions)
    {
      requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
  }

  static bool isDeviceSuitable(VkPhysicalDevice physicDevice)
  {
    QueueFamilyIndices indices = findQueueFamilies(physicDevice, surface);

    bool extensionsSupported = checkDeviceExtensionSupport(physicDevice);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
      SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicDevice, surface);
      swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
  }

  static void updateUniformBuffer(uint32_t currentImage) {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(20.0f), swapChainExtent.width / (float) swapChainExtent.height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
  }
}