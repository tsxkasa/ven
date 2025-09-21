#pragma once

#include "pch.h"
#include "ven_device.h"
#include "ven_model.h"

namespace ven {

struct PipelineConfigInfo {
  PipelineConfigInfo() = default;
  PipelineConfigInfo(const PipelineConfigInfo&) = delete;
  PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAsmInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  std::vector<VkDynamicState> dynamicStateEnables;
  VkPipelineDynamicStateCreateInfo dynamicStateInfo;
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
  Pipeline operator=(const Pipeline&) = delete;

  void bind(VkCommandBuffer cmdBuffer);
  static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

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