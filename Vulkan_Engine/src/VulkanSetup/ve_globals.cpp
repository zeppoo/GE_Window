#include "ve_globals.hpp"

namespace ve {
  const int MAX_FRAMES_IN_FLIGHT = 2;
  uint32_t currentFrame = 0;

  uint32_t WIN_WIDTH; // Window Height
  uint32_t WIN_HEIGHT; // Window Width
  std::string NAME; // Window name
  GLFWwindow* window = nullptr; // GLFW window reference (pointer)
  VkInstance vkInstance = VK_NULL_HANDLE; // Vk Api instance
  VkDebugUtilsMessengerEXT debugMessenger; // Vulkan Debug Messenger Extension instance
  VkPhysicalDevice physicDevice = VK_NULL_HANDLE; // Physical GPU Reference
  VkDevice logicDevice; // the translator between the Family Queues and the GPU
  VkSurfaceKHR surface; // A surface which fully processed images will be shown on
  VkSwapchainKHR swapChain; // basically a queue of fully processed images waiting to be shown
  VkFormat swapChainImageFormat; // To cache our image format
  VkExtent2D swapChainExtent; // To cache our surface extent
  VkQueue graphicsQueue; // Queue for rendering images
  VkQueue presentQueue; // Queue for presenting rendered frames
  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;
  std::vector<VkFramebuffer> swapChainFramebuffers;
  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> commandBuffers;
  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" }; // Default validation layers
  std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
  std::vector<VkImage> swapChainImages;
  std::vector<VkImageView> swapChainImageViews;
  std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
  bool framebufferResized = false;

#ifdef NDEBUG // NDEBUG = No Debug
  const bool enableValidationLayers = false;
#else
  const bool enableValidationLayers = true;
#endif
}