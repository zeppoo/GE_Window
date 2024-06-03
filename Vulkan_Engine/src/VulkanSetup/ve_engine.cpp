#include "ve_engine.hpp"
#include "ve_globals.hpp"
#include "ve_debugMessenger.hpp"
#include "ve_window.hpp"
#include "ve_device.hpp"
#include "ve_swapChain.hpp"
#include "ve_shaderBuffers.hpp"
#include <cstring>

namespace ve
{
  void ve_engine::Cleanup()
  {
    vkDeviceWaitIdle(logicDevice);
    cleanupSwapChain();

    vkDestroyBuffer(logicDevice, indexBuffer, nullptr);
    vkFreeMemory(logicDevice, indexBufferMemory, nullptr);

    vkDestroyBuffer(logicDevice, vertexBuffer, nullptr);
    vkFreeMemory(logicDevice, vertexBufferMemory, nullptr);

    vkDestroyPipeline(logicDevice, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(logicDevice, pipelineLayout, nullptr);

    vkDestroyRenderPass(logicDevice, renderPass, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      vkDestroySemaphore(logicDevice, renderFinishedSemaphores[i], nullptr);
      vkDestroySemaphore(logicDevice, imageAvailableSemaphores[i], nullptr);
      vkDestroyFence(logicDevice, inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(logicDevice, commandPool, nullptr);

    vkDestroyDevice(logicDevice, nullptr);

    if (enableValidationLayers) {
      DestroyDebugUtilsMessengerEXT(vkInstance, debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(vkInstance, surface, nullptr);
    vkDestroyInstance(vkInstance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
  }

  void ve_engine::initEngine()
  {
    initGLFW();
    initVULKAN();
    initWindow();

    if (enableValidationLayers)
    {
      setupDebugMessenger();
    }

    // Wrong order of calling might result in SEGGV
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    auto pipelineInfo = createPipelineConfiguration();
    createGraphicsPipeline(pipelineInfo);
    createFramebuffers();
    createCommandPool();
    createVertexBuffer();
    createIndexBuffer();
    createCommandBuffer();
    createSyncObjects();

    windowLoop();

    Cleanup();
  }

  void ve_engine::initGLFW()
  {
    if (!glfwInit())
    {
      std::cerr << "Failed to initialize GLFW\n";
      return;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  }

  void ve_engine::initVULKAN()
  {
    if (enableValidationLayers && !checkValidationLayerSupport())
    {
      throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (enableValidationLayers)
    {
      createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
      createInfo.ppEnabledLayerNames = validationLayers.data();
      populateDebugMessengerCreateInfo(debugCreateInfo);
      createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
    } else
    {
      createInfo.enabledLayerCount = 0;
      createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &vkInstance) != VK_SUCCESS)
    {
      vkDestroyInstance(vkInstance, nullptr);
      glfwDestroyWindow(window);
      glfwTerminate();
      throw std::runtime_error("Failed to create Vulkan instance");
    }
  }

  bool ve_engine::checkValidationLayerSupport()
  {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName: validationLayers)
    {
      bool layerFound = false;

      for (const auto &layerProperties: availableLayers)
      {
        if (strcmp(layerName, layerProperties.layerName) == 0)
        {
          layerFound = true;
          break;
        }
      }

      if (!layerFound)
      {
        return false;
      }
    }

    return true;
  }

  std::vector<const char *> ve_engine::getRequiredExtensions()
  {
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers)
    {
      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
  }

  void ve_engine::setupDebugMessenger()
  {
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(vkInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to set up debug messenger!");
    }
  }
}