cmake_minimum_required(VERSION 3.20)

project(graviballs CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PCH_FILE_H ${PROJECT_ROOT_DIR}/include/pch.h)

set(PROJECT_INCLUDE_DIR
    "${PROJECT_ROOT_DIR}/include"
    "${PROJECT_ROOT_DIR}/include/entities"
    "${PROJECT_ROOT_DIR}/include/utils"
    "${PROJECT_ROOT_DIR}/include/ui"
    "${PROJECT_ROOT_DIR}/include/systems"
    "${PROJECT_ROOT_DIR}/include/components"
    "${PROJECT_ROOT_DIR}/include/ecs"
)

set(LOG_FILES_DIR ${CMAKE_CURRENT_BINARY_DIR}/logs)
set(EXECUTABLE_OUTPUT_DIR ${PROJECT_ROOT_DIR}/out)

file(MAKE_DIRECTORY ${LOG_FILES_DIR})
file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_DIR})

set(MAIN_CPP_FILE ${PROJECT_ROOT_DIR}/src/main.cpp)
set(PCH_FILE_CPP ${PROJECT_ROOT_DIR}/src/pch.cpp)

set(COMMON_SOURCE_FILES
    ${MAIN_CPP_FILE}
    ${PCH_FILE_CPP}
    "${PROJECT_ROOT_DIR}/src/entities/ball.cpp"
    "${PROJECT_ROOT_DIR}/src/utils/shadercompile.cpp"
    "${PROJECT_ROOT_DIR}/src/utils/mouse.cpp"
    "${PROJECT_ROOT_DIR}/src/ui/ui_main.cpp"
    "${PROJECT_ROOT_DIR}/src/systems/renderer.cpp"
    "${PROJECT_ROOT_DIR}/src/systems/physics.cpp"
    "${PROJECT_ROOT_DIR}/src/ecs/ecs.cpp"
)

# ------------- COMPILES ---------------

# Add executable target
add_executable(graviballs)

target_sources(graviballs
    PRIVATE
        ${COMMON_SOURCE_FILES}
)

# Packages dependencies for project

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(imgui CONFIG REQUIRED)

target_link_libraries(graviballs
    PRIVATE
        glad::glad
        glm::glm
        fmt::fmt
        glfw
        spdlog::spdlog
        imgui::imgui
)

# Set output directory for the executable
set_target_properties(graviballs PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_DIR}
    DEBUG_POSTFIX "_d"
)

# Include directories
target_include_directories(graviballs PRIVATE
    ${PROJECT_INCLUDE_DIR}
    ${Stb_INCLUDE_DIR}
)

# Compile features for C++23
target_compile_features(graviballs PRIVATE cxx_std_23)

target_compile_options(graviballs PRIVATE ${CUSTOM_COMPILE_FLAGS})

# --- Address Sanitizer ---
# target_compile_options(graviballs PRIVATE -fsanitize=address)
# target_link_options(graviballs PRIVATE -fsanitize=address)

# Precompiled Headers
target_precompile_headers(graviballs PRIVATE include/pch.h)

message(STATUS "    To build: Ensure a C/C++ compiler is installed.")
message(STATUS "    Ensure VCPKG_ROOT is set in environment path.")
message(STATUS "    IMPORTANT: If build fails, check log files in: ${LOG_FILES_DIR}")
message(STATUS "    Final executable expected at: ${OUTPUT_EXE_FILE}")

# --- Informational Messages ---
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "  Build Directory: ${CMAKE_CURRENT_BINARY_DIR}")