#pragma once

#include "ve_configuration.hpp"
#include <vector>

namespace ve
{
	struct SwapChainSupportDetails {

    VkSurfaceCapabilitiesKHR capabilities;

    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    };
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);


	class ve_swapChain {
	public:
		ve_swapChain(ve_configuration& config);

		void createSwapChain();

	private:
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void createImageViews();
		void createRenderPass();


		ve_configuration& config;

	};


}