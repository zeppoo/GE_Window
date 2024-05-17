#include "ve_logicDevice.hpp"

namespace ve {

	ve_logicDevice::ve_logicDevice(VkDevice* pdevice, VkQueue* pgraphicsQueue, VkPhysicalDevice* pphysicDevice, const bool* enableValidationLayers, const std::vector<const char*>* validationLayers)
	{
        createLogicalDevice(pdevice, pgraphicsQueue, pphysicDevice, enableValidationLayers, validationLayers);
	}

	ve_logicDevice::~ve_logicDevice()
	{

	}

    void ve_logicDevice::createLogicalDevice(VkDevice* pdevice, VkQueue* pgraphicsQueue, VkPhysicalDevice* pphysicDevice, const bool* enableValidationLayers, const std::vector<const char*>* validationLayers) {

        QueueFamilyIndices indices = findQueueFamilies(*pphysicDevice);

        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;

        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
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
    }
}