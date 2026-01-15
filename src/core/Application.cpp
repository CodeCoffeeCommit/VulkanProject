#include "Application.h"
#include "Editor.h"
#include "Selection.h"
#include "../render/SwapChain.h"
#include "../render/Renderer.h"
#include "../world/Primitives.h"
#include <iostream>

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

    // Initialize Editor (ECS)
    libre::Editor::instance().initialize();

    // Create camera
    camera = std::make_unique<Camera>();
    camera->setAspectRatio(static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT));

    vulkanContext = std::make_unique<VulkanContext>(window.get());
    vulkanContext->init();

    swapChain = new SwapChain();
    swapChain->init(vulkanContext.get(), window->getHandle());

    renderer = new Renderer();
    renderer->init(vulkanContext.get(), swapChain);

    // Create default scene objects in ECS
    createDefaultScene();

    lastFrameTime = std::chrono::steady_clock::now();

    std::cout << "\n[OK] Application initialized successfully!" << std::endl;
    printControls();
}

void Application::createDefaultScene() {
    auto& world = libre::Editor::instance().getWorld();

    // Create a cube
    auto cube = libre::Primitives::createCube(world, 2.0f, "DefaultCube");

    // Create a sphere  
    auto sphere = libre::Primitives::createSphere(world, 1.0f, 32, 16, "Sphere");
    if (auto* t = sphere.get<libre::TransformComponent>()) {
        t->position = glm::vec3(3.0f, 0.0f, 0.0f);
        t->dirty = true;
    }

    // Create a cylinder
    auto cylinder = libre::Primitives::createCylinder(world, 0.5f, 2.0f, 32, "Cylinder");
    if (auto* t = cylinder.get<libre::TransformComponent>()) {
        t->position = glm::vec3(-3.0f, 0.0f, 0.0f);
        t->dirty = true;
    }

    std::cout << "[OK] Default scene created with "
        << world.getEntityCount() << " entities" << std::endl;
}

