#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "vulkan/vulkan.h"
#include <iostream>
#include <vector>
#include <string>

namespace ve {

    class ve_configuration 
    {
    public:
        ve_configuration(const int w, const int h, const std::string name);

        int WIN_WIDTH;// Window Height
        int WIN_HEIGHT;// Window Width
        std::string NAME;// Window name
        GLFWwindow* window;// GLFW window reference (pointer)
        VkInstance vkInstance; // Vk Api instance
        VkDebugUtilsMessengerEXT debugMessenger;// Vulkan Debug Messenger Extension instance
        VkPhysicalDevice physicDevice  = VK_NULL_HANDLE;// Physical GPU Reference
        VkDevice logicDevice;// the translator between the Family Queues and the GPU
        VkSurfaceKHR surface;// A surface which fully proccesed images will be shown on
		VkSwapchainKHR swapChain;// basically a queue of fully proccesed images waiting to be shown
        VkFormat swapChainImageFormat;// To cache our image format
        VkExtent2D swapChainExtent;// To cache our surface extent
        VkQueue graphicsQueue;// Queue for rendering images
        VkQueue presentQueue;// Queue for presenting rendered frames
        std::vector<const char*> validationLayers  = { "VK_LAYER_KHRONOS_validation" };// Some default validation Layers from Khronos
        std::vector<const char*> deviceExtensions  = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
        std::vector<VkImage> swapChainImages;// VkImages stored in the swapchain
        std::vector<VkImageView> swapChainImageViews; // A way to grab VkImage data


        #ifdef NDEBUG // NDEBUG = No Debug
			const bool enableValidationLayers = false;
		#else
			const bool enableValidationLayers = true;
		#endif
    };

}