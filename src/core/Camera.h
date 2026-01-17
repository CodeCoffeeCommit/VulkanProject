#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * Camera - Pure data struct representing camera state
 *
 * The Camera itself has no behavior - it's just data.
 * CameraController subclasses manipulate this data based on input.
 * This separation allows different control schemes (orbit, fly, turntable)
 * to all work with the same Camera struct.
 */
struct Camera {
    // Position and orientation
    glm::vec3 position{ 0.0f, 5.0f, 10.0f };
    glm::vec3 target{ 0.0f, 0.0f, 0.0f };
    glm::vec3 up{ 0.0f, 1.0f, 0.0f };

    // Projection settings
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    float aspectRatio = 16.0f / 9.0f;

    // Computed matrices (updated by controller or manually)
    glm::mat4 viewMatrix{ 1.0f };
    glm::mat4 projectionMatrix{ 1.0f };

    // Recompute matrices from current state
    void updateMatrices() {
        viewMatrix = glm::lookAt(position, target, up);
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        projectionMatrix[1][1] *= -1; // Flip Y for Vulkan
    }

    // Convenience getters (for compatibility with existing code)
    glm::mat4 getViewMatrix() const { return viewMatrix; }
    glm::mat4 getProjectionMatrix() const { return projectionMatrix; }
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getTarget() const { return target; }

    void setAspectRatio(float aspect) {
        aspectRatio = aspect;
        updateMatrices();
    }
};