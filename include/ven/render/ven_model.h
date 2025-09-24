#pragma once

#include "pch.h"
#include "ven_device.h"

namespace ven {
class Model {
public:
  struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    static std::vector<VkVertexInputBindingDescription>
    getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescriptions();
  };

  struct Builder {
    std::vector<ven::Model::Vertex> vertices{};
    std::vector<uint32_t> indices{};
  };

private:
  ven::Device& venDevice;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  uint32_t vertexCount;

  bool hasIndexBuffer = false;
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;
  uint32_t indexCount;

  void createVertexBuffers(const std::vector<Vertex>& vertices);
  void createIndexBuffer(const std::vector<uint32_t>& indices);

public:
  Model(ven::Device& device, const ven::Model::Builder& builder);
  ~Model();

  Model(const Model&) = delete;
  Model& operator=(const Model&) = delete;

  void bind(VkCommandBuffer cmdBuffer);
  void draw(VkCommandBuffer cmdBuffer);
};
} // namespace ven