#include "ve_engine.hpp"
#include "ve_debugMessenger.hpp"
#include "ve_window.hpp"
#include "ve_physicDevice.hpp"
#include "ve_logicDevice.hpp"

namespace ve
{
	ve_engine::ve_engine(const int w, const int h, const std::string name) : WIN_WIDTH{ w }, WIN_HEIGHT{ h }, NAME{ name }
	{
		initGLFW();
		try {
			initVULKAN();
		}
		catch (const std::exception&) {
			if (vkInstance != VK_NULL_HANDLE) {
				vkDestroyInstance(vkInstance, nullptr);
			}
			glfwTerminate();
			throw;
		}
		if(enableValidationLayers)
		{
			checkValidationLayerSupport();
			if (enableValidationLayers && !checkValidationLayerSupport()) {
			throw std::runtime_error("validation layers requested, but not available!");
			}

			setupDebugMessenger();
		}
	}

	ve_engine::~ve_engine()
	{
		if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(vkInstance, debugMessenger, nullptr);
		}

		vkDestroyDevice(device, nullptr);
		vkDestroySurfaceKHR(vkInstance, surface, nullptr);

		vkDestroyInstance(vkInstance, nullptr);
	}

	void ve_engine::initEngine(ve_engine* vkEngine)
	{
		// Create Window
		ve_window window{ WIN_WIDTH, WIN_HEIGHT, NAME , &vkInstance, &surface };

		// Get GPU
		ve_physicDevice physicalDevice{};
		physicalDevice.pickPhysicalDevice(&vkInstance, &physicDevice, &surface);

		// Create Logical Device
		ve_logicDevice logicalDevice{};
		logicalDevice.createLogicalDevice(&device, &graphicsQueue, &presentQueue, &physicDevice, &enableValidationLayers, &validationLayers, &surface);

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

		vkCreateInstance(&createInfo, nullptr, &vkInstance);

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();

			populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
		} 
		else {
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&createInfo, nullptr, &vkInstance) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create Vulkan instance");
		}
	}

	bool ve_engine::checkValidationLayerSupport() {

    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
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

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

	void ve_engine::setupDebugMessenger() 
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(vkInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
		}
	}
}