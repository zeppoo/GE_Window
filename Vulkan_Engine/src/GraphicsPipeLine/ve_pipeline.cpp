#include "ve_pipeline.hpp"

namespace ve {

    ve_pipeline::ve_pipeline(ve_configuration& config) : config{config} {}

    ve_pipeline::~ve_pipeline() {
        vkDestroyShaderModule(config.logicDevice, fragShaderModule, nullptr);
        vkDestroyShaderModule(config.logicDevice, vertShaderModule, nullptr);
    }

    void ve_pipeline::createShaderStages(VkPipelineShaderStageCreateInfo vertShaderStageInfo, VkPipelineShaderStageCreateInfo fragShaderStageInfo)
    {
        auto vertShaderCode = readFile("D:/Rein/Coding/C++/Projects/GE_Window/Vulkan_Engine/src/shaders/shader.vert.spv");
        auto fragShaderCode = readFile("D:/Rein/Coding/C++/Projects/GE_Window/Vulkan_Engine/src/shaders/shader.frag.spv");

        vertShaderModule = createShaderModule(vertShaderCode);
        fragShaderModule = createShaderModule(fragShaderCode);

        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";
    }

    std::vector<char> ve_pipeline::readFile(const std::string& filepath) {
        std::ifstream file{filepath, std::ios::ate | std::ios::binary};
        if (!file.is_open()) {
            throw std::runtime_error("failed to open file: " + filepath);
        }
        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }

    void ve_pipeline::createGraphicsPipeline(PipelineConfigInfo& configInfo) {


        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};

        createShaderStages(vertShaderStageInfo, fragShaderStageInfo);

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

        VkPipelineViewportStateCreateInfo viewportInfo = createViewPortStateInfo();

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &configInfo.vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
        pipelineInfo.pViewportState = &viewportInfo;
        pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
        pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
        pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
        pipelineInfo.pDepthStencilState = nullptr;
        pipelineInfo.pDynamicState = nullptr;

        pipelineInfo.layout = config.pipelineLayout;
        pipelineInfo.renderPass = config.renderPass;
        pipelineInfo.subpass = 0;

        if (vkCreateGraphicsPipelines(config.logicDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &config.graphicsPipeline) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }
    }


    VkShaderModule ve_pipeline::createShaderModule(const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(config.logicDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }

        return shaderModule;
    }

    PipelineConfigInfo ve_pipeline::createPipelineConfiguration() {
        PipelineConfigInfo configInfo{};

        //Dynamic State Configuration
        configInfo.dynamicState = createDynamicStateInfo();
        //Vertex Input State Configuration
        configInfo.vertexInputInfo = createVertexInputStateInfo();
        //Input Assembly State Configuration
        configInfo.inputAssemblyInfo =  createInputAssemblyStateInfo();
        //Rasterization State Configuration
        configInfo.rasterizationInfo =  createRasterizationStateInfo();
        //MultiSampling State Configuration
        configInfo.multisampleInfo = createMultiSamplingStateInfo();
        //Color Blend State Configuration
        configInfo.colorBlendInfo = createColorBlendStateInfo();
        //Shape Pipeline Layout
        createPipelineLayout(config.pipelineLayout);

        return configInfo;
    }


    VkPipelineDynamicStateCreateInfo ve_pipeline::createDynamicStateInfo()
    {
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(config.dynamicStates.size());
        dynamicState.pDynamicStates = config.dynamicStates.data();
        return dynamicState;
    }

    VkPipelineVertexInputStateCreateInfo ve_pipeline::createVertexInputStateInfo()
    {
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional
        return vertexInputInfo;
    }

    VkPipelineInputAssemblyStateCreateInfo ve_pipeline::createInputAssemblyStateInfo()
    {
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;
        return inputAssembly;
    }

    VkPipelineViewportStateCreateInfo ve_pipeline::createViewPortStateInfo()
    {
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) config.swapChainExtent.width;
        viewport.height = (float) config.swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = config.swapChainExtent;

        VkPipelineViewportStateCreateInfo viewportInfo{};
        viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo.viewportCount = 1;
        viewportInfo.pViewports = &viewport;
        viewportInfo.scissorCount = 1;
        viewportInfo.pScissors = &scissor;

        return viewportInfo;
    }

    VkPipelineRasterizationStateCreateInfo ve_pipeline::createRasterizationStateInfo()
    {
        VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
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

        return rasterizationInfo;
    }

    VkPipelineMultisampleStateCreateInfo ve_pipeline::createMultiSamplingStateInfo()
    {
        VkPipelineMultisampleStateCreateInfo multisampleInfo{};
        multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleInfo.sampleShadingEnable = VK_FALSE;
        multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleInfo.minSampleShading = 1.0f; // Optional
        multisampleInfo.pSampleMask = nullptr; // Optional
        multisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampleInfo.alphaToOneEnable = VK_FALSE; // Optional

        return multisampleInfo;
    }

    VkPipelineColorBlendAttachmentState ve_pipeline::createColorBlendAttachmentStateInfo()
    {
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        return colorBlendAttachment;
    }

    VkPipelineColorBlendStateCreateInfo ve_pipeline::createColorBlendStateInfo()
    {
        VkPipelineColorBlendAttachmentState colorBlendAttachment = createColorBlendAttachmentStateInfo();
        VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
        colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendInfo.logicOpEnable = VK_FALSE;
        colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlendInfo.attachmentCount = 1;
        colorBlendInfo.pAttachments = &colorBlendAttachment;
        colorBlendInfo.blendConstants[0] = 0.0f; // Optional
        colorBlendInfo.blendConstants[1] = 0.0f; // Optional
        colorBlendInfo.blendConstants[2] = 0.0f; // Optional
        colorBlendInfo.blendConstants[3] = 0.0f; // Optional

        return colorBlendInfo;
    }

    void ve_pipeline::createPipelineLayout(VkPipelineLayout pipelineLayout)
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

        if (vkCreatePipelineLayout(config.logicDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

}