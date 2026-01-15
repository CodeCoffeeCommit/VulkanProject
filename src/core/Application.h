#pragma once

#include "Window.h"
#include "InputManager.h"
#include "Camera.h"
#include "../render/VulkanContext.h"
#include <memory>
#include <chrono>

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
    void recreateSwapChain();
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
    std::unique_ptr<Camera> camera;

    // Rendering components
    SwapChain* swapChain = nullptr;
    Renderer* renderer = nullptr;

    // Timing
    std::chrono::steady_clock::time_point lastFrameTime;
    float deltaTime = 0.0f;
    float fps = 0.0f;

    // Mouse state for camera control
    bool middleMouseDown = false;
    bool shiftHeld = false;
    bool ctrlHeld = false;
    bool altHeld = false;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;

    // Configuration
    static constexpr int WINDOW_WIDTH = 1280;
    static constexpr int WINDOW_HEIGHT = 720;
    static constexpr const char* WINDOW_TITLE = "Libre DCC Tool - 3D Viewport";
};