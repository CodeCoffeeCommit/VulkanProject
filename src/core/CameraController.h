#pragma once

#include "Camera.h"

class InputManager;

/**
 * CameraController - Abstract base class for camera control schemes
 *
 * Different controllers implement different navigation paradigms:
 * - OrbitController: Blender-style orbit around target (MMB drag)
 * - TurntableController: ZBrush-style object rotation (future)
 * - FlyController: Unreal-style WASD + mouselook (future)
 * - PanZoomController: 2D canvas navigation (future)
 *
 * All controllers manipulate the same Camera struct, allowing
 * easy switching between control schemes at runtime.
 */
class CameraController {
public:
    explicit CameraController(Camera* camera) : camera(camera) {}
    virtual ~CameraController() = default;

    // Process input and update camera - called every frame
    virtual void processInput(InputManager* input, float deltaTime) = 0;

    // Update internal state (animations, smoothing) - called every frame
    virtual void update(float deltaTime) { (void)deltaTime; }

    // Reset to default view
    virtual void reset() = 0;

    // View presets (controllers can override or ignore)
    virtual void setFrontView() {}
    virtual void setRightView() {}
    virtual void setTopView() {}
    virtual void setBackView() {}
    virtual void setLeftView() {}
    virtual void setBottomView() {}

    // Focus on a point (orbit around this target)
    virtual void focusOn(const glm::vec3& point) {
        if (camera) camera->target = point;
    }

    // Access the camera
    Camera* getCamera() const { return camera; }

protected:
    Camera* camera = nullptr;
};