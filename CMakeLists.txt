cmake_minimum_required(VERSION 3.16)
project(LibreDCC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

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
    
    # Rendering
    src/render/VulkanContext.cpp
    src/render/VulkanContext.h
    src/render/SwapChain.cpp
    src/render/SwapChain.h
    src/render/Renderer.cpp
    src/render/Renderer.h
    src/render/GraphicsPipeline.cpp
    src/render/GraphicsPipeline.h
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
)

# Copy shaders to build directory
file(GLOB SHADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.spv"
)

foreach(SHADER ${SHADERS})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    configure_file(${SHADER} ${CMAKE_CURRENT_BINARY_DIR}/shaders/${SHADER_NAME} COPYONLY)
endforeach()

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

if(MSVC)
    # Visual Studio specific settings
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)
else()
    # GCC/Clang settings
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

# Debug/Release configurations
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
endif()