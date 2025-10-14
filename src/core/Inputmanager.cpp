#include "InputManager.h"
#include "Window.h"
#include <iostream>

InputManager::InputManager(Window* window) : window(window) {
    // Initialize all key states to false
    for (int i = 0; i <= GLFW_KEY_LAST; i++) {
        prevKeyStates[i] = false;
    }
    for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; i++) {
        prevMouseButtonStates[i] = false;
    }

    std::cout << "[OK] Input manager created" << std::endl;
}

void InputManager::update() {
    GLFWwindow* glfwWindow = window->getHandle();

    // Update keyboard state
    for (int key = 0; key <= GLFW_KEY_LAST; key++) {
        prevKeyStates[key] = (glfwGetKey(glfwWindow, key) == GLFW_PRESS);
    }

    // Update mouse button state
    for (int button = 0; button <= GLFW_MOUSE_BUTTON_LAST; button++) {
        prevMouseButtonStates[button] = (glfwGetMouseButton(glfwWindow, button) == GLFW_PRESS);
    }

    // Update mouse position
    prevMousePos = currentMousePos;
    double xpos, ypos;
    glfwGetCursorPos(glfwWindow, &xpos, &ypos);
    currentMousePos = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
}

bool InputManager::isKeyPressed(int key) const {
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return glfwGetKey(window->getHandle(), key) == GLFW_PRESS;
}

bool InputManager::isKeyDown(int key) const {
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return isKeyPressed(key) && !prevKeyStates[key];
}

bool InputManager::isKeyUp(int key) const {
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return !isKeyPressed(key) && prevKeyStates[key];
}

bool InputManager::isMouseButtonPressed(int button) const {
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false;
    return glfwGetMouseButton(window->getHandle(), button) == GLFW_PRESS;
}

bool InputManager::isMouseButtonDown(int button) const {
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false;
    return isMouseButtonPressed(button) && !prevMouseButtonStates[button];
}

bool InputManager::isMouseButtonUp(int button) const {
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false;
    return !isMouseButtonPressed(button) && prevMouseButtonStates[button];
}

glm::vec2 InputManager::getMousePosition() const {
    return currentMousePos;
}

glm::vec2 InputManager::getMouseDelta() const {
    return currentMousePos - prevMousePos;
}