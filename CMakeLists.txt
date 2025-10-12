cmake_minimum_required(VERSION 3.15)

# Tell CMake to use vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(VulkanProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Vulkan
find_package(Vulkan REQUIRED)

# Find GLFW
find_package(glfw3 REQUIRED)

# Add executable
add_executable(VulkanProject src/main.cpp)

# Link libraries
target_link_libraries(VulkanProject 
    Vulkan::Vulkan
    glfw
)

# Include directories
target_include_directories(VulkanProject PRIVATE 
    ${Vulkan_INCLUDE_DIRS}
)