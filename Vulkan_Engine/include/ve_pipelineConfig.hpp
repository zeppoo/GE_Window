#pragma once

#include "ve_configuration.hpp"

namespace ve {

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
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class ve_pipelineConfig {
    public:
        ve_pipelineConfig(ve_configuration& config);

        PipelineConfigInfo createPipelineConfiguration();

    private:
        void createDynamicStateInfo(VkPipelineDynamicStateCreateInfo& dynamicState);
        void createVertexInputStateInfo(VkPipelineVertexInputStateCreateInfo& vertexInputInfo);
        void createInputAssemblyStateInfo(VkPipelineInputAssemblyStateCreateInfo& inputAssemblyInfo);
        void createViewPortStateInfo( VkViewport& viewport, VkRect2D& scissor);
        void createRasterizationStateInfo(VkPipelineRasterizationStateCreateInfo& rasterizationInfo);
        void createMultiSamplingStateInfo(VkPipelineMultisampleStateCreateInfo& multisampleInfo);
        void createColorBlendStateInfo(VkPipelineColorBlendStateCreateInfo& colorBlendInfo, VkPipelineColorBlendAttachmentState& colorBlendAttachment);

        ve_configuration& config;

    };

}
