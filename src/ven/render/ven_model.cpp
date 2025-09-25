#include "ven_model.h"
#include <cstddef>
#include <cstring>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace std {
template <>
struct hash<ven::Model::Vertex> {
  size_t operator()(ven::Model::Vertex const& vertex) const {
    size_t seed = 0;
    ven::hashCombine(seed, vertex.position, vertex.color, vertex.normal,
                     vertex.uv);
    return seed;
  }
};
} // namespace std

ven::Model::Model(ven::Device& device, const ven::Model::Builder& builder)
    : venDevice(device) {
  createVertexBuffers(builder.vertices);
  createIndexBuffer(builder.indices);
}

ven::Model::~Model() {}

void ven::Model::createVertexBuffers(const std::vector<Vertex>& vertices) {
  vertexCount = static_cast<uint32_t>(vertices.size());
  assert(vertexCount >= 3 && "Vertex Count must be atleast 3");
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
  uint32_t vertexSize = sizeof(vertices[0]);

  ven::Buffer stagingBuffer{venDevice, vertexSize, vertexCount,
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

  stagingBuffer.map();
  stagingBuffer.writeToBuffer((void*)vertices.data());

  vertexBuffer = std::make_unique<ven::Buffer>(
      venDevice, vertexSize, vertexCount,
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  venDevice.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(),
                       bufferSize);
}

void ven::Model::createIndexBuffer(const std::vector<uint32_t>& indices) {
  indexCount = static_cast<uint32_t>(indices.size());
  hasIndexBuffer = indexCount > 0;
  if (!hasIndexBuffer)
    return;
  VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
  uint32_t indexSize = sizeof(indices[0]);

  ven::Buffer stagingBuffer{
      venDevice,
      indexSize,
      indexCount,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
  };

  stagingBuffer.map();
  stagingBuffer.writeToBuffer((void*)indices.data());

  indexBuffer = std::make_unique<ven::Buffer>(
      venDevice, indexSize, indexCount,
      VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  venDevice.copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(),
                       bufferSize);
}

void ven::Model::draw(VkCommandBuffer cmdBuffer) {
  if (hasIndexBuffer)
    vkCmdDrawIndexed(cmdBuffer, indexCount, 1, 0, 0, 0);
  else
    vkCmdDraw(cmdBuffer, vertexCount, 1, 0, 0);
}

void ven::Model::bind(VkCommandBuffer cmdBuffer) {
  VkBuffer buffers[] = {vertexBuffer->getBuffer()};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(cmdBuffer, 0, 1, buffers, offsets);

  if (hasIndexBuffer)
    vkCmdBindIndexBuffer(cmdBuffer, indexBuffer->getBuffer(), 0,
                         VK_INDEX_TYPE_UINT32);
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
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

  attributeDescriptions.push_back(
      {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)});
  attributeDescriptions.push_back(
      {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)});
  attributeDescriptions.push_back(
      {2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)});
  attributeDescriptions.push_back(
      {3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)});
  return attributeDescriptions;
}

std::unique_ptr<ven::Model>
ven::Model::createModel(ven::Device& device, const std::string& filepath) {
  ven::Model::Builder builder{};
  builder.LoadModel(filepath);
  fmt::println("Vertex count: {}", builder.vertices.size());
  return std::make_unique<ven::Model>(device, builder);
}

void ven::Model::Builder::LoadModel(const std::string& filepath) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> mats;
  std::string warn, err;

  if (!tinyobj::LoadObj(&attrib, &shapes, &mats, &warn, &err, filepath.c_str()))
    throw std::runtime_error(warn + err);

  vertices.clear();
  indices.clear();

  std::unordered_map<ven::Model::Vertex, uint32_t> uniqueVertices{};

  for (const auto& shape : shapes) {
    for (const auto& index : shape.mesh.indices) {
      ven::Model::Vertex vertex{};
      if (index.vertex_index >= 0) {
        vertex.position = {attrib.vertices[3 * index.vertex_index + 0],
                           attrib.vertices[3 * index.vertex_index + 1],
                           attrib.vertices[3 * index.vertex_index + 2]};

        vertex.color = {attrib.colors[3 * index.vertex_index + 0],
                        attrib.colors[3 * index.vertex_index + 1],
                        attrib.colors[3 * index.vertex_index + 2]};
      }
      if (index.normal_index >= 0) {
        vertex.normal = {
            attrib.normals[3 * index.normal_index + 0],
            attrib.normals[3 * index.normal_index + 1],
            attrib.normals[3 * index.normal_index + 2],
        };

        if (index.texcoord_index >= 0)
          vertex.uv = {attrib.texcoords[2 * index.texcoord_index + 0],
                       attrib.texcoords[2 * index.texcoord_index + 1]};
      }
      if (uniqueVertices.count(vertex) == 0) {
        uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
        vertices.push_back(vertex);
      }
      indices.push_back(uniqueVertices[vertex]);
    }
  }
}