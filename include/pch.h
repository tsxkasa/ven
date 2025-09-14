#pragma once
#include <cassert>
#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#if defined(_WIN32) || defined(CYGWIN)
#define NOMINMAX
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h>
#endif
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <map>
#include <memory>
#include <queue>
#include <stack>
#include <expected>

#include <fmt/format.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/common.h>
#ifdef WIN32
#include "Windows.h"
#elif (UNIX)
#include "unistd.h"
#endif

#include "color.h"

using Entity = std::uint32_t;