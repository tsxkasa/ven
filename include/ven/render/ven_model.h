#pragma once

#include "pch.h"
#include "ven_buffer.h"
#include "ven_device.h"
#include "ven_utils.h"

namespace ven {
class Model {
public:
  struct Vertex {
    glm::vec3 position{};
    glm::vec3 color{};
    glm::vec3 normal{};
    glm::vec2 uv{};
    static std::vector<VkVertexInputBindingDescription>
    getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescriptions();

    bool operator==(const Vertex& o) const {
      return position == o.position && color == o.color && normal == o.normal &&
             uv == o.uv;
    }
  };

  struct Builder {
    std::vector<ven::Model::Vertex> vertices{};
    std::vector<uint32_t> indices{};

    /*
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> dst(
        0, std::numeric_limits<unsigned int>::max());

    float min_float = 0.0f;
    float max_float = 1.0f;
    std::array<float, 3> a;
    for (int i = 0; i < 3; i++) {
      unsigned int random_int = dst(gen);
      float random_float =
          min_float +
          (static_cast<float>(random_int) /
           static_cast<float>(std::numeric_limits<unsigned int>::max())) *
              (max_float - min_float);
      a[i] = random_float;
    }

    vertex.color = glm::vec3{a[0], a[1], a[2]};
    */
    void LoadModel(const std::string& filepath);
  };

private:
  ven::Device& venDevice;
  std::unique_ptr<ven::Buffer> vertexBuffer;
  uint32_t vertexCount;

  bool hasIndexBuffer = false;
  std::unique_ptr<ven::Buffer> indexBuffer;
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
  static std::unique_ptr<ven::Model> createModel(ven::Device& device,
                                                 const std::string& filepath);
};
} // namespace ven