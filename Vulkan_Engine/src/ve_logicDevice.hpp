#pragma once

#include "main.hpp"
#include <optional>
#include <vector>


namespace ve {

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
};

class ve_logicDevice
{
public:
	ve_logicDevice(VkDevice* pdevice, VkQueue* pgraphicsQueue, VkPhysicalDevice* pphysicDevice);
	~ve_logicDevice();


private:
	void createLogicalDevice(VkDevice* pdevice, VkQueue* pgraphicsQueue, VkPhysicalDevice* pphysicDevice);

	bool isDeviceSuitable(VkPhysicalDevice device);

	uint32_t findQueueFamilies(VkPhysicalDevice device);
};

}