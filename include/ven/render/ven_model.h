#pragma once

#include "pch.h"
#include "ven_device.h"
#include <glm/fwd.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace ven {
class Model {
public:
  struct Vertex {
    glm::vec2 position;
    static std::vector<VkVertexInputBindingDescription>
    getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescriptions();
  };

private:
  ven::Device& venDevice;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  uint32_t vertexCount;

  void createVertexBuffers(const std::vector<Vertex>& vertices);

public:
  Model(ven::Device& device, const std::vector<Vertex>& vertices);
  ~Model();

  Model(const Model&) = delete;
  Model& operator=(const Model&) = delete;

  void bind(VkCommandBuffer cmdBuffer);
  void draw(VkCommandBuffer cmdBuffer);
};
} // namespace ven