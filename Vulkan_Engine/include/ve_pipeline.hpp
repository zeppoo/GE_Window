#pragma once

#include "ve_configuration.hpp"
#include <fstream>

namespace ve
{
    class ve_pipeline
    {
    public:
    	ve_pipeline(ve_configuration& config);

    	void createGraphicsPipeline();

    	void createShaderStages(VkShaderModule& vertShaderModule, VkShaderModule& fragShaderModule);

    	void ve_pipeline::createDynamicStates();

    	static std::vector<char> readFile(const std::string& filename);

    	VkShaderModule createShaderModule(const std::vector<char>& code);

	private:
		ve_configuration& config;

    };
}