#pragma once

#include "Window.h"
#include "InputManager.h"
#include "../render/VulkanContext.h"
#include <memory>
#include <chrono>

class Application {
public:
    Application();
    ~Application();

    // Main entry point
    void run();

private:
    void init();
    void mainLoop();
    void cleanup();
    void update(float deltaTime);
    void render();

    // Core components
    std::unique_ptr<Window> window;
    std::unique_ptr<InputManager> inputManager;
    std::unique_ptr<VulkanContext> vulkanContext;

    // Timing
    std::chrono::steady_clock::time_point lastFrameTime;
    float deltaTime = 0.0f;
    float fps = 0.0f;

    // Configuration
    static constexpr int WINDOW_WIDTH = 1280;
    static constexpr int WINDOW_HEIGHT = 720;
    static constexpr const char* WINDOW_TITLE = "Libre DCC Tool - Alpha";
};