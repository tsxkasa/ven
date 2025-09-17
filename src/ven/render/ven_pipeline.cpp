#include "ven_pipeline.h"
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan_core.h>
#include <winuser.h>

ven::Pipeline::Pipeline(ven::Device& device, const std::string& vert_path,
                        const std::string& frag_path,
                        const ven::PipelineConfigInfo& config)
    : ven_device{device} {
  createPipeline(vert_path, frag_path, config);
}

std::vector<char> ven::Pipeline::readShaders(const std::string& path) {
  std::ifstream file{path, std::ios::ate | std::ios::binary};

  if (!file.is_open())
    throw std::runtime_error("Failed to open file " + path);

  std::size_t fsize = static_cast<std::size_t>(file.tellg());

  std::vector<char> buffer(fsize);

  file.seekg(0);
  file.read(buffer.data(), fsize);

  file.close();

  return buffer;
}

void ven::Pipeline::createPipeline(const std::string& vert_path,
                                   const std::string& frag_path,
                                   const ven::PipelineConfigInfo& config) {
  auto vertbin = readShaders(vert_path);
  auto fragbin = readShaders(frag_path);

  fmt::println("Vertex Shader Code Size: {:d}", vertbin.size());
  fmt::println("Fragment Shader Code Size: {:d}", fragbin.size());
}

void ven::Pipeline::createShaderModule(const std::vector<char>& code,
                                       VkShaderModule* mod) {
  VkShaderModuleCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = code.size();
  // reinterpret_cast because SPIR-V expects u32, only works on vec, invalid if
  // C-style array
  create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

  if (vkCreateShaderModule(ven_device.device(), &create_info, nullptr, mod) !=
      VK_SUCCESS)
    throw std::runtime_error("Failed to create shader module.");
}

ven::PipelineConfigInfo ven::Pipeline::defaultConfigInfo(uint32_t width,
                                                         uint32_t height) {
  PipelineConfigInfo config_info{};

  config_info.input_asm_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  config_info.input_asm_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  config_info.input_asm_info.primitiveRestartEnable = VK_FALSE;

  config_info.viewport.x = 0.0f;
  config_info.viewport.y = 0.0f;
  config_info.viewport.width = static_cast<float>(width);
  config_info.viewport.height = static_cast<float>(height);
  config_info.viewport.minDepth = 0.0f;
  config_info.viewport.maxDepth = 1.0f;

  config_info.scissor.offset = {0, 0};
  config_info.scissor.extent = {width, height};

  config_info.viewport_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  config_info.viewport_info.viewportCount = 1;
  config_info.viewport_info.pViewports = &config_info.viewport;
  config_info.viewport_info.scissorCount = 1;
  config_info.viewport_info.pScissors = &config_info.scissor;

  config_info.rasterization_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  config_info.rasterization_info.depthClampEnable = VK_FALSE;
  config_info.rasterization_info.rasterizerDiscardEnable = VK_FALSE;
  config_info.rasterization_info.polygonMode = VK_POLYGON_MODE_FILL;
  config_info.rasterization_info.lineWidth = 1.0f;
  config_info.rasterization_info.cullMode = VK_CULL_MODE_NONE;
  config_info.rasterization_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
  config_info.rasterization_info.depthBiasEnable = VK_FALSE;
  config_info.rasterization_info.depthBiasConstantFactor = 0.0f;
  config_info.rasterization_info.depthBiasClamp = 0.0f;
  config_info.rasterization_info.depthBiasSlopeFactor = 0.0f;

  config_info.multisample_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  config_info.multisample_info.sampleShadingEnable = VK_FALSE;
  config_info.multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  config_info.multisample_info.minSampleShading = 1.0f;
  config_info.multisample_info.pSampleMask = nullptr;
  config_info.multisample_info.alphaToCoverageEnable = VK_FALSE;
  config_info.multisample_info.alphaToOneEnable = VK_FALSE;

  config_info.colorblend_attachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  config_info.colorblend_attachment.blendEnable = VK_FALSE;
  config_info.colorblend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  config_info.colorblend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  config_info.colorblend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
  config_info.colorblend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  config_info.colorblend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  config_info.colorblend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

  config_info.colorblend_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  config_info.colorblend_info.logicOpEnable = VK_FALSE;
  config_info.colorblend_info.logicOp = VK_LOGIC_OP_COPY;
  config_info.colorblend_info.attachmentCount = 1;
  config_info.colorblend_info.pAttachments = &config_info.colorblend_attachment;
  config_info.colorblend_info.blendConstants[0] = 0.0f;
  config_info.colorblend_info.blendConstants[1] = 0.0f;
  config_info.colorblend_info.blendConstants[2] = 0.0f;
  config_info.colorblend_info.blendConstants[3] = 0.0f;

  config_info.depth_stencil_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  config_info.depth_stencil_info.depthTestEnable = VK_TRUE;
  config_info.depth_stencil_info.depthWriteEnable = VK_TRUE;
  config_info.depth_stencil_info.depthCompareOp = VK_COMPARE_OP_LESS;
  config_info.depth_stencil_info.depthBoundsTestEnable = VK_FALSE;
  config_info.depth_stencil_info.minDepthBounds = 0.0f;
  config_info.depth_stencil_info.maxDepthBounds = 1.0f;
  config_info.depth_stencil_info.stencilTestEnable = VK_FALSE;
  config_info.depth_stencil_info.front = {};
  config_info.depth_stencil_info.back = {};
  return config_info;
}