#include "ve_configuration.hpp"

namespace ve {
	
	ve_configuration::ve_configuration(const uint32_t w, const uint32_t h, const std::string name)
    :
	MAX_FRAMES_IN_FLIGHT(2),
    WIN_WIDTH{w}, // Default width
    WIN_HEIGHT{h}, // Default height
    NAME(name), // Default name
    window(nullptr), // Default window pointer
    vkInstance{VK_NULL_HANDLE}, // Default Vulkan instance
    debugMessenger(), // Default debug messenger
    physicDevice(VK_NULL_HANDLE), // Default physical device
    logicDevice(), // Default logical device
    surface(), // Default surface
    swapChain(),
    swapChainImageFormat(),
    swapChainExtent(),// Default swap chain
    graphicsQueue(), // Default graphics queue
    presentQueue(), // Default present queue
	renderPass(),
	graphicsPipeline(),
    pipelineLayout(),
	swapChainFramebuffers({}),
	commandPool(),
	commandBuffers(),
	imageAvailableSemaphores(),
	renderFinishedSemaphores(),
	inFlightFences(),
    validationLayers({"VK_LAYER_KHRONOS_validation"}), // Default validation layers
    deviceExtensions({VK_KHR_SWAPCHAIN_EXTENSION_NAME}), // Default device extensions
    swapChainImages({}),
    swapChainImageViews({}),
	dynamicStates ({VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR})
{}

}