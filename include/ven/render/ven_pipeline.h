#pragma once

#include "pch.h"
#include "ven_device.h"
#include <vector>
#include <vulkan/vulkan_core.h>

namespace ven {

struct PipelineConfigInfo {
  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAsmInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

class Pipeline {
public:
  Pipeline(ven::Device& device, const std::string& vertPath,
           const std::string& fragPath, const ven::PipelineConfigInfo& config);

  ~Pipeline();

  Pipeline(const Pipeline&) = delete;
  void operator=(const Pipeline&) = delete;

  void bind(VkCommandBuffer cmdBuffer);
  static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo,
                                        uint32_t width, uint32_t height);

private:
  ven::Device& venDevice;
  VkPipeline graphicsPipeline;
  VkShaderModule vertModule;
  VkShaderModule fragModule;

  static std::vector<char> readShaders(const std::string& path);

  void createPipeline(const std::string& vertPath, const std::string& fragPath,
                      const PipelineConfigInfo& config);

  void createShaderModule(const std::vector<char>& code, VkShaderModule* mod);
};
} // namespace ven