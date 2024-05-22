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
            VkPipelineViewportStateCreateInfo viewportInfo;
            VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
            VkPipelineRasterizationStateCreateInfo rasterizationInfo;
            VkPipelineMultisampleStateCreateInfo multisampleInfo;
            VkPipelineColorBlendAttachmentState colorBlendAttachment;
            VkPipelineColorBlendStateCreateInfo colorBlendInfo;
            VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
            VkPipelineLayout pipelineLayout = nullptr;
            VkRenderPass renderPass = nullptr;
            uint32_t subpass = 0;
    };

    class ve_pipeline
    {
    public:
    	ve_pipeline(ve_configuration& config);
        ~ve_pipeline();

    	void createGraphicsPipeline();

	private:
        void createShaderStages();

        static std::vector<char> readFile(const std::string& filename);

        VkShaderModule createShaderModule(const std::vector<char>& code);

        void createPipelineLayout();

		ve_configuration& config;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}