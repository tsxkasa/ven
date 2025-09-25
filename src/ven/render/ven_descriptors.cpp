#include "ven_descriptors.h"

// *************** Descriptor Set Layout Builder *********************

ven::DescriptorSetLayout::Builder&
ven::DescriptorSetLayout::Builder::addBinding(uint32_t binding,
                                              VkDescriptorType descriptorType,
                                              VkShaderStageFlags stageFlags,
                                              uint32_t count) {
  assert(bindings.count(binding) == 0 && "Binding already in use");
  VkDescriptorSetLayoutBinding layoutBinding{};
  layoutBinding.binding = binding;
  layoutBinding.descriptorType = descriptorType;
  layoutBinding.descriptorCount = count;
  layoutBinding.stageFlags = stageFlags;
  bindings[binding] = layoutBinding;
  return *this;
}

std::unique_ptr<ven::DescriptorSetLayout>
ven::DescriptorSetLayout::Builder::build() const {
  return std::make_unique<ven::DescriptorSetLayout>(venDevice, bindings);
}

// *************** Descriptor Set Layout *********************

ven::DescriptorSetLayout::DescriptorSetLayout(
    ven::Device& venDevice,
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
    : venDevice{venDevice}
    , bindings{bindings} {
  std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
  for (auto kv : bindings) {
    setLayoutBindings.push_back(kv.second);
  }

  VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
  descriptorSetLayoutInfo.sType =
      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  descriptorSetLayoutInfo.bindingCount =
      static_cast<uint32_t>(setLayoutBindings.size());
  descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

  if (vkCreateDescriptorSetLayout(venDevice.device(), &descriptorSetLayoutInfo,
                                  nullptr,
                                  &descriptorSetLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor set layout!");
  }
}

ven::DescriptorSetLayout::~DescriptorSetLayout() {
  vkDestroyDescriptorSetLayout(venDevice.device(), descriptorSetLayout,
                               nullptr);
}

// *************** Descriptor Pool Builder *********************

ven::DescriptorPool::Builder&
ven::DescriptorPool::Builder::addPoolSize(VkDescriptorType descriptorType,
                                          uint32_t count) {
  poolSizes.push_back({descriptorType, count});
  return *this;
}

ven::DescriptorPool::Builder&
ven::DescriptorPool::Builder::setPoolFlags(VkDescriptorPoolCreateFlags flags) {
  poolFlags = flags;
  return *this;
}
ven::DescriptorPool::Builder&
ven::DescriptorPool::Builder::setMaxSets(uint32_t count) {
  maxSets = count;
  return *this;
}

std::unique_ptr<ven::DescriptorPool>
ven::DescriptorPool::Builder::build() const {
  return std::make_unique<ven::DescriptorPool>(venDevice, maxSets, poolFlags,
                                               poolSizes);
}

// *************** Descriptor Pool *********************

ven::DescriptorPool::DescriptorPool(
    ven::Device& venDevice, uint32_t maxSets,
    VkDescriptorPoolCreateFlags poolFlags,
    const std::vector<VkDescriptorPoolSize>& poolSizes)
    : venDevice{venDevice} {
  VkDescriptorPoolCreateInfo descriptorPoolInfo{};
  descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  descriptorPoolInfo.pPoolSizes = poolSizes.data();
  descriptorPoolInfo.maxSets = maxSets;
  descriptorPoolInfo.flags = poolFlags;

  if (vkCreateDescriptorPool(venDevice.device(), &descriptorPoolInfo, nullptr,
                             &descriptorPool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor pool!");
  }
}

ven::DescriptorPool::~DescriptorPool() {
  vkDestroyDescriptorPool(venDevice.device(), descriptorPool, nullptr);
}

bool ven::DescriptorPool::allocateDescriptor(
    const VkDescriptorSetLayout descriptorSetLayout,
    VkDescriptorSet& descriptor) const {
  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = descriptorPool;
  allocInfo.pSetLayouts = &descriptorSetLayout;
  allocInfo.descriptorSetCount = 1;

  if (vkAllocateDescriptorSets(venDevice.device(), &allocInfo, &descriptor) !=
      VK_SUCCESS) {
    return false;
  }
  return true;
}

void ven::DescriptorPool::freeDescriptors(
    std::vector<VkDescriptorSet>& descriptors) const {
  vkFreeDescriptorSets(venDevice.device(), descriptorPool,
                       static_cast<uint32_t>(descriptors.size()),
                       descriptors.data());
}

void ven::DescriptorPool::resetPool() {
  vkResetDescriptorPool(venDevice.device(), descriptorPool, 0);
}

// *************** Descriptor Writer *********************

ven::DescriptorWriter::DescriptorWriter(ven::DescriptorSetLayout& setLayout,
                                        ven::DescriptorPool& pool)
    : setLayout{setLayout}
    , pool{pool} {}

ven::DescriptorWriter&
ven::DescriptorWriter::writeBuffer(uint32_t binding,
                                   VkDescriptorBufferInfo* bufferInfo) {
  assert(setLayout.bindings.count(binding) == 1 &&
         "Layout does not contain specified binding");

  auto& bindingDescription = setLayout.bindings[binding];

  assert(bindingDescription.descriptorCount == 1 &&
         "Binding single descriptor info, but binding expects multiple");

  VkWriteDescriptorSet write{};
  write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write.descriptorType = bindingDescription.descriptorType;
  write.dstBinding = binding;
  write.pBufferInfo = bufferInfo;
  write.descriptorCount = 1;

  writes.push_back(write);
  return *this;
}

ven::DescriptorWriter&
ven::DescriptorWriter::writeImage(uint32_t binding,
                                  VkDescriptorImageInfo* imageInfo) {
  assert(setLayout.bindings.count(binding) == 1 &&
         "Layout does not contain specified binding");

  auto& bindingDescription = setLayout.bindings[binding];

  assert(bindingDescription.descriptorCount == 1 &&
         "Binding single descriptor info, but binding expects multiple");

  VkWriteDescriptorSet write{};
  write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write.descriptorType = bindingDescription.descriptorType;
  write.dstBinding = binding;
  write.pImageInfo = imageInfo;
  write.descriptorCount = 1;

  writes.push_back(write);
  return *this;
}

bool ven::DescriptorWriter::build(VkDescriptorSet& set) {
  bool success =
      pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
  if (!success) {
    return false;
  }
  overwrite(set);
  return true;
}

void ven::DescriptorWriter::overwrite(VkDescriptorSet& set) {
  for (auto& write : writes) {
    write.dstSet = set;
  }
  vkUpdateDescriptorSets(pool.venDevice.device(), writes.size(), writes.data(),
                         0, nullptr);
}