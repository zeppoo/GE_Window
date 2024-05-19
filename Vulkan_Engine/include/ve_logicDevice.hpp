#pragma once

#include "ve_configuration.hpp"
#include "ve_queueFamily.hpp"

namespace ve {

class ve_logicDevice
{
public:
	ve_logicDevice(ve_configuration& config);

	void createLogicalDevice();

private:
	ve_configuration& config;
};

}