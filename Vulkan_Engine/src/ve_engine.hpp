#pragma once

#include "main.hpp"

namespace ve
{
	class ve_engine
	{
	public:
		ve_engine(const int w, const int h, const std::string name);

		void initEngine();

	private:

		void initGLFW();
		void initVULKAN();

		const int WIN_WIDTH;
		const int WIN_HEIGHT;

		const std::string NAME;
		VkInstance vkInstance;
		VkSurfaceKHR surface;
		VkPhysicalDevice physicDevice = VK_NULL_HANDLE;
		VkDevice device;
		VkQueue graphicsQueue;
	};

}