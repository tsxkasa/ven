#pragma once

#include "pch.h"

namespace ven {
class Pipeline {
public:
  Pipeline(const std::string &vert_path, const std::string &frag_path);

private:
  static std::vector<char> readShaders(const std::string &path);

  void createPipeline(const std::string &vert_path, const std::string &frag_path);
};
} // namespace ven