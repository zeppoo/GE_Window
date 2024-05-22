#include "ve_configuration.hpp"

namespace ve {
	
	ve_configuration::ve_configuration(const uint32_t w, const uint32_t h, const std::string name)
    : 
    WIN_WIDTH{w}, // Default width
    WIN_HEIGHT{h}, // Default height
    NAME(name), // Default name
    window(nullptr), // Default window pointer
    vkInstance{VK_NULL_HANDLE}, // Default Vulkan instance
    debugMessenger(VK_NULL_HANDLE), // Default debug messenger
    physicDevice(VK_NULL_HANDLE), // Default physical device
    logicDevice(VK_NULL_HANDLE), // Default logical device
    surface(VK_NULL_HANDLE), // Default surface
    swapChain(VK_NULL_HANDLE),
    swapChainImageFormat(),
    swapChainExtent(),// Default swap chain
    graphicsQueue(VK_NULL_HANDLE), // Default graphics queue
    presentQueue(VK_NULL_HANDLE), // Default present queue
	renderPass(VK_NULL_HANDLE),
	graphicsPipeline(VK_NULL_HANDLE),
    pipelineLayout(VK_NULL_HANDLE),
    validationLayers({"VK_LAYER_KHRONOS_validation"}), // Default validation layers
    deviceExtensions({VK_KHR_SWAPCHAIN_EXTENSION_NAME}), // Default device extensions
    swapChainImages({VK_NULL_HANDLE}),
    swapChainImageViews({VK_NULL_HANDLE}),
	dynamicStates({VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR})
{}

}