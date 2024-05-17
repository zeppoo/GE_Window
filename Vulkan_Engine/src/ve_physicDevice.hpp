#pragma once

#include "main.hpp"
#include "ve_queueFamily.hpp"


namespace ve {

class ve_physicDevice 
{
public:
	void pickPhysicalDevice(VkInstance* pvkInstance, VkPhysicalDevice* pphysicDevice, VkSurfaceKHR* psurface);
};

}