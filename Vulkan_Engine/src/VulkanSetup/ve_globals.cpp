#include "ve_globals.hpp"

namespace ve {
  const int MAX_FRAMES_IN_FLIGHT = 2; // Maximum number of frames that can be processed concurrently
    uint32_t currentFrame = 0; // Index of the current frame being processed

    uint32_t WIN_WIDTH; // Window width
    uint32_t WIN_HEIGHT; // Window height
    std::string NAME; // Window name
    GLFWwindow* window = nullptr; // GLFW window reference (pointer)
    VkSurfaceKHR surface; // Vulkan surface for rendering

    VkInstance vkInstance = VK_NULL_HANDLE; // Vulkan instance
    VkDebugUtilsMessengerEXT debugMessenger; // Vulkan debug messenger
    VkPhysicalDevice physicDevice = VK_NULL_HANDLE; // Physical GPU reference
    VkDevice logicDevice; // Logical device

    VkPipelineLayout pipelineLayout; // Pipeline layout
    VkPipeline graphicsPipeline; // Graphics pipeline
    VkQueue graphicsQueue; // Queue for rendering images
    VkRenderPass renderPass; // Render pass

    VkSwapchainKHR swapChain; // Swapchain for image presentation
    VkFormat swapChainImageFormat; // Format of swapchain images
    VkExtent2D swapChainExtent; // Extent of swapchain images
    std::vector<VkImage> swapChainImages; // Images in the swapchain
    std::vector<VkImageView> swapChainImageViews; // Image views for the swapchain images
    VkQueue presentQueue; // Queue for presenting rendered frames

    VkBuffer vertexBuffer; // Vertex buffer
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    VkCommandPool commandPool; // Command pool
    std::vector<VkFramebuffer> swapChainFramebuffers; // Framebuffers for the swapchain
    std::vector<VkCommandBuffer> commandBuffers; // Command buffers

    std::vector<VkSemaphore> imageAvailableSemaphores; // Semaphores for image availability
    std::vector<VkSemaphore> renderFinishedSemaphores; // Semaphores for render completion
    std::vector<VkFence> inFlightFences; // Fences for frame synchronization
    std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" }; // Validation layers
    std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME }; // Device extensions
    std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR }; // Dynamic states for the pipeline
    bool framebufferResized = false; // Flag indicating if the framebuffer was resized

#ifdef NDEBUG // NDEBUG = No Debug
    const bool enableValidationLayers = false; // Enable validation layers in debug mode
#else
    const bool enableValidationLayers = true; // Enable validation layers in debug mode
#endif
}