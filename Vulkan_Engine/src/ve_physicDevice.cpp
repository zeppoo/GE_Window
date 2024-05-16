#include "ve_physicDevice.hpp"
#include <iostream>
#include <vector>

namespace ve {
    
    ve_physicDevice::ve_physicDevice(VkInstance* pvkInstance, VkPhysicalDevice* pphysicDevice)
    {
        pickPhysicalDevice(pvkInstance, pphysicDevice);
    }


    void ve_physicDevice::pickPhysicalDevice(VkInstance* pvkInstance, VkPhysicalDevice* pphysicDevice) {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(*pvkInstance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(*pvkInstance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            if (isDeviceSuitable(device)) {
                *pphysicDevice = device;
                break;
            }
        }

        if (*pphysicDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }

    }

    bool ve_physicDevice::isDeviceSuitable(VkPhysicalDevice device) {
        QueueFamilyIndices indices = findQueueFamilies(device);

        return indices.graphicsFamily.has_value();
    }

    QueueFamilyIndices ve_physicDevice::findQueueFamilies(VkPhysicalDevice device) {
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

