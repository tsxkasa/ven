#pragma once
#if defined(_WIN32) || defined(CYGWIN)
#  define NOMINMAX
#  include <Windows.h>
#elif defined(__linux__)
#  include <unistd.h>
#endif
#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <chrono>
#include <expected>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>


#include <fmt/core.h>
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <spdlog/common.h>
#include <stb_image.h>
#include <vulkan/vulkan.h>

#include "color.h"
#include "mouse.h"

using Entity = std::uint32_t;