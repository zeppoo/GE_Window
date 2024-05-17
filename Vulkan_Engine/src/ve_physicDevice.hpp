#pragma once

#include "main.hpp"
#include "ve_queueFamily.hpp"


namespace ve {

class ve_physicDevice 
{
public:
	ve_physicDevice(VkInstance* pvkInstance, VkPhysicalDevice* pphysicDevice);

private:
	void pickPhysicalDevice(VkInstance* pvkInstance, VkPhysicalDevice* pphysicDevice);
};

}