#pragma once

#include "../world/World.h"
#include "../components/CoreComponents.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <vector>
#include <optional>

namespace libre {

    // ============================================================================
    // RAY - For raycasting
    // ============================================================================

    struct Ray {
        glm::vec3 origin;
        glm::vec3 direction;

        glm::vec3 at(float t) const {
            return origin + direction * t;
        }
    };

    // ============================================================================
    // HIT RESULT
    // ============================================================================

    struct HitResult {
        EntityID entity = INVALID_ENTITY;
        float distance = std::numeric_limits<float>::max();
        glm::vec3 hitPoint;
        glm::vec3 hitNormal;

        bool hit() const { return entity != INVALID_ENTITY; }
    };

    // ============================================================================
    // SELECTION SYSTEM
    // ============================================================================

    class SelectionSystem {
    public:
        // Generate ray from screen coordinates
        static Ray screenToRay(const Camera& camera, float screenX, float screenY,
            int screenWidth, int screenHeight) {
            // Convert to NDC (-1 to 1)
            float x = (2.0f * screenX) / screenWidth - 1.0f;
            float y = 1.0f - (2.0f * screenY) / screenHeight;  // Flip Y

            glm::mat4 invProj = glm::inverse(camera.getProjectionMatrix());
            glm::mat4 invView = glm::inverse(camera.getViewMatrix());

            // Unproject near and far points
            glm::vec4 rayNDC(x, y, -1.0f, 1.0f);
            glm::vec4 rayFarNDC(x, y, 1.0f, 1.0f);

            glm::vec4 rayEye = invProj * rayNDC;
            rayEye /= rayEye.w;

            glm::vec4 rayFarEye = invProj * rayFarNDC;
            rayFarEye /= rayFarEye.w;

            glm::vec4 rayWorld = invView * rayEye;
            glm::vec4 rayFarWorld = invView * rayFarEye;

            Ray ray;
            ray.origin = glm::vec3(rayWorld);
            ray.direction = glm::normalize(glm::vec3(rayFarWorld - rayWorld));

            return ray;
        }

        // Raycast against all entities
        static HitResult raycast(World& world, const Ray& ray) {
            HitResult closest;

            // Get all entities with bounds and render components
            auto* boundsStorage = world.getStorage<BoundsComponent>();
            if (!boundsStorage) return closest;

            boundsStorage->forEach([&](EntityID id, BoundsComponent& bounds) {
                // Skip hidden/unselectable entities
                auto* meta = world.getMetadata(id);
                if (meta && !meta->isSelectable()) return;

                auto* render = world.getComponent<RenderComponent>(id);
                if (render && !render->visible) return;

                // Update world bounds if needed
                if (bounds.dirty) {
                    auto* transform = world.getComponent<TransformComponent>(id);
                    if (transform) {
                        bounds.updateWorldBounds(transform->worldMatrix);
                    }
                }

                // Test AABB intersection
                float tMin, tMax;
                if (bounds.intersectsRay(ray.origin, ray.direction, tMin, tMax)) {
                    if (tMin < closest.distance && tMin > 0.0f) {
                        // For more precise hit, could do triangle intersection here
                        closest.entity = id;
                        closest.distance = tMin;
                        closest.hitPoint = ray.at(tMin);
                        // Simple normal approximation
                        closest.hitNormal = glm::normalize(closest.hitPoint - bounds.worldCenter);
                    }
                }
                });

            return closest;
        }

