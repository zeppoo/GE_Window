#include "ve_pipelineConfig.hpp"

namespace ve {

    ve_pipelineConfig::ve_pipelineConfig(ve::ve_configuration &config) : config{config} {}

    PipelineConfigInfo ve_pipelineConfig::createPipelineConfiguration() {
        PipelineConfigInfo configInfo{};

        //Dynamic State Configuration
        createDynamicStateInfo(configInfo.dynamicState);
        //Vertex Input State Configuration
        createVertexInputStateInfo(configInfo.vertexInputInfo);
        //Input Assembly State Configuration
        createInputAssemblyStateInfo(configInfo.inputAssemblyInfo);
        //ViewPort State Configuration
        createViewPortStateInfo( configInfo.viewport, configInfo.scissor);
        //Rasterization State Configuration
        createRasterizationStateInfo(configInfo.rasterizationInfo);
        //MultiSampling State Configuration
        createMultiSamplingStateInfo(configInfo.multisampleInfo);
        //Color Blend State Configuration
        createColorBlendStateInfo(configInfo.colorBlendInfo, configInfo.colorBlendAttachment);

        return configInfo;
    }

    void ve_pipelineConfig::createDynamicStateInfo(VkPipelineDynamicStateCreateInfo& dynamicState)
    {
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(config.dynamicStates.size());
        dynamicState.pDynamicStates = config.dynamicStates.data();
    }

    void ve_pipelineConfig::createVertexInputStateInfo(VkPipelineVertexInputStateCreateInfo& vertexInputInfo)
    {
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional
    }

    void ve_pipelineConfig::createInputAssemblyStateInfo(VkPipelineInputAssemblyStateCreateInfo& inputAssemblyInfo)
    {
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;
    }

    void ve_pipelineConfig::createViewPortStateInfo(VkViewport& viewport, VkRect2D& scissor)
    {
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(config.WIN_WIDTH);
        viewport.height = static_cast<float>(config.WIN_HEIGHT);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        scissor.offset = {0, 0};
        scissor.extent = {config.WIN_WIDTH, config.WIN_HEIGHT};
    }

    void ve_pipelineConfig::createRasterizationStateInfo(VkPipelineRasterizationStateCreateInfo& rasterizationInfo)
    {
        rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationInfo.depthClampEnable = VK_FALSE;
        rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizationInfo.lineWidth = 1.0f;
        rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizationInfo.depthBiasEnable = VK_FALSE;
        rasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
        rasterizationInfo.depthBiasClamp = 0.0f; // Optional
        rasterizationInfo.depthBiasSlopeFactor = 0.0f; // Optional
    }

    void ve_pipelineConfig::createMultiSamplingStateInfo(VkPipelineMultisampleStateCreateInfo& multisampleInfo)
    {
        multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleInfo.sampleShadingEnable = VK_FALSE;
        multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleInfo.minSampleShading = 1.0f; // Optional
        multisampleInfo.pSampleMask = nullptr; // Optional
        multisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampleInfo.alphaToOneEnable = VK_FALSE; // Optional
    }

    void ve_pipelineConfig::createColorBlendStateInfo(VkPipelineColorBlendStateCreateInfo& colorBlendInfo, VkPipelineColorBlendAttachmentState& colorBlendAttachment)
    {
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendInfo.logicOpEnable = VK_FALSE;
        colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlendInfo.attachmentCount = 1;
        colorBlendInfo.pAttachments = &colorBlendAttachment;
        colorBlendInfo.blendConstants[0] = 0.0f; // Optional
        colorBlendInfo.blendConstants[1] = 0.0f; // Optional
        colorBlendInfo.blendConstants[2] = 0.0f; // Optional
        colorBlendInfo.blendConstants[3] = 0.0f; // Optional
    }
}