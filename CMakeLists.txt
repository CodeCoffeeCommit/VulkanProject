cmake_minimum_required(VERSION 3.15)

# vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(VulkanProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Include all source files for triangle rendering
set(ALL_SOURCES
    src/main.cpp
    src/render/VulkanContext.cpp
    src/render/GraphicsPipeline.cpp
    src/render/Renderer.cpp
    src/render/SwapChain.cpp
)

# Create executable
add_executable(VulkanProject ${ALL_SOURCES})

# Link libraries
target_link_libraries(VulkanProject 
    Vulkan::Vulkan
    glfw
    glm::glm
)

# Include directories
target_include_directories(VulkanProject PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
)