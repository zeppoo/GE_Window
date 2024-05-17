#pragma once

#include "main.hpp"
#include "ve_queueFamily.hpp"

namespace ve {

class ve_logicDevice
{
public:
	void createLogicalDevice(VkDevice* pdevice, VkQueue* pgraphicsQueue, VkQueue* ppresentQueue, VkPhysicalDevice* pphysicDevice, const bool* enableValidationLayers, const std::vector<const char*>* validationLayers, VkSurfaceKHR* psurface);

};

}