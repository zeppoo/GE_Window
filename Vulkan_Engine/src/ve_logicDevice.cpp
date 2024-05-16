#include "ve_logicDevice.hpp"

namespace ve {

	ve_logicDevice::ve_logicDevice(VkDevice* device, VkQueue* graphicsQueue, VkPhysicalDevice* physicDevice) : device {device}, graphicsQueue {graphicsQueue}, physicDevice {physicDevice}
	{

	}

	ve_logicDevice::~ve_logicDevice()
	{

	}

    void ve_logicDevice::createLogicalDevice(VkDevice* pdevice, VkQueue* pgraphicsQueue, VkPhysicalDevice* pphysicDevice) {
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

        if (vkCreateDevice(*pphysicDevice, &createInfo, nullptr, pdevice) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(*pdevice, indices.graphicsFamily.value(), 0, pgraphicsQueue);
    }

    uint32_t ve_logicDevice::findQueueFamilies(VkPhysicalDevice device) {

        QueueFamilyIndices indices;
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            i++;
        }

        return indices;
    }

}