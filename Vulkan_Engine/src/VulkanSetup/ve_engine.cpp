#include "ve_engine.hpp"

namespace ve
{
	ve_engine::ve_engine(ve_configuration& config) : config{config}
	{
		initGLFW();
		try {
			initVULKAN();
		}
		catch (const std::exception&) {
			if (config.vkInstance != VK_NULL_HANDLE) {
				vkDestroyInstance(config.vkInstance, nullptr);
			}
			glfwTerminate();
			throw;
		}
		if(config.enableValidationLayers)
		{
			checkValidationLayerSupport();
			if (config.enableValidationLayers && !checkValidationLayerSupport()) {
			throw std::runtime_error("validation layers requested, but not available!");
			}

			setupDebugMessenger();
		}
	}

	ve_engine::~ve_engine()
	{
		if (config.enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(config.vkInstance, config.debugMessenger, nullptr);
		}

		for (auto imageView : config.swapChainImageViews) {
        vkDestroyImageView(config.logicDevice, imageView, nullptr);
		}

		vkDestroyPipeline(config.logicDevice, config.graphicsPipeline, nullptr);
		vkDestroyRenderPass(config.logicDevice, config.renderPass, nullptr);
		vkDestroySwapchainKHR(config.logicDevice, config.swapChain, nullptr);
		vkDestroyDevice(config.logicDevice, nullptr);
		vkDestroySurfaceKHR(config.vkInstance, config.surface, nullptr);

		vkDestroyInstance(config.vkInstance, nullptr);
	}

	void ve_engine::initEngine(ve_engine* vkEngine)
	{
		// Create Window
		ve_window window{ config };

		// Get GPU
		ve_physicDevice physical_Device{config};
		physical_Device.pickPhysicalDevice();

		// Create Logical Device
		ve_logicDevice logical_Device{config};
		logical_Device.createLogicalDevice();

		// Instantiate Swapchain
		ve_swapChain swap_chain{config};
		swap_chain.createSwapChain();

		ve_renderPass render_pass{config};
		render_pass.createRenderPass();

		ve::ve_pipelineConfig pipelineConfig{config};
		ve::PipelineConfigInfo pipelineInfo = pipelineConfig.createPipelineConfiguration();

		ve::ve_pipeline vulkanPipeline{ config,  pipelineInfo};
		vulkanPipeline.createGraphicsPipeline(pipelineInfo);

		window.windowLoop();
	}

	void ve_engine::initGLFW()
	{
		if (!glfwInit()) {
			std::cerr << "Failed to initialize GLFW\n";
			return;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}

	void ve_engine::initVULKAN()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Vulkan";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_2;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// Enable GLFW required extensions
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		// Get Extensions e.g. (DebugMessenger Extension)
		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		if (vkCreateInstance(&createInfo, nullptr, &config.vkInstance) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create Vulkan instance");
		}

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (config.enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(config.validationLayers.size());
			createInfo.ppEnabledLayerNames = config.validationLayers.data();

			populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
		} 
		else {
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}
	}

	bool ve_engine::checkValidationLayerSupport() {

    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : config.validationLayers) {
    bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
	}

	std::vector<const char*> ve_engine::getRequiredExtensions() {

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (config.enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

	void ve_engine::setupDebugMessenger() 
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(config.vkInstance, &createInfo, nullptr, &config.debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
		}
	}
}