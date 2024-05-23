#include "ve_device.hpp"
#include "ve_swapChain.hpp"
#include <set>


namespace ve
{
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
        QueueFamilyIndices indices = findQueueFamilies(config.physicDevice);

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

    QueueFamilyIndices ve_device::findQueueFamilies(VkPhysicalDevice physicDevice) {
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
            vkGetPhysicalDeviceSurfaceSupportKHR(physicDevice, i, config.surface, &presentSupport);
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

    bool ve_device::isDeviceSuitable(VkPhysicalDevice physicDevice) {
        QueueFamilyIndices indices = findQueueFamilies(physicDevice);

        bool extensionsSupported = checkDeviceExtensionSupport(physicDevice);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicDevice, config.surface);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && extensionsSupported && swapChainAdequate;
    }

}