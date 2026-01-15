#include "Application.h"
#include "../render/SwapChain.h"
#include "../render/Renderer.h"
#include <iostream>
#include <stdexcept>

Application::Application() {
    std::cout << "====================================" << std::endl;
    std::cout << "LIBRE DCC TOOL - 3D Viewport" << std::endl;
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

    window = std::make_unique<Window>(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    inputManager = std::make_unique<InputManager>(window.get());

    // Create camera
    camera = std::make_unique<Camera>();
    camera->setAspectRatio(static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT));

    vulkanContext = std::make_unique<VulkanContext>(window.get());
    vulkanContext->init();

    swapChain = new SwapChain();
    swapChain->init(vulkanContext.get(), window->getHandle());

    renderer = new Renderer();
    renderer->init(vulkanContext.get(), swapChain);

    lastFrameTime = std::chrono::steady_clock::now();

    std::cout << "\n[OK] Application initialized successfully!" << std::endl;
    std::cout << "\n=== Controls ===" << std::endl;
    std::cout << "Middle Mouse + Drag: Orbit" << std::endl;
    std::cout << "Shift + Middle Mouse + Drag: Pan" << std::endl;
    std::cout << "Scroll Wheel: Zoom" << std::endl;
    std::cout << "Numpad 1: Front View" << std::endl;
    std::cout << "Numpad 3: Right View" << std::endl;
    std::cout << "Numpad 7: Top View" << std::endl;
    std::cout << "Numpad 0: Reset View" << std::endl;
    std::cout << "ESC: Exit" << std::endl;
    std::cout << "================\n" << std::endl;
}

void Application::mainLoop() {
    while (!window->shouldClose()) {
        auto currentTime = std::chrono::steady_clock::now();
        deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
        lastFrameTime = currentTime;
        fps = 1.0f / deltaTime;

        window->pollEvents();
        inputManager->update();

        processInput(deltaTime);
        update(deltaTime);
        render();

        if (window->wasResized()) {
            window->resetResizeFlag();

            int width, height;
            glfwGetFramebufferSize(window->getHandle(), &width, &height);

            if (width > 0 && height > 0) {
                camera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
                recreateSwapChain();
            }
        }
    }

    renderer->waitIdle();
}

void Application::processInput(float deltaTime) {
    // ESC to exit
    if (inputManager->isKeyPressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window->getHandle(), GLFW_TRUE);
    }

    // Track modifier keys
    shiftHeld = inputManager->isKeyPressed(GLFW_KEY_LEFT_SHIFT) ||
        inputManager->isKeyPressed(GLFW_KEY_RIGHT_SHIFT);
    ctrlHeld = inputManager->isKeyPressed(GLFW_KEY_LEFT_CONTROL) ||
        inputManager->isKeyPressed(GLFW_KEY_RIGHT_CONTROL);

    // View shortcuts (numpad)
    if (inputManager->isKeyJustPressed(GLFW_KEY_KP_1)) {
        camera->setFront();
    }
    if (inputManager->isKeyJustPressed(GLFW_KEY_KP_3)) {
        camera->setRight();
    }
    if (inputManager->isKeyJustPressed(GLFW_KEY_KP_7)) {
        camera->setTop();
    }
    if (inputManager->isKeyJustPressed(GLFW_KEY_KP_0)) {
        camera->reset();
    }

    // Middle mouse button for camera control
    if (inputManager->isMouseButtonJustPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
        middleMouseDown = true;
        lastMouseX = inputManager->getMouseX();
        lastMouseY = inputManager->getMouseY();
    }
    if (inputManager->isMouseButtonJustReleased(GLFW_MOUSE_BUTTON_MIDDLE)) {
        middleMouseDown = false;
    }

    // Camera orbit/pan with middle mouse
    if (middleMouseDown) {
        double currentX = inputManager->getMouseX();
        double currentY = inputManager->getMouseY();
        float deltaX = static_cast<float>(currentX - lastMouseX);
        float deltaY = static_cast<float>(currentY - lastMouseY);

        if (shiftHeld) {
            // Pan
            camera->pan(deltaX, deltaY);
        }
        else {
            // Orbit
            camera->orbit(deltaX, deltaY);
        }

        lastMouseX = currentX;
        lastMouseY = currentY;
    }

    // Scroll wheel for zoom
    double scrollY = inputManager->getScrollY();
    if (scrollY != 0.0) {
        camera->zoom(static_cast<float>(scrollY));
    }
}

void Application::update(float deltaTime) {
    // Future: animation, physics, etc.
}

void Application::render() {
    renderer->drawFrame(camera.get());
}

void Application::recreateSwapChain() {
    renderer->waitIdle();
    swapChain->recreate(window->getHandle());
}

void Application::cleanup() {
    std::cout << "\n[CLEANUP]" << std::endl;

    if (renderer) {
        renderer->waitIdle();
        renderer->cleanup();
        delete renderer;
        renderer = nullptr;
    }

    if (swapChain) {
        swapChain->cleanup();
        delete swapChain;
        swapChain = nullptr;
    }

    if (vulkanContext) {
        vulkanContext->cleanup();
        vulkanContext.reset();
    }

    if (camera) {
        camera.reset();
    }

    if (inputManager) {
        inputManager.reset();
    }

    if (window) {
        window.reset();
    }

    std::cout << "[OK] Application cleaned up" << std::endl;
}