#pragma once

#include "ve_configuration.hpp"
#include <fstream>

namespace ve
{
    struct PipelineConfigInfo {
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
        void createShaderStages(VkPipelineShaderStageCreateInfo vertShaderStageInfo, VkPipelineShaderStageCreateInfo fragShaderStageInfo);
        static std::vector<char> readFile(const std::string& filename);
        VkShaderModule createShaderModule(const std::vector<char>& code);

        VkPipelineDynamicStateCreateInfo createDynamicStateInfo();
        VkPipelineVertexInputStateCreateInfo createVertexInputStateInfo();
        VkPipelineInputAssemblyStateCreateInfo createInputAssemblyStateInfo();
        VkPipelineViewportStateCreateInfo createViewPortStateInfo();
        VkPipelineRasterizationStateCreateInfo createRasterizationStateInfo();
        VkPipelineMultisampleStateCreateInfo createMultiSamplingStateInfo();
        VkPipelineColorBlendAttachmentState createColorBlendAttachmentStateInfo();
        VkPipelineColorBlendStateCreateInfo createColorBlendStateInfo();
        void createPipelineLayout(VkPipelineLayout pipelineLayout);

		ve_configuration& config;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}