void Application::printControls() {
    std::cout << "\n=== Controls ===" << std::endl;
    std::cout << "Middle Mouse + Drag: Orbit" << std::endl;
    std::cout << "Shift + Middle Mouse: Pan" << std::endl;
    std::cout << "Scroll Wheel: Zoom" << std::endl;
    std::cout << "Left Click: Select" << std::endl;
    std::cout << "Shift + Left Click: Add to Selection" << std::endl;
    std::cout << "A: Select All" << std::endl;
    std::cout << "Alt+A: Deselect All" << std::endl;
    std::cout << "Delete/X: Delete Selected" << std::endl;
    std::cout << "Ctrl+Z: Undo" << std::endl;
    std::cout << "Ctrl+Shift+Z: Redo" << std::endl;
    std::cout << "Numpad 1/3/7/0: View shortcuts" << std::endl;
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

        processInput(deltaTime);
        update(deltaTime);
        render();

        inputManager->update();

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

void Application::processInput(float dt) {
    auto& editor = libre::Editor::instance();

    // ESC to exit
    if (inputManager->isKeyPressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window->getHandle(), GLFW_TRUE);
    }

    // Track modifier keys
    shiftHeld = inputManager->isKeyPressed(GLFW_KEY_LEFT_SHIFT) ||
        inputManager->isKeyPressed(GLFW_KEY_RIGHT_SHIFT);
    ctrlHeld = inputManager->isKeyPressed(GLFW_KEY_LEFT_CONTROL) ||
        inputManager->isKeyPressed(GLFW_KEY_RIGHT_CONTROL);
    altHeld = inputManager->isKeyPressed(GLFW_KEY_LEFT_ALT) ||
        inputManager->isKeyPressed(GLFW_KEY_RIGHT_ALT);

    // Undo/Redo
    if (ctrlHeld && inputManager->isKeyJustPressed(GLFW_KEY_Z)) {
        if (shiftHeld) {
            editor.redo();
            std::cout << "[Redo]" << std::endl;
        }
        else {
            editor.undo();
            std::cout << "[Undo]" << std::endl;
        }
    }

    // Delete selected
    if (inputManager->isKeyJustPressed(GLFW_KEY_DELETE) ||
        inputManager->isKeyJustPressed(GLFW_KEY_X)) {
        editor.deleteSelected();
    }

    // Select All / Deselect All
    if (inputManager->isKeyJustPressed(GLFW_KEY_A)) {
        if (altHeld) {
            editor.deselectAll();
        }
        else {
            editor.selectAll();
        }
    }

    // View shortcuts (numpad)
    if (inputManager->isKeyJustPressed(GLFW_KEY_KP_1)) camera->setFront();
    if (inputManager->isKeyJustPressed(GLFW_KEY_KP_3)) camera->setRight();
    if (inputManager->isKeyJustPressed(GLFW_KEY_KP_7)) camera->setTop();
    if (inputManager->isKeyJustPressed(GLFW_KEY_KP_0)) camera->reset();

    // Left click for selection
    if (inputManager->isMouseButtonJustPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        if (!middleMouseDown) {  // Don't select while orbiting
            handleSelection();
        }
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
            camera->pan(deltaX, deltaY);
        }
        else {
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

void Application::handleSelection() {
    auto& editor = libre::Editor::instance();
    auto& world = editor.getWorld();

    int width, height;
    glfwGetFramebufferSize(window->getHandle(), &width, &height);

    float mouseX = static_cast<float>(inputManager->getMouseX());
    float mouseY = static_cast<float>(inputManager->getMouseY());

    // Create ray from mouse position
    libre::Ray ray = libre::SelectionSystem::screenToRay(
        *camera, mouseX, mouseY, width, height);

    // Perform raycast
    libre::HitResult hit = libre::SelectionSystem::raycast(world, ray);

    if (hit.hit()) {
        editor.select(hit.entity, shiftHeld);

        auto* meta = world.getMetadata(hit.entity);
        if (meta) {
            std::cout << "[Selected] " << meta->name
                << " (distance: " << hit.distance << ")" << std::endl;
        }
    }
    else if (!shiftHeld) {
        editor.deselectAll();
    }
}

void Application::update(float dt) {
    // Update Editor (processes commands, events)
    libre::Editor::instance().update(dt);

    // Update transforms
    updateTransforms();
}

void Application::updateTransforms() {
    auto& world = libre::Editor::instance().getWorld();

    // Update transform matrices
    world.forEach<libre::TransformComponent>([&](libre::EntityID id, libre::TransformComponent& t) {
        if (t.dirty) {
            // For root entities, local = world
            if (world.getParent(id) == libre::INVALID_ENTITY) {
                t.worldMatrix = t.getLocalMatrix();
            }
            else {
                // Get parent transform
                auto* parentT = world.getComponent<libre::TransformComponent>(world.getParent(id));
                if (parentT) {
                    t.worldMatrix = parentT->worldMatrix * t.getLocalMatrix();
                }
            }

            // Update bounds
            auto* bounds = world.getComponent<libre::BoundsComponent>(id);
            if (bounds) {
                bounds->updateWorldBounds(t.worldMatrix);
            }

            t.dirty = false;
        }
        });
}

void Application::render() {
    // Sync ECS data to renderer
    syncECSToRenderer();

    // Draw frame
    renderer->drawFrame(camera.get());
}

void Application::syncECSToRenderer() {
    // In Phase 2, we'll update the renderer to use ECS data directly
    // For now, the renderer uses its own Mesh objects
    // This function will bridge ECS MeshComponents to Vulkan buffers
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

    // Shutdown Editor
    libre::Editor::instance().shutdown();

    if (vulkanContext) {
        vulkanContext->cleanup();
        vulkanContext.reset();
    }

    camera.reset();
    inputManager.reset();
    window.reset();

    std::cout << "[OK] Application cleaned up" << std::endl;
}