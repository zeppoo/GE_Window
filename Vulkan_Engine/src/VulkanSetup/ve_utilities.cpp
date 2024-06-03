#include "ve_utilities.hpp"
#include "ve_swapChain.hpp"

namespace ve
{
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicDevice, VkSurfaceKHR surface)
  {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto &queueFamily: queueFamilies)
    {
      if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
      {
        indices.graphicsFamily = i;
      }

      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(physicDevice, i, surface, &presentSupport);
      if (presentSupport)
      {
        indices.presentFamily = i;
      }

      if (indices.isComplete())
      {
        break;
      }

      i++;
    }

    return indices;
  }

  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
  {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
      details.formats.resize(formatCount);
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
      details.presentModes.resize(presentModeCount);
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
  }

  PipelineConfigInfo createPipelineConfiguration()
  {
    PipelineConfigInfo configInfo{};

    configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    configInfo.viewport.x = 0.0f;
    configInfo.viewport.y = 0.0f;
    configInfo.viewport.width = static_cast<float>(swapChainExtent.width);
    configInfo.viewport.height = static_cast<float>(swapChainExtent.height);
    configInfo.viewport.minDepth = 0.0f;
    configInfo.viewport.maxDepth = 1.0f;

    configInfo.scissor.offset = {0, 0};
    configInfo.scissor.extent = {swapChainExtent.width, swapChainExtent.height};

    configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
    configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    configInfo.rasterizationInfo.lineWidth = 1.0f;
    configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;

    configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
    configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
    configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
    configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
    configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

    configInfo.colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;
    configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
    configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
    configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

    configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
    configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
    configInfo.colorBlendInfo.attachmentCount = 1;
    configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
    configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional


    configInfo.dynamicStates.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    configInfo.dynamicStates.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    configInfo.dynamicStates.pDynamicStates = dynamicStates.data();

    return configInfo;
  }

  void cleanupSwapChain() {
    for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
      vkDestroyFramebuffer(logicDevice, swapChainFramebuffers[i], nullptr);
    }

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
      vkDestroyImageView(logicDevice, swapChainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(logicDevice, swapChain, nullptr);
  }

  void recreateSwapChain()
  {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
      glfwGetFramebufferSize(window, &width, &height);
      glfwWaitEvents();
    }

    vkDeviceWaitIdle(logicDevice);

    cleanupSwapChain();
    createSwapChain();
    createImageViews();
    createFramebuffers();
  }
}