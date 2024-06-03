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

  //Window Data
  extern uint32_t WIN_WIDTH;
  extern uint32_t WIN_HEIGHT;
  extern std::string NAME;
  extern GLFWwindow *window;
  extern VkSurfaceKHR surface;

  //Vulkan Device
  extern VkInstance vkInstance;
  extern VkDebugUtilsMessengerEXT debugMessenger;
  extern VkPhysicalDevice physicDevice;
  extern VkDevice logicDevice;

  // Vulkan Graphics pipeline
  extern VkPipelineLayout pipelineLayout;
  extern VkPipeline graphicsPipeline;
  extern VkQueue graphicsQueue;
  extern VkRenderPass renderPass;

  // Vulkan SwapChain
  extern VkSwapchainKHR swapChain;
  extern VkFormat swapChainImageFormat;
  extern VkExtent2D swapChainExtent;
  extern std::vector<VkImage> swapChainImages;
  extern std::vector<VkImageView> swapChainImageViews;
  extern VkQueue presentQueue;

  // Vulkan Buffers
  extern VkBuffer vertexBuffer;
  extern VkDeviceMemory vertexBufferMemory;
  extern VkBuffer indexBuffer;
  extern VkDeviceMemory indexBufferMemory;
  extern VkCommandPool commandPool;
  extern std::vector<VkFramebuffer> swapChainFramebuffers;
  extern std::vector<VkCommandBuffer> commandBuffers;

  // Vulkan Utilities and more
  extern std::vector<VkSemaphore> imageAvailableSemaphores;
  extern std::vector<VkSemaphore> renderFinishedSemaphores;
  extern std::vector<VkFence> inFlightFences;
  extern std::vector<const char *> validationLayers;
  extern std::vector<const char *> deviceExtensions;
  extern std::vector<VkDynamicState> dynamicStates;
  extern bool framebufferResized;

#ifdef NDEBUG // NDEBUG = No Debug
  extern const bool enableValidationLayers;
#else
  extern const bool enableValidationLayers;
#endif


}
