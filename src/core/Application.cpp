#include "Application.h"
#include <iostream>
#include <stdexcept>

Application::Application() {
    std::cout << "====================================" << std::endl;
    std::cout << "LIBRE DCC TOOL - Day 8-9 Refactor" << std::endl;
    std::cout << "====================================" << std::endl;
}

Application::~Application() {
    cleanup();
}

void Application::run() {
    init();
    mainLoop();
}

void Application::init() {
    std::cout << "\n[INITIALIZATION]" << std::endl;

    // Create window first
    window = std::make_unique<Window>(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    // Create input manager
    inputManager = std::make_unique<InputManager>(window.get());

    // Initialize Vulkan (now takes Window*)
    vulkanContext = std::make_unique<VulkanContext>(window.get());
    vulkanContext->init();

    // Initialize timing
    lastFrameTime = std::chrono::steady_clock::now();

    std::cout << "\n[OK] Application initialized successfully!" << std::endl;
    std::cout << "====================================" << std::endl;
}

void Application::mainLoop() {
    std::cout << "\n[MAIN LOOP STARTED]" << std::endl;
    std::cout << "Press ESC to exit...\n" << std::endl;

    while (!window->shouldClose()) {
        // Calculate delta time
        auto currentTime = std::chrono::steady_clock::now();
        deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
        lastFrameTime = currentTime;
        fps = 1.0f / deltaTime;

        // Process input
        window->pollEvents();
        inputManager->update();

        // Check for ESC key to close
        if (inputManager->isKeyPressed(GLFW_KEY_ESCAPE)) {
            std::cout << "ESC pressed - exiting..." << std::endl;
            glfwSetWindowShouldClose(window->getHandle(), GLFW_TRUE);
        }

        // Update application logic
        update(deltaTime);

        // Render frame
        render();
    }

    std::cout << "\n[MAIN LOOP ENDED]" << std::endl;
}

void Application::update(float deltaTime) {
    // Application logic will go here
    // For now, just print FPS occasionally
    static float fpsTimer = 0.0f;
    fpsTimer += deltaTime;

    if (fpsTimer >= 2.0f) {
        std::cout << "FPS: " << static_cast<int>(fps) << std::endl;
        fpsTimer = 0.0f;
    }
}

void Application::render() {
    // Draw the triangle
    vulkanContext->drawFrame();
}

void Application::cleanup() {
    std::cout << "\n[CLEANUP]" << std::endl;

    // Vulkan cleanup
    if (vulkanContext) {
        vulkanContext.reset();
    }

    // Input manager cleanup
    if (inputManager) {
        inputManager.reset();
    }

    // Window cleanup (GLFW)
    if (window) {
        window.reset();
    }

    std::cout << "[OK] Application cleaned up" << std::endl;
    std::cout << "====================================" << std::endl;
}