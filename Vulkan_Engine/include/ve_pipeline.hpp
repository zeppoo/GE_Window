#pragma once

#include "ve_configuration.hpp"
#include <fstream>

namespace ve
{
    struct PipelineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineDynamicStateCreateInfo dynamicState;
        VkPipelineVertexInputStateCreateInfo vertexInputInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    };

    class ve_pipeline
    {
    public:
    	ve_pipeline(ve_configuration& config);
        ~ve_pipeline();

    	void createGraphicsPipeline(PipelineConfigInfo& pipelineInfo);
        PipelineConfigInfo createPipelineConfiguration();

	private:
        static std::vector<char> readFile(const std::string& filename);
        VkShaderModule createShaderModule(const std::vector<char>& code);
        VkPipelineShaderStageCreateInfo createVertShaderStageInfo(VkShaderModule module);
        VkPipelineShaderStageCreateInfo createFragShaderStageInfo(VkShaderModule module);

		ve_configuration& config;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}