#include "ven_pipeline.h"
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

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
  return config_info;
}