#pragma once

#include "ve_configuration.hpp"
#include "ve_pipeline.hpp"

namespace ve {
    class ve_pipelineConfig {
    public:
        ve_pipelineConfig(ve_configuration& config);

        PipelineConfigInfo createPipelineConfiguration();

    private:
        void createDynamicStateInfo(VkPipelineDynamicStateCreateInfo& dynamicState);
        void createVertexInputStateInfo(VkPipelineVertexInputStateCreateInfo& vertexInputInfo);
        void createInputAssemblyStateInfo(VkPipelineInputAssemblyStateCreateInfo& inputAssemblyInfo);
        void createViewPortStateInfo(VkPipelineViewportStateCreateInfo& viewportInfo, VkViewport& viewport, VkRect2D& scissor);
        void createRasterizationStateInfo(VkPipelineRasterizationStateCreateInfo& rasterizationInfo);
        void createMultiSamplingStateInfo(VkPipelineMultisampleStateCreateInfo& multisampleInfo);
        void createColorBlendStateInfo(VkPipelineColorBlendStateCreateInfo& colorBlendInfo, VkPipelineColorBlendAttachmentState& colorBlendAttachment);

        ve_configuration& config;

    };

}
