#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window;

class InputManager {
public:
    InputManager(Window* window);

    // Update state (call once per frame)
    void update();

    // Keyboard
    bool isKeyPressed(int key) const;
    bool isKeyDown(int key) const;      // Just pressed this frame
    bool isKeyUp(int key) const;        // Just released this frame

    // Mouse buttons
    bool isMouseButtonPressed(int button) const;
    bool isMouseButtonDown(int button) const;
    bool isMouseButtonUp(int button) const;

    // Mouse position
    glm::vec2 getMousePosition() const;
    glm::vec2 getMouseDelta() const;    // Movement since last frame

private:
    Window* window;

    // Previous frame state
    bool prevKeyStates[GLFW_KEY_LAST + 1] = {};
    bool prevMouseButtonStates[GLFW_MOUSE_BUTTON_LAST + 1] = {};
    glm::vec2 prevMousePos = glm::vec2(0.0f);
    glm::vec2 currentMousePos = glm::vec2(0.0f);
};