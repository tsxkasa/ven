#include "ven_pipeline.h"
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan_core.h>
#include <winuser.h>

ven::Pipeline::Pipeline(ven::Device& device, const std::string& vertPath,
                        const std::string& fragPath,
                        const ven::PipelineConfigInfo& config)
    : venDevice{device} {
  createPipeline(vertPath, fragPath, config);
}

std::vector<char> ven::Pipeline::readShaders(const std::string& path) {
  std::ifstream file{path, std::ios::ate | std::ios::binary};

  if (!file.is_open())
    throw std::runtime_error("Failed to open file " + path);

  std::size_t fSize = static_cast<std::size_t>(file.tellg());

  std::vector<char> buffer(fSize);

  file.seekg(0);
  file.read(buffer.data(), fSize);

  file.close();

  return buffer;
}

void ven::Pipeline::createPipeline(const std::string& vertPath,
                                   const std::string& fragPath,
                                   const ven::PipelineConfigInfo& config) {
  auto vertbin = readShaders(vertPath);
  auto fragbin = readShaders(fragPath);

  fmt::println("Vertex Shader Code Size: {:d}", vertbin.size());
  fmt::println("Fragment Shader Code Size: {:d}", fragbin.size());
}

void ven::Pipeline::createShaderModule(const std::vector<char>& code,
                                       VkShaderModule* mod) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  // reinterpret_cast because SPIR-V expects u32, only works on vec, invalid if
  // C-style array
  createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

  if (vkCreateShaderModule(venDevice.device(), &createInfo, nullptr, mod) !=
      VK_SUCCESS)
    throw std::runtime_error("Failed to create shader module.");
}

ven::PipelineConfigInfo ven::Pipeline::defaultConfigInfo(uint32_t width,
                                                         uint32_t height) {
  PipelineConfigInfo configInfo{};

  configInfo.inputAsmInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  configInfo.inputAsmInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  configInfo.inputAsmInfo.primitiveRestartEnable = VK_FALSE;

  configInfo.viewport.x = 0.0f;
  configInfo.viewport.y = 0.0f;
  configInfo.viewport.width = static_cast<float>(width);
  configInfo.viewport.height = static_cast<float>(height);
  configInfo.viewport.minDepth = 0.0f;
  configInfo.viewport.maxDepth = 1.0f;

  configInfo.scissor.offset = {0, 0};
  configInfo.scissor.extent = {width, height};

  configInfo.viewportInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  configInfo.viewportInfo.viewportCount = 1;
  configInfo.viewportInfo.pViewports = &configInfo.viewport;
  configInfo.viewportInfo.scissorCount = 1;
  configInfo.viewportInfo.pScissors = &configInfo.scissor;

  configInfo.rasterizationInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
  configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
  configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
  configInfo.rasterizationInfo.lineWidth = 1.0f;
  configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
  configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
  configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
  configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
  configInfo.rasterizationInfo.depthBiasClamp = 0.0f;
  configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

  configInfo.multisampleInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
  configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  configInfo.multisampleInfo.minSampleShading = 1.0f;
  configInfo.multisampleInfo.pSampleMask = nullptr;
  configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
  configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;

  configInfo.colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
  configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
  configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

  configInfo.colorBlendInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
  configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
  configInfo.colorBlendInfo.attachmentCount = 1;
  configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
  configInfo.colorBlendInfo.blendConstants[0] = 0.0f;
  configInfo.colorBlendInfo.blendConstants[1] = 0.0f;
  configInfo.colorBlendInfo.blendConstants[2] = 0.0f;
  configInfo.colorBlendInfo.blendConstants[3] = 0.0f;

  configInfo.depthStencilInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
  configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
  configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
  configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
  configInfo.depthStencilInfo.minDepthBounds = 0.0f;
  configInfo.depthStencilInfo.maxDepthBounds = 1.0f;
  configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
  configInfo.depthStencilInfo.front = {};
  configInfo.depthStencilInfo.back = {};
  return configInfo;
}