#pragma once

#include "ve_configuration.hpp"
#include "ve_debugMessenger.hpp"
#include "ve_window.hpp"
#include "ve_device.hpp"
#include "ve_swapChain.hpp"
#include "ve_pipeline.hpp"
#include <cstring>
#include <memory>


namespace ve
{
	class ve_engine
	{
	public:
		ve_engine(ve_configuration& config);
		~ve_engine();

		void initEngine(ve_engine* vkEngine);

	private:

		void initGLFW();
		void initVULKAN();
		bool checkValidationLayerSupport();
		std::vector<const char*> getRequiredExtensions();
		void setupDebugMessenger();

		std::unique_ptr<ve_pipeline> vulkanPipeline;
		ve_configuration& config;
	};

}