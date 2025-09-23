#include "ven_model.h"
#include <cstddef>
#include <cstring>
#include <vector>
#include <vulkan/vulkan_core.h>

ven::Model::Model(ven::Device& device, const std::vector<Vertex>& vertices)
    : venDevice(device) {
  createVertexBuffers(vertices);
}

ven::Model::~Model() {
  vkDestroyBuffer(venDevice.device(), vertexBuffer, nullptr);
  vkFreeMemory(venDevice.device(), vertexBufferMemory, nullptr);
}

void ven::Model::createVertexBuffers(const std::vector<Vertex>& vertices) {
  vertexCount = static_cast<uint32_t>(vertices.size());
  assert(vertexCount >= 3 && "Vertex Count must be atleast 3");
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

  venDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         vertexBuffer, vertexBufferMemory);

  void* data;
  vkMapMemory(venDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
  vkUnmapMemory(venDevice.device(), vertexBufferMemory);
}

void ven::Model::draw(VkCommandBuffer cmdBuffer) {
  vkCmdDraw(cmdBuffer, vertexCount, 1, 0, 0);
}

void ven::Model::bind(VkCommandBuffer cmdBuffer) {
  VkBuffer buffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(cmdBuffer, 0, 1, buffers, offsets);
}

std::vector<VkVertexInputBindingDescription>
ven::Model::Vertex::getBindingDescriptions() {
  std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
  bindingDescriptions[0].binding = 0;
  bindingDescriptions[0].stride = sizeof(ven::Model::Vertex);
  bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription>
ven::Model::Vertex::getAttributeDescriptions() {
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[0].offset = 0;

  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(Vertex, color);
  return attributeDescriptions;
}