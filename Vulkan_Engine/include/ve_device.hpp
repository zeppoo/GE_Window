#pragma once

#include "ve_configuration.hpp"
#include <optional>

namespace ve
{
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    class ve_device
    {
    public:
        ve_device(ve_configuration& config);

        void pickPhysicalDevice();
        void createLogicalDevice();

    private:
        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice physicDevice);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicDevice);

        ve_configuration& config;
    };
}