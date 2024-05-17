#include "ve_physicDevice.hpp"
#include <iostream>
#include <vector>

namespace ve {

    void ve_physicDevice::pickPhysicalDevice(VkInstance* pvkInstance, VkPhysicalDevice* pphysicDevice, VkSurfaceKHR* psurface) {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(*pvkInstance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(*pvkInstance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            if (isDeviceSuitable(device, *psurface)) {
                *pphysicDevice = device;
                break;
            }
        }

        if (*pphysicDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }

    }
}

