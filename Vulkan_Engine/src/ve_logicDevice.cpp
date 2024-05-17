#include "ve_logicDevice.hpp"

namespace ve {

    void ve_logicDevice::createLogicalDevice(VkDevice* pdevice, VkQueue* pgraphicsQueue, VkQueue* ppresentQueue,  VkPhysicalDevice* pphysicDevice, const bool* enableValidationLayers, const std::vector<const char*>* validationLayers, VkSurfaceKHR* psurface) {

        QueueFamilyIndices indices = findQueueFamilies(*pphysicDevice, *psurface);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = 0;

        if (*enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>((*validationLayers).size());
            createInfo.ppEnabledLayerNames = (*validationLayers).data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(*pphysicDevice, &createInfo, nullptr, pdevice) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(*pdevice, indices.graphicsFamily.value(), 0, pgraphicsQueue);
        vkGetDeviceQueue(*pdevice, indices.presentFamily.value(), 0, ppresentQueue);
    }
}