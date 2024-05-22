#pragma once

#include "ve_configuration.hpp"
#include "ve_pipelineConfig.hpp"
#include <fstream>

namespace ve
{
    class ve_pipeline
    {
    public:
    	ve_pipeline(ve_configuration& config, PipelineConfigInfo& pipelineInfo);
        ~ve_pipeline();

    	void createGraphicsPipeline(PipelineConfigInfo& pipelineInfo);

	private:
        static std::vector<char> readFile(const std::string& filename);

        VkShaderModule createShaderModule(const std::vector<char>& code);

        void createPipelineLayout();

		ve_configuration& config;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}