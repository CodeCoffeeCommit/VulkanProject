#pragma once

#include "CameraController.h"
#include "InputManager.h"
#include <algorithm>
#include <cmath>

/**
 * OrbitController - Blender-style orbit camera control
 *
 * Controls:
 * - Middle Mouse + Drag: Orbit around target
 * - Shift + Middle Mouse: Pan
 * - Scroll Wheel: Zoom
 * - Numpad 1/3/7/0: View presets
 *
 * The camera orbits around a target point using spherical coordinates.
 * The target can be changed to orbit around different objects.
 */
class OrbitController : public CameraController {
public:
    explicit OrbitController(Camera* camera) : CameraController(camera) {
        reset();
    }

    void processInput(InputManager* input, float deltaTime) override {
        (void)deltaTime; // Not needed for this controller

        if (!camera || !input) return;

        // Track modifier keys
        bool shiftHeld = input->isKeyPressed(GLFW_KEY_LEFT_SHIFT) ||
            input->isKeyPressed(GLFW_KEY_RIGHT_SHIFT);

        // View presets (Numpad)
        if (input->isKeyJustPressed(GLFW_KEY_KP_1)) setFrontView();
        if (input->isKeyJustPressed(GLFW_KEY_KP_3)) setRightView();
        if (input->isKeyJustPressed(GLFW_KEY_KP_7)) setTopView();
        if (input->isKeyJustPressed(GLFW_KEY_KP_0)) reset();

        // Middle mouse button handling
        if (input->isMouseButtonJustPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
            middleMouseDown = true;
            lastMouseX = input->getMouseX();
            lastMouseY = input->getMouseY();
        }
        if (input->isMouseButtonJustReleased(GLFW_MOUSE_BUTTON_MIDDLE)) {
            middleMouseDown = false;
        }

        // Orbit / Pan with middle mouse
        if (middleMouseDown) {
            double currentX = input->getMouseX();
            double currentY = input->getMouseY();
            float deltaX = static_cast<float>(currentX - lastMouseX);
            float deltaY = static_cast<float>(currentY - lastMouseY);

            if (shiftHeld) {
                pan(deltaX, deltaY);
            }
            else {
                orbit(deltaX, deltaY);
            }

            lastMouseX = currentX;
            lastMouseY = currentY;
        }

        // Zoom with scroll wheel
        double scrollY = input->getScrollY();
        if (scrollY != 0.0) {
            zoom(static_cast<float>(scrollY));
        }
    }

    void reset() override {
        distance = 10.0f;
        azimuth = 45.0f;
        elevation = 30.0f;
        if (camera) {
            camera->target = glm::vec3(0.0f, 0.0f, 0.0f);
        }
        updateCameraPosition();
    }

    void setFrontView() override {
        azimuth = 0.0f;
        elevation = 0.0f;
        updateCameraPosition();
    }

    void setRightView() override {
        azimuth = 90.0f;
        elevation = 0.0f;
        updateCameraPosition();
    }

    void setTopView() override {
        azimuth = 0.0f;
        elevation = 89.9f;
        updateCameraPosition();
    }

    void setBackView() override {
        azimuth = 180.0f;
        elevation = 0.0f;
        updateCameraPosition();
    }

    void setLeftView() override {
        azimuth = -90.0f;
        elevation = 0.0f;
        updateCameraPosition();
    }

    void setBottomView() override {
        azimuth = 0.0f;
        elevation = -89.9f;
        updateCameraPosition();
    }

    void focusOn(const glm::vec3& point) override {
        if (camera) {
            camera->target = point;
            updateCameraPosition();
        }
    }

    // Direct access for external control
    void setDistance(float d) { distance = std::clamp(d, 0.5f, 500.0f); updateCameraPosition(); }
    float getDistance() const { return distance; }

private:
    void orbit(float deltaX, float deltaY) {
        azimuth -= deltaX * orbitSensitivity;
        elevation += deltaY * orbitSensitivity;

        // Clamp elevation to prevent flipping
        elevation = std::clamp(elevation, -89.0f, 89.0f);

        // Wrap azimuth
        if (azimuth > 360.0f) azimuth -= 360.0f;
        if (azimuth < 0.0f) azimuth += 360.0f;

        updateCameraPosition();
    }

    void pan(float deltaX, float deltaY) {
        if (!camera) return;

        // Calculate right and up vectors relative to camera
        glm::vec3 forward = glm::normalize(camera->target - camera->position);
        glm::vec3 right = glm::normalize(glm::cross(forward, camera->up));
        glm::vec3 camUp = glm::normalize(glm::cross(right, forward));

        // Pan the target
        camera->target -= right * deltaX * panSensitivity * distance;
        camera->target += camUp * deltaY * panSensitivity * distance;

        updateCameraPosition();
    }

    void zoom(float delta) {
        distance -= delta * zoomSensitivity;
        distance = std::clamp(distance, 0.5f, 500.0f);
        updateCameraPosition();
    }

    void updateCameraPosition() {
        if (!camera) return;

        // Convert spherical to Cartesian coordinates
        float azimuthRad = glm::radians(azimuth);
        float elevationRad = glm::radians(elevation);

        camera->position.x = camera->target.x + distance * cos(elevationRad) * sin(azimuthRad);
        camera->position.y = camera->target.y + distance * sin(elevationRad);
        camera->position.z = camera->target.z + distance * cos(elevationRad) * cos(azimuthRad);

        camera->updateMatrices();
    }

    // Orbit parameters (spherical coordinates)
    float distance = 10.0f;
    float azimuth = 45.0f;     // Horizontal angle (degrees)
    float elevation = 30.0f;   // Vertical angle (degrees)

    // Mouse tracking
    bool middleMouseDown = false;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;

    // Sensitivity settings
    float orbitSensitivity = 0.5f;
    float panSensitivity = 0.01f;
    float zoomSensitivity = 1.0f;
};