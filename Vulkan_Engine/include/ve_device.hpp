#pragma once

#include "ve_globals.hpp"

namespace ve {
  void pickPhysicalDevice();

  void createLogicalDevice();

  void createCommandPool();

  void createCommandBuffer();

  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

  void createSyncObjects();

  void drawFrame();

  static bool isDeviceSuitable(VkPhysicalDevice device);

  static bool checkDeviceExtensionSupport(VkPhysicalDevice physicDevice);

  static void updateUniformBuffer(uint32_t currentImage);
}
