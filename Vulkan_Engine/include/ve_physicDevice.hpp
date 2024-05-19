#pragma once

#include "ve_configuration.hpp"
#include "ve_queueFamily.hpp"


namespace ve {

class ve_physicDevice 
{
public:
	ve_physicDevice(ve_configuration& config);

	void pickPhysicalDevice();

private:
	ve_configuration& config;
};

}