#include "ven_pipeline.h"

ven::Pipeline::Pipeline(const std::string &vert_path,
                        const std::string &frag_path) {
  createPipeline(vert_path, frag_path);
}

std::vector<char> ven::Pipeline::readShaders(const std::string &path) {
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

void ven::Pipeline::createPipeline(const std::string &vert_path,
                                   const std::string &frag_path) {
  auto vertbin = readShaders(vert_path);
  auto fragbin = readShaders(frag_path);

  fmt::println("Vertex Shader Code Size: {:d}", vertbin.size());
  fmt::println("Fragment Shader Code Size: {:d}", fragbin.size());
}