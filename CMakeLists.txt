cmake_minimum_required(VERSION 3.16)
project(LibreDCC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Add executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    
    # Core
    src/core/Application.cpp
    src/core/Application.h
    src/core/Window.cpp
    src/core/Window.h
    src/core/InputManager.cpp
    src/core/InputManager.h
    src/core/Camera.cpp
    src/core/Camera.h
    
    # Rendering
    src/render/VulkanContext.cpp
    src/render/VulkanContext.h
    src/render/SwapChain.cpp
    src/render/SwapChain.h
    src/render/Renderer.cpp
    src/render/Renderer.h
    src/render/GraphicsPipeline.cpp
    src/render/GraphicsPipeline.h
    src/render/Mesh.cpp
    src/render/Mesh.h
    src/render/Grid.cpp
    src/render/Grid.h
    src/render/Primitives.cpp
    src/render/Primitives.h
    src/render/UniformBuffer.cpp
    src/render/UniformBuffer.h
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${Vulkan_LIBRARIES}
    glfw
    glm::glm
)

# Platform-specific settings
if(WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
endif()