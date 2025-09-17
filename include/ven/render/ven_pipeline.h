#pragma once

#include "pch.h"
#include "ven_device.h"
#include <vector>
#include <vulkan/vulkan_core.h>

namespace ven {

struct PipelineConfigInfo {
  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineViewportStateCreateInfo viewport_info;
  VkPipelineInputAssemblyStateCreateInfo input_asm_info;
  VkPipelineRasterizationStateCreateInfo rasterization_info;
  VkPipelineMultisampleStateCreateInfo multisample_info;
  VkPipelineColorBlendAttachmentState colorblend_attachment;
  VkPipelineColorBlendStateCreateInfo colorblend_info;
  VkPipelineDepthStencilStateCreateInfo depth_stencil_info;
  VkPipelineLayout pipeline_layout = nullptr;
  VkRenderPass render_pass = nullptr;
  uint32_t subpass = 0;
};

class Pipeline {
public:
  Pipeline(ven::Device& device, const std::string& vert_path,
           const std::string& frag_path, const ven::PipelineConfigInfo& config);

  ~Pipeline() {};

  Pipeline(const Pipeline&) = delete;
  void operator=(const Pipeline&) = delete;

  static ven::PipelineConfigInfo defaultConfigInfo(uint32_t width,
                                                   uint32_t height);

private:
  ven::Device& ven_device;
  VkPipeline graphics_pipeline;
  VkShaderModule vert_module;
  VkShaderModule frag_module;

  static std::vector<char> readShaders(const std::string& path);

  void createPipeline(const std::string& vert_path,
                      const std::string& frag_path,
                      const PipelineConfigInfo& config);

  void createShaderModule(const std::vector<char>& code, VkShaderModule* mod);
};
} // namespace ven