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
			setupDebugMessenger();
		}
	}

	ve_engine::~ve_engine()
	{
		for (size_t i = 0; i < config.MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(config.logicDevice, config.renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(config.logicDevice, config.imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(config.logicDevice, config.inFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(config.logicDevice, config.commandPool, nullptr);

		for (auto framebuffer : config.swapChainFramebuffers) {
			vkDestroyFramebuffer(config.logicDevice, framebuffer, nullptr);
		}

		vkDestroyPipeline(config.logicDevice, config.graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(config.logicDevice, config.pipelineLayout, nullptr);
		vkDestroyRenderPass(config.logicDevice, config.renderPass, nullptr);

		for (auto imageView : config.swapChainImageViews) {
			vkDestroyImageView(config.logicDevice, imageView, nullptr);
		}

		vkDestroySwapchainKHR(config.logicDevice, config.swapChain, nullptr);
		vkDestroyDevice(config.logicDevice, nullptr);

		if (config.enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(config.vkInstance, config.debugMessenger, nullptr);
		}

		vkDestroySurfaceKHR(config.vkInstance, config.surface, nullptr);
		vkDestroyInstance(config.vkInstance, nullptr);

		glfwDestroyWindow(config.window);

		glfwTerminate();
	}

	void ve_engine::initEngine(ve_engine* vkEngine)
	{
		// Create Window
		ve_window window{ config };

		// Gets GPU and creates Logical Device
        ve_device device{config};
        device.pickPhysicalDevice();// first
        device.createLogicalDevice();// second

		// Instantiate Swapchain
		ve_swapChain swap_chain{config};
		swap_chain.createSwapChain();
		swap_chain.createImageViews();
		swap_chain.createRenderPass();

		auto pipelineInfo = ve_pipeline::createPipelineConfiguration(config);
		vulkanPipeline = std::make_unique<ve_pipeline>(config, pipelineInfo);

		swap_chain.createFramebuffers();
		device.createCommandPool();
		device.createCommandBuffer();
		device.createSyncObjects();

		window.windowLoop(device);
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
		if (config.enableValidationLayers && !checkValidationLayerSupport()) {
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
		if (config.enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(config.validationLayers.size());
			createInfo.ppEnabledLayerNames = config.validationLayers.data();

			populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
		} else {
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&createInfo, nullptr, &config.vkInstance) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create Vulkan instance");
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