        // Raycast with triangle-level precision
        static HitResult raycastPrecise(World& world, const Ray& ray) {
            HitResult closest;

            auto* meshStorage = world.getStorage<MeshComponent>();
            if (!meshStorage) return closest;

            meshStorage->forEach([&](EntityID id, MeshComponent& mesh) {
                auto* meta = world.getMetadata(id);
                if (meta && !meta->isSelectable()) return;

                auto* render = world.getComponent<RenderComponent>(id);
                if (render && !render->visible) return;

                auto* transform = world.getComponent<TransformComponent>(id);
                if (!transform) return;

                // Transform ray to local space
                glm::mat4 invWorld = glm::inverse(transform->worldMatrix);
                glm::vec4 localOrigin = invWorld * glm::vec4(ray.origin, 1.0f);
                glm::vec4 localDir = invWorld * glm::vec4(ray.direction, 0.0f);

                Ray localRay;
                localRay.origin = glm::vec3(localOrigin);
                localRay.direction = glm::normalize(glm::vec3(localDir));

                // Test each triangle
                for (size_t i = 0; i < mesh.indices.size(); i += 3) {
                    glm::vec3 v0 = mesh.vertices[mesh.indices[i]].position;
                    glm::vec3 v1 = mesh.vertices[mesh.indices[i + 1]].position;
                    glm::vec3 v2 = mesh.vertices[mesh.indices[i + 2]].position;

                    float t;
                    if (rayTriangleIntersect(localRay, v0, v1, v2, t)) {
                        if (t < closest.distance && t > 0.001f) {
                            closest.entity = id;
                            closest.distance = t;

                            // Transform hit point back to world space
                            glm::vec3 localHit = localRay.at(t);
                            closest.hitPoint = glm::vec3(transform->worldMatrix *
                                glm::vec4(localHit, 1.0f));

                            // Calculate face normal
                            glm::vec3 localNormal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
                            closest.hitNormal = glm::normalize(glm::vec3(
                                glm::transpose(invWorld) * glm::vec4(localNormal, 0.0f)));
                        }
                    }
                }
                });

            return closest;
        }

        // Box select (frustum selection)
        static std::vector<EntityID> boxSelect(World& world, const Camera& camera,
            float x1, float y1, float x2, float y2,
            int screenWidth, int screenHeight) {
            std::vector<EntityID> selected;

            // Ensure proper ordering
            if (x1 > x2) std::swap(x1, x2);
            if (y1 > y2) std::swap(y1, y2);

            auto* boundsStorage = world.getStorage<BoundsComponent>();
            if (!boundsStorage) return selected;

            glm::mat4 viewProj = camera.getProjectionMatrix() * camera.getViewMatrix();

            boundsStorage->forEach([&](EntityID id, BoundsComponent& bounds) {
                auto* meta = world.getMetadata(id);
                if (meta && !meta->isSelectable()) return;

                // Project center to screen
                glm::vec4 clip = viewProj * glm::vec4(bounds.worldCenter, 1.0f);
                if (clip.w <= 0) return;  // Behind camera

                glm::vec3 ndc = glm::vec3(clip) / clip.w;
                float sx = (ndc.x + 1.0f) * 0.5f * screenWidth;
                float sy = (1.0f - ndc.y) * 0.5f * screenHeight;

                // Check if in selection box
                if (sx >= x1 && sx <= x2 && sy >= y1 && sy <= y2) {
                    selected.push_back(id);
                }
                });

            return selected;
        }

    private:
        // Möller–Trumbore ray-triangle intersection
        static bool rayTriangleIntersect(const Ray& ray,
            const glm::vec3& v0,
            const glm::vec3& v1,
            const glm::vec3& v2,
            float& t) {
            const float EPSILON = 0.0000001f;

            glm::vec3 edge1 = v1 - v0;
            glm::vec3 edge2 = v2 - v0;
            glm::vec3 h = glm::cross(ray.direction, edge2);
            float a = glm::dot(edge1, h);

            if (a > -EPSILON && a < EPSILON) return false;

            float f = 1.0f / a;
            glm::vec3 s = ray.origin - v0;
            float u = f * glm::dot(s, h);

            if (u < 0.0f || u > 1.0f) return false;

            glm::vec3 q = glm::cross(s, edge1);
            float v = f * glm::dot(ray.direction, q);

            if (v < 0.0f || u + v > 1.0f) return false;

            t = f * glm::dot(edge2, q);
            return t > EPSILON;
        }
    };

} // namespace libre
