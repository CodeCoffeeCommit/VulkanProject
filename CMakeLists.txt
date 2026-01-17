cmake_minimum_required(VERSION 3.16)
project(LibreDCC)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Find glslc shader compiler (comes with Vulkan SDK)
find_program(GLSLC glslc HINTS 
    "$ENV{VULKAN_SDK}/Bin"
    "$ENV{VK_SDK_PATH}/Bin"
)

if(NOT GLSLC)
    message(FATAL_ERROR "glslc not found! Make sure Vulkan SDK is installed.")
endif()

message(STATUS "Found glslc: ${GLSLC}")

# Define shader sources
set(SHADER_SOURCES
    ${CMAKE_SOURCE_DIR}/shaders/workbench.vert
    ${CMAKE_SOURCE_DIR}/shaders/workbench.frag
    ${CMAKE_SOURCE_DIR}/shaders/grid.vert
    ${CMAKE_SOURCE_DIR}/shaders/grid.frag
    ${CMAKE_SOURCE_DIR}/shaders/ui.vert
    ${CMAKE_SOURCE_DIR}/shaders/ui.frag
)

# Create compiled shaders output directory
set(SHADER_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/shaders/compiled)
file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

# Compile each shader
set(SHADER_SPIRV_FILES "")
foreach(SHADER ${SHADER_SOURCES})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    set(SPIRV_OUTPUT ${SHADER_OUTPUT_DIR}/${SHADER_NAME}.spv)
    
    add_custom_command(
        OUTPUT ${SPIRV_OUTPUT}
        COMMAND ${GLSLC} ${SHADER} -o ${SPIRV_OUTPUT}
        DEPENDS ${SHADER}
        COMMENT "Compiling shader: ${SHADER_NAME}"
    )
    
    list(APPEND SHADER_SPIRV_FILES ${SPIRV_OUTPUT})
endforeach()

# Create a target for shaders
add_custom_target(Shaders DEPENDS ${SHADER_SPIRV_FILES})

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
    src/core/CallbackData.h
    src/core/Camera.h
    src/core/CameraController.h
    src/core/OrbitController.h
    src/core/Event.h
    src/core/Command.h
    src/core/Editor.cpp
    src/core/Editor.h
    src/core/Selection.h
    
    # World (ECS)
    src/world/Types.h
    src/world/ComponentStorage.h
    src/world/RelationshipStore.h
    src/world/World.cpp
    src/world/World.h
    src/world/Primitives.h 
    
    # Components
    src/components/CoreComponents.h
    
    # UI
    src/ui/Core.h
    src/ui/Theme.h
    src/ui/Renderer.h
    src/ui/Renderer.cpp
    src/ui/Widgets.h
    src/ui/Widgets.cpp
    src/ui/UI.h
    src/ui/UI.cpp
    
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

# Make sure shaders are built before the main executable
add_dependencies(${PROJECT_NAME} Shaders)

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

# Copy compiled shaders to build output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/compiled"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders/compiled"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/compiled"
    COMMENT "Copying shaders to output directory"
)


