#pragma once

#include "main.hpp"
#include <vector>

namespace ve
{
	class ve_engine
	{
	public:
		ve_engine(const int w, const int h, const std::string name);
		~ve_engine();

		void initEngine(ve_engine* vkEngine);

	private:

		void initGLFW();
		void initVULKAN();
		bool checkValidationLayerSupport();
		std::vector<const char*> getRequiredExtensions();
		void setupDebugMessenger();

		// Window Data
		const int WIN_WIDTH;
		const int WIN_HEIGHT;
		const std::string NAME;

		#ifdef NDEBUG // NDEBUG = No Debug
			const bool enableValidationLayers = false;
		#else
			const bool enableValidationLayers = true;
		#endif

		VkDebugUtilsMessengerEXT debugMessenger;// Vulkan Debug Messenger Extension instance

		VkInstance vkInstance;// Vk Api instance
		const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
		VkSurfaceKHR surface;// Vk Rendering Surface
		VkPhysicalDevice physicDevice = VK_NULL_HANDLE;// Physical GPU Reference
		VkDevice device;// Virtual Vk GPU reference

		// Vulkan Queues instances
		VkQueue graphicsQueue;// Queue for rendering
		VkQueue presentQueue;// Queue for presenting rendered frames
	};

}