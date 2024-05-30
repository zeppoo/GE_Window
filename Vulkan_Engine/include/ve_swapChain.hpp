#pragma once

#include "ve_globals.hpp"
#include "ve_utilities.hpp"

namespace ve
{
    void createSwapChain();

    void createImageViews();

    void createRenderPass();

    void createFramebuffers();

    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

    static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
}