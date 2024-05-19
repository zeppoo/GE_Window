#include "ve_logicDevice.hpp"

namespace ve {

    ve_logicDevice::ve_logicDevice(ve_configuration& config) : config{config} {}

    void ve_logicDevice::createLogicalDevice() 
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

}