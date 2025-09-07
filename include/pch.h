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

#include <fmt/fmt.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb_image.h>
#ifdef WIN32
#include "Windows.h"
#elif (UNIX)
#include "unistd.h"
#endif