#pragma once

#include "pch.h"
#include "ven_device.h"

namespace ven {

class DescriptorSetLayout {
public:
  class Builder {
  public:
    Builder(ven::Device& venDevice)
        : venDevice{venDevice} {}

    auto addBinding(uint32_t binding, VkDescriptorType descriptorType,
                    VkShaderStageFlags stageFlags, uint32_t count = 1)
        -> Builder&;
    auto build() const -> std::unique_ptr<ven::DescriptorSetLayout>;

  private:
    ven::Device& venDevice;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
  };

  DescriptorSetLayout(
      ven::Device& venDevice,
      const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>&
          bindings);
  ~DescriptorSetLayout();
  DescriptorSetLayout(const ven::DescriptorSetLayout&) = delete;
  auto operator=(const ven::DescriptorSetLayout&)
      -> DescriptorSetLayout& = delete;

  [[nodiscard]] auto getDescriptorSetLayout() const -> VkDescriptorSetLayout {
    return descriptorSetLayout;
  }

private:
  ven::Device& venDevice;
  VkDescriptorSetLayout descriptorSetLayout;
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

  friend class DescriptorWriter;
};

class DescriptorPool {
public:
  class Builder {
  public:
    Builder(ven::Device& venDevice)
        : venDevice{venDevice} {}

    auto addPoolSize(VkDescriptorType descriptorType, uint32_t count)
        -> Builder&;
    auto setPoolFlags(VkDescriptorPoolCreateFlags flags) -> Builder&;
    auto setMaxSets(uint32_t count) -> Builder&;
    [[nodiscard]] auto build() const -> std::unique_ptr<ven::DescriptorPool>;

  private:
    ven::Device& venDevice;
    std::vector<VkDescriptorPoolSize> poolSizes{};
    uint32_t maxSets = 1000;
    VkDescriptorPoolCreateFlags poolFlags = 0;
  };

  DescriptorPool(ven::Device& venDevice, uint32_t maxSets,
                 VkDescriptorPoolCreateFlags poolFlags,
                 const std::vector<VkDescriptorPoolSize>& poolSizes);
  ~DescriptorPool();
  DescriptorPool(const DescriptorPool&) = delete;
  auto operator=(const DescriptorPool&) -> DescriptorPool& = delete;

  auto allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout,
                          VkDescriptorSet& descriptor) const -> bool;

  void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

  void resetPool();

private:
  ven::Device& venDevice;
  VkDescriptorPool descriptorPool;

  friend class DescriptorWriter;
};

class DescriptorWriter {
public:
  DescriptorWriter(ven::DescriptorSetLayout& setLayout,
                   ven::DescriptorPool& pool);

  auto writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo)
      -> DescriptorWriter&;
  auto writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo)
      -> DescriptorWriter&;

  auto build(VkDescriptorSet& set) -> bool;
  void overwrite(VkDescriptorSet& set);

private:
  ven::DescriptorSetLayout& setLayout;
  ven::DescriptorPool& pool;
  std::vector<VkWriteDescriptorSet> writes;
};

} // namespace ven
