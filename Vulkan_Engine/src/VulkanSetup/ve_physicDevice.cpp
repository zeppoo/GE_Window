#include "ve_physicDevice.hpp"
#include <iostream>
#include <vector>

namespace ve {

    ve_physicDevice::ve_physicDevice(ve_configuration& config) : config{config} {}

    void ve_physicDevice::pickPhysicalDevice() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(config.vkInstance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(config.vkInstance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            if (isDeviceSuitable(device, config.surface, config.deviceExtensions)) {
                config.physicDevice = device;
                break;
            }
        }

        if (config.physicDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }

    }
}

