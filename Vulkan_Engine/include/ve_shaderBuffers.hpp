#pragma once

#include "ve_globals.hpp"
#include "ve_utilities.hpp"

namespace ve {

  struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
      VkVertexInputBindingDescription bindingDescription{};
      bindingDescription.binding = 0;
      bindingDescription.stride = sizeof(Vertex);
      bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

      return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
      std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
      attributeDescriptions[0].binding = 0;
      attributeDescriptions[0].location = 0;
      attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
      attributeDescriptions[0].offset = offsetof(Vertex, pos);

      attributeDescriptions[1].binding = 0;
      attributeDescriptions[1].location = 1;
      attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
      attributeDescriptions[1].offset = offsetof(Vertex, color);

      return attributeDescriptions;
    }
  };

  struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
  };

  void createVertexBuffer();

  void createIndexBuffer();

  void createUniformBuffers();

  static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

  static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

  static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

  extern const std::vector<uint16_t> indices;
  extern const std::vector<Vertex> vertices;
}
