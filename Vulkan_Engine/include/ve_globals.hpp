#pragma once

#define GLFW_INCLUDE_VULKAN

#include "GLFW/glfw3.h"
#include "vulkan/vulkan.h"
#include <iostream>
#include <vector>
#include <string>

namespace ve
{
  extern const int MAX_FRAMES_IN_FLIGHT;
  extern uint32_t currentFrame;

  extern uint32_t WIN_WIDTH; // Window Height
  extern uint32_t WIN_HEIGHT; // Window Width
  extern std::string NAME; // Window name
  extern GLFWwindow *window; // GLFW window reference (pointer)
  extern VkInstance vkInstance; // Vk Api instance
  extern VkDebugUtilsMessengerEXT debugMessenger; // Vulkan Debug Messenger Extension instance
  extern VkPhysicalDevice physicDevice; // Physical GPU Reference
  extern VkDevice logicDevice; // the translator between the Family Queues and the GPU
  extern VkSurfaceKHR surface; // A surface which fully processed images will be shown on
  extern VkSwapchainKHR swapChain; // basically a queue of fully processed images waiting to be shown
  extern VkFormat swapChainImageFormat; // To cache our image format
  extern VkExtent2D swapChainExtent; // To cache our surface extent
  extern VkQueue graphicsQueue; // Queue for rendering images
  extern VkQueue presentQueue; // Queue for presenting rendered frames
  extern VkRenderPass renderPass;
  extern VkPipelineLayout pipelineLayout;
  extern VkPipeline graphicsPipeline;
  extern std::vector<VkFramebuffer> swapChainFramebuffers;
  extern VkCommandPool commandPool;
  extern std::vector<VkCommandBuffer> commandBuffers;
  extern std::vector<VkSemaphore> imageAvailableSemaphores;
  extern std::vector<VkSemaphore> renderFinishedSemaphores;
  extern std::vector<VkFence> inFlightFences;
  extern std::vector<const char *> validationLayers;
  extern std::vector<const char *> deviceExtensions;
  extern std::vector<VkImage> swapChainImages;
  extern std::vector<VkImageView> swapChainImageViews;
  extern std::vector<VkDynamicState> dynamicStates;
  extern bool framebufferResized;

#ifdef NDEBUG // NDEBUG = No Debug
  extern const bool enableValidationLayers;
#else
  extern const bool enableValidationLayers;
#endif


}
