cmake_minimum_required(VERSION 3.20)

project(ven CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------- INCLUDE FILES --------------

set(PCH_FILE_H ${PROJECT_ROOT_DIR}/include/pch.h)

set(PROJECT_INCLUDE_DIR
    "${PROJECT_ROOT_DIR}/include"
    "${PROJECT_ROOT_DIR}/include/utils"
    "${PROJECT_ROOT_DIR}/include/ven/app"
    "${PROJECT_ROOT_DIR}/include/ven/window"
    "${PROJECT_ROOT_DIR}/include/ven/render"
    "${PROJECT_ROOT_DIR}/include/ven/ui"
)

# ------------- LOG FILES --------------

set(EXECUTABLE_OUTPUT_DIR ${PROJECT_ROOT_DIR}/out)

file(MAKE_DIRECTORY ${LOG_FILES_DIR})
file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_DIR})

# ------------- SOURCE FILES --------------

set(MAIN_CPP_FILE ${PROJECT_ROOT_DIR}/src/main.cpp)
set(PCH_FILE_CPP ${PROJECT_ROOT_DIR}/src/pch.cpp)

set(VEN_SYSTEM_SOURCES
    "${PROJECT_ROOT_DIR}/src/ven/app/ven_app.cpp"
    "${PROJECT_ROOT_DIR}/src/ven/window/ven_window.cpp"
    "${PROJECT_ROOT_DIR}/src/ven/ui/ven_ui.cpp"
    "${PROJECT_ROOT_DIR}/src/ven/render/ven_pipeline.cpp"
    "${PROJECT_ROOT_DIR}/src/ven/render/ven_device.cpp"
    "${PROJECT_ROOT_DIR}/src/ven/render/ven_swap_chain.cpp"
    "${PROJECT_ROOT_DIR}/src/ven/render/ven_model.cpp"
)

set(COMMON_SOURCE_FILES
    ${MAIN_CPP_FILE}
    ${PCH_FILE_CPP}
    ${VEN_SYSTEM_SOURCES}
)

# ------------- SHADER FILES --------------

set(SHADER_DIR
"${PROJECT_ROOT_DIR}/shaders"
)

set(SHADER_FILES
    ${SHADER_DIR}/shader.vert
    ${SHADER_DIR}/shader.frag
)

foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    set(OUTPUT_SHADER "${PROJECT_ROOT_DIR}/shaders/${SHADER_NAME}.spv")
    add_custom_command(
        OUTPUT ${OUTPUT_SHADER}
        COMMAND $ENV{VULKAN_SDK}/bin/glslc ${SHADER_FILE} -o ${OUTPUT_SHADER}
        DEPENDS ${SHADER_FILE}
        COMMENT "Compiling shader ${SHADER_NAME}"
        VERBATIM
    )
    add_custom_target(${SHADER_NAME}_compiled ALL DEPENDS ${OUTPUT_SHADER})
endforeach()

# ------------- COMPILES ---------------

# Add executable target
add_executable(ven)

target_sources(ven
    PRIVATE
        ${COMMON_SOURCE_FILES}
)

# Packages dependencies for project

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(imgui CONFIG REQUIRED)

target_link_libraries(ven
    PRIVATE
        Vulkan::Vulkan
        glm::glm
        fmt::fmt
        glfw
        spdlog::spdlog
        imgui::imgui
)

# Set output directory for the executable
set_target_properties(ven PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_DIR}
    DEBUG_POSTFIX "_d"
)

# Include directories
target_include_directories(ven PRIVATE
    ${PROJECT_INCLUDE_DIR}
    ${Stb_INCLUDE_DIR}
)

# Compile features for C++23
target_compile_features(ven PRIVATE cxx_std_23)

target_compile_options(ven PRIVATE ${CUSTOM_COMPILE_FLAGS})

# --- Address Sanitizer ---
# target_compile_options(ven PRIVATE -fsanitize=address)
# target_link_options(ven PRIVATE -fsanitize=address)

# Precompiled Headers
target_precompile_headers(ven PRIVATE "${PROJECT_ROOT_DIR}/include/pch.h")

message(STATUS "    To build: Ensure a C/C++ compiler is installed.")
message(STATUS "    Ensure VCPKG_ROOT is set in environment path.")
message(STATUS "    IMPORTANT: If build fails, check log files in: ${LOG_FILES_DIR}")
message(STATUS "    Final executable expected at: ${OUTPUT_EXE_FILE}")

# --- Informational Messages ---
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "  Build Directory: ${CMAKE_CURRENT_BINARY_DIR}")