#include "InputManager.h"
#include "Window.h"
#include <iostream>

InputManager::InputManager(Window* window) : window(window) {
    glfwWindow = window->getHandle();

    // Set user pointer to this instance
    glfwSetWindowUserPointer(glfwWindow, this);

    // Set callbacks
    glfwSetKeyCallback(glfwWindow, keyCallback);
    glfwSetMouseButtonCallback(glfwWindow, mouseButtonCallback);
    glfwSetCursorPosCallback(glfwWindow, cursorPositionCallback);
    glfwSetScrollCallback(glfwWindow, scrollCallback);

    // Get initial mouse position
    glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);
    lastMouseX = mouseX;
    lastMouseY = mouseY;

    std::cout << "[OK] Input manager initialized" << std::endl;
}

InputManager::~InputManager() {
    std::cout << "[OK] Input manager destroyed" << std::endl;
}

void InputManager::update() {
    // Clear just pressed/released states
    keysJustPressed.clear();
    keysJustReleased.clear();
    mouseButtonsJustPressed.clear();
    mouseButtonsJustReleased.clear();

    // Update mouse delta
    mouseDeltaX = mouseX - lastMouseX;
    mouseDeltaY = mouseY - lastMouseY;
    lastMouseX = mouseX;
    lastMouseY = mouseY;

    // Reset scroll (scroll is event-based, not state-based)
    scrollX = 0.0;
    scrollY = 0.0;
}

bool InputManager::isKeyPressed(int key) const {
    auto it = keysPressed.find(key);
    return it != keysPressed.end() && it->second;
}

bool InputManager::isKeyJustPressed(int key) const {
    auto it = keysJustPressed.find(key);
    return it != keysJustPressed.end() && it->second;
}

bool InputManager::isKeyJustReleased(int key) const {
    auto it = keysJustReleased.find(key);
    return it != keysJustReleased.end() && it->second;
}

bool InputManager::isMouseButtonPressed(int button) const {
    auto it = mouseButtonsPressed.find(button);
    return it != mouseButtonsPressed.end() && it->second;
}

bool InputManager::isMouseButtonJustPressed(int button) const {
    auto it = mouseButtonsJustPressed.find(button);
    return it != mouseButtonsJustPressed.end() && it->second;
}

bool InputManager::isMouseButtonJustReleased(int button) const {
    auto it = mouseButtonsJustReleased.find(button);
    return it != mouseButtonsJustReleased.end() && it->second;
}

// Static callbacks
void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (!input) return;

    if (action == GLFW_PRESS) {
        input->keysPressed[key] = true;
        input->keysJustPressed[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        input->keysPressed[key] = false;
        input->keysJustReleased[key] = true;
    }
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (!input) return;

    if (action == GLFW_PRESS) {
        input->mouseButtonsPressed[button] = true;
        input->mouseButtonsJustPressed[button] = true;
    }
    else if (action == GLFW_RELEASE) {
        input->mouseButtonsPressed[button] = false;
        input->mouseButtonsJustReleased[button] = true;
    }
}

void InputManager::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    auto* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (!input) return;

    input->mouseX = xpos;
    input->mouseY = ypos;

    if (input->firstMouse) {
        input->lastMouseX = xpos;
        input->lastMouseY = ypos;
        input->firstMouse = false;
    }
}

void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    auto* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (!input) return;

    input->scrollX = xoffset;
    input->scrollY = yoffset;
}