#pragma once

#include "Window.h"
#include "InputManager.h"
#include "Camera.h"
#include "CameraController.h"
#include "../render/VulkanContext.h"
#include <memory>
#include <chrono>

// Forward declarations
class SwapChain;
class Renderer;

class Application {
public:
    Application();
    ~Application();

    void run();

private:
    void init();
    void mainLoop();
    void cleanup();
    void update(float deltaTime);
    void render();

    // Resize handling
    void handleResize();
    bool isMinimized() const;

    void processInput(float deltaTime);

    // ECS integration
    void createDefaultScene();
    void updateTransforms();
    void syncECSToRenderer();
    void handleSelection();
    void printControls();

    // Core components
    std::unique_ptr<Window> window;
    std::unique_ptr<InputManager> inputManager;
    std::unique_ptr<VulkanContext> vulkanContext;

    // Camera system (separated: data + controller)
    std::unique_ptr<Camera> camera;
    std::unique_ptr<CameraController> cameraController;

    // Rendering components
    std::unique_ptr<SwapChain> swapChain;
    std::unique_ptr<Renderer> renderer;

    // Timing
    std::chrono::steady_clock::time_point lastFrameTime;
    float deltaTime = 0.0f;
    float fps = 0.0f;

    // Input state for non-camera controls
    bool shiftHeld = false;
    bool ctrlHeld = false;
    bool altHeld = false;

    // Resize tracking
    bool framebufferResized = false;

    // Configuration
    static constexpr int WINDOW_WIDTH = 1280;
    static constexpr int WINDOW_HEIGHT = 720;
    static constexpr const char* WINDOW_TITLE = "Libre DCC Tool - 3D Viewport";
};