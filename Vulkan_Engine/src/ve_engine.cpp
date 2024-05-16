#include "ve_engine.hpp"
#include "ve_window.hpp"

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

	}

	void ve_engine::initEngine()
	{
		//Create Pointers to engine
		VkInstance* pvkInstance = &vkInstance;
		VkSurfaceKHR* psurface = &surface;
		VkPhysicalDevice* pphysicDevice = &physicDevice;
		VkDevice* pdevice = &device;
		VkQueue* pgraphicsQueue = &graphicsQueue;

		ve_window window{ WIN_WIDTH, WIN_HEIGHT, NAME , pvkInstance, &surface };
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

		createInfo.enabledLayerCount = 0;

		vkCreateInstance(&createInfo, nullptr, &vkInstance);

		if (vkCreateInstance(&createInfo, nullptr, &vkInstance) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create Vulkan instance");
		}
	}

}