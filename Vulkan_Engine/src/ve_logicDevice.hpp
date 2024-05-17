#pragma once

#include "main.hpp"
#include "ve_queueFamily.hpp"

namespace ve {

class ve_logicDevice
{
public:
	ve_logicDevice(VkDevice* pdevice, VkQueue* pgraphicsQueue, VkPhysicalDevice* pphysicDevice, const bool* enableValidationLayers, const std::vector<const char*>* validationLayers);
	~ve_logicDevice();


private:
	void createLogicalDevice(VkDevice* pdevice, VkQueue* pgraphicsQueue, VkPhysicalDevice* pphysicDevice, const bool* enableValidationLayers, const std::vector<const char*>* validationLayers);

};

}