cmake_minimum_required(VERSION 3.15)

# vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(VulkanProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Vulkan
find_package(Vulkan REQUIRED)

# Find vcpkg packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Source files - organized by component
set(CORE_SOURCES
    src/core/Application.cpp
    src/core/Window.cpp
    src/core/InputManager.cpp
)

set(RENDER_SOURCES
    src/render/VulkanContext.cpp
    src/render/SwapChain.cpp
    src/render/GraphicsPipeline.cpp
    src/render/Renderer.cpp
)

set(SOURCES
    src/main.cpp
    ${CORE_SOURCES}
    ${RENDER_SOURCES}
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Vulkan::Vulkan
    glfw
    glm::glm
)

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy shaders to build directory (if you have them)
# file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})