#pragma once

#include "main.hpp"
#include <optional>

namespace ve {

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;

	bool isComplete() {
		return graphicsFamily.has_value();
	}
};

class ve_physicDevice 
{
public:
	ve_physicDevice(VkInstance* pvkInstance, VkPhysicalDevice* pphysicDevice);

private:
	void pickPhysicalDevice(VkInstance* pvkInstance, VkPhysicalDevice* pphysicDevice);
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
};

}