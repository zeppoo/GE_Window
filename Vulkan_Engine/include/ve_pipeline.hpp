#pragma once

#include "ve_configuration.hpp"
#include <fstream>

namespace ve
{
    struct PipelineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineVertexInputStateCreateInfo vertexInputInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDynamicStateCreateInfo dynamicStates;
    };

    class ve_pipeline
    {
    public:
    	ve_pipeline(ve_configuration& config, PipelineConfigInfo& configInfo);

    	void createGraphicsPipeline();
        static PipelineConfigInfo createPipelineConfiguration(ve_configuration& config);

	private:
        static std::vector<char> readFile(const std::string& filename);
        VkShaderModule createShaderModule(const std::vector<char>& code);
        VkPipelineShaderStageCreateInfo createVertShaderStageInfo(VkShaderModule module);
        VkPipelineShaderStageCreateInfo createFragShaderStageInfo(VkShaderModule module);

		ve_configuration& config;
        PipelineConfigInfo& configInfo;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}