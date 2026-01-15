#pragma once

#include "World.h"
#include "../components/CoreComponents.h"
#include <glm/glm.hpp>
#include <cmath>

namespace libre {

    // ============================================================================
    // ECS PRIMITIVE FACTORY
    // ============================================================================
    // Creates entities with Mesh, Transform, Render, Bounds components

    class Primitives {
    public:
        // Create a cube entity
        static EntityHandle createCube(World& world, float size = 1.0f,
            const std::string& name = "Cube") {
            auto entity = world.createEntity(name, "mesh");
            EntityID id = entity.getID();

            // Add mesh component
            MeshComponent mesh;
            generateCubeMesh(mesh, size);
            world.addComponent<MeshComponent>(id, mesh);

            // Add render component
            RenderComponent render;
            render.baseColor = glm::vec3(0.8f, 0.8f, 0.8f);
            world.addComponent<RenderComponent>(id, render);

            // Add bounds component
            BoundsComponent bounds;
            float h = size * 0.5f;
            bounds.localMin = glm::vec3(-h);
            bounds.localMax = glm::vec3(h);
            world.addComponent<BoundsComponent>(id, bounds);

            return entity;
        }

        // Create a sphere entity
        static EntityHandle createSphere(World& world, float radius = 1.0f,
            int segments = 32, int rings = 16,
            const std::string& name = "Sphere") {
            auto entity = world.createEntity(name, "mesh");
            EntityID id = entity.getID();

            MeshComponent mesh;
            generateSphereMesh(mesh, radius, segments, rings);
            world.addComponent<MeshComponent>(id, mesh);

            RenderComponent render;
            render.baseColor = glm::vec3(0.8f, 0.8f, 0.8f);
            world.addComponent<RenderComponent>(id, render);

            BoundsComponent bounds;
            bounds.localMin = glm::vec3(-radius);
            bounds.localMax = glm::vec3(radius);
            world.addComponent<BoundsComponent>(id, bounds);

            return entity;
        }

        // Create a plane entity
        static EntityHandle createPlane(World& world, float size = 10.0f,
            int subdivisions = 1,
            const std::string& name = "Plane") {
            auto entity = world.createEntity(name, "mesh");
            EntityID id = entity.getID();

            MeshComponent mesh;
            generatePlaneMesh(mesh, size, subdivisions);
            world.addComponent<MeshComponent>(id, mesh);

            RenderComponent render;
            render.baseColor = glm::vec3(0.6f, 0.6f, 0.6f);
            world.addComponent<RenderComponent>(id, render);

            BoundsComponent bounds;
            float h = size * 0.5f;
            bounds.localMin = glm::vec3(-h, -0.001f, -h);
            bounds.localMax = glm::vec3(h, 0.001f, h);
            world.addComponent<BoundsComponent>(id, bounds);

            return entity;
        }

        // Create a cylinder entity
        static EntityHandle createCylinder(World& world, float radius = 0.5f,
            float height = 2.0f, int segments = 32,
            const std::string& name = "Cylinder") {
            auto entity = world.createEntity(name, "mesh");
            EntityID id = entity.getID();

            MeshComponent mesh;
            generateCylinderMesh(mesh, radius, height, segments);
            world.addComponent<MeshComponent>(id, mesh);

            RenderComponent render;
            render.baseColor = glm::vec3(0.8f, 0.8f, 0.8f);
            world.addComponent<RenderComponent>(id, render);

            BoundsComponent bounds;
            float halfH = height * 0.5f;
            bounds.localMin = glm::vec3(-radius, -halfH, -radius);
            bounds.localMax = glm::vec3(radius, halfH, radius);
            world.addComponent<BoundsComponent>(id, bounds);

            return entity;
        }

    private:
        // ========================================================================
        // MESH GENERATION
        // ========================================================================

        static void generateCubeMesh(MeshComponent& mesh, float size) {
            float h = size * 0.5f;
            glm::vec3 baseColor(0.8f);

            // 24 vertices (4 per face for proper normals)
            mesh.vertices = {
                // Front face (Z+)
                {{-h, -h,  h}, {0, 0, 1}, baseColor * 0.9f, {0, 0}},
                {{ h, -h,  h}, {0, 0, 1}, baseColor * 0.9f, {1, 0}},
                {{ h,  h,  h}, {0, 0, 1}, baseColor * 0.9f, {1, 1}},
                {{-h,  h,  h}, {0, 0, 1}, baseColor * 0.9f, {0, 1}},
                // Back face (Z-)
                {{ h, -h, -h}, {0, 0, -1}, baseColor * 0.7f, {0, 0}},
                {{-h, -h, -h}, {0, 0, -1}, baseColor * 0.7f, {1, 0}},
                {{-h,  h, -h}, {0, 0, -1}, baseColor * 0.7f, {1, 1}},
                {{ h,  h, -h}, {0, 0, -1}, baseColor * 0.7f, {0, 1}},
                // Top face (Y+)
                {{-h,  h,  h}, {0, 1, 0}, baseColor * 1.0f, {0, 0}},
                {{ h,  h,  h}, {0, 1, 0}, baseColor * 1.0f, {1, 0}},
                {{ h,  h, -h}, {0, 1, 0}, baseColor * 1.0f, {1, 1}},
                {{-h,  h, -h}, {0, 1, 0}, baseColor * 1.0f, {0, 1}},
                // Bottom face (Y-)
                {{-h, -h, -h}, {0, -1, 0}, baseColor * 0.5f, {0, 0}},
                {{ h, -h, -h}, {0, -1, 0}, baseColor * 0.5f, {1, 0}},
                {{ h, -h,  h}, {0, -1, 0}, baseColor * 0.5f, {1, 1}},
                {{-h, -h,  h}, {0, -1, 0}, baseColor * 0.5f, {0, 1}},
                // Right face (X+)
                {{ h, -h,  h}, {1, 0, 0}, baseColor * 0.85f, {0, 0}},
                {{ h, -h, -h}, {1, 0, 0}, baseColor * 0.85f, {1, 0}},
                {{ h,  h, -h}, {1, 0, 0}, baseColor * 0.85f, {1, 1}},
                {{ h,  h,  h}, {1, 0, 0}, baseColor * 0.85f, {0, 1}},
                // Left face (X-)
                {{-h, -h, -h}, {-1, 0, 0}, baseColor * 0.65f, {0, 0}},
                {{-h, -h,  h}, {-1, 0, 0}, baseColor * 0.65f, {1, 0}},
                {{-h,  h,  h}, {-1, 0, 0}, baseColor * 0.65f, {1, 1}},
                {{-h,  h, -h}, {-1, 0, 0}, baseColor * 0.65f, {0, 1}},
            };

            mesh.indices = {
                0,1,2, 2,3,0,       // Front
                4,5,6, 6,7,4,       // Back
                8,9,10, 10,11,8,    // Top
                12,13,14, 14,15,12, // Bottom
                16,17,18, 18,19,16, // Right
                20,21,22, 22,23,20  // Left
            };

            mesh.calculateBounds();
        }

        static void generateSphereMesh(MeshComponent& mesh, float radius,
            int segments, int rings) {
            glm::vec3 baseColor(0.8f);

            for (int y = 0; y <= rings; y++) {
                for (int x = 0; x <= segments; x++) {
                    float xSeg = (float)x / (float)segments;
                    float ySeg = (float)y / (float)rings;
                    float theta = xSeg * 2.0f * 3.14159f;
                    float phi = ySeg * 3.14159f;

                    float xPos = std::cos(theta) * std::sin(phi);
                    float yPos = std::cos(phi);
                    float zPos = std::sin(theta) * std::sin(phi);

                    glm::vec3 pos(xPos * radius, yPos * radius, zPos * radius);
                    glm::vec3 normal = glm::normalize(pos);

                    MeshVertex v;
                    v.position = pos;
                    v.normal = normal;
                    v.color = baseColor;
                    v.uv = glm::vec2(xSeg, ySeg);
                    mesh.vertices.push_back(v);
                }
            }

            for (int y = 0; y < rings; y++) {
                for (int x = 0; x < segments; x++) {
                    uint32_t i0 = y * (segments + 1) + x;
                    uint32_t i1 = i0 + 1;
                    uint32_t i2 = (y + 1) * (segments + 1) + x;
                    uint32_t i3 = i2 + 1;

                    mesh.indices.push_back(i0);
                    mesh.indices.push_back(i2);
                    mesh.indices.push_back(i1);

                    mesh.indices.push_back(i1);
                    mesh.indices.push_back(i2);
                    mesh.indices.push_back(i3);
                }
            }

            mesh.calculateBounds();
        }

        static void generatePlaneMesh(MeshComponent& mesh, float size, int subdivisions) {
            glm::vec3 baseColor(0.6f);
            glm::vec3 normal(0, 1, 0);
            float h = size * 0.5f;
            float step = size / subdivisions;

            for (int z = 0; z <= subdivisions; z++) {
                for (int x = 0; x <= subdivisions; x++) {
                    float xPos = -h + x * step;
                    float zPos = -h + z * step;

                    MeshVertex v;
                    v.position = glm::vec3(xPos, 0, zPos);
                    v.normal = normal;
                    v.color = baseColor;
                    v.uv = glm::vec2((float)x / subdivisions, (float)z / subdivisions);
                    mesh.vertices.push_back(v);
                }
            }

            for (int z = 0; z < subdivisions; z++) {
                for (int x = 0; x < subdivisions; x++) {
                    uint32_t i0 = z * (subdivisions + 1) + x;
                    uint32_t i1 = i0 + 1;
                    uint32_t i2 = (z + 1) * (subdivisions + 1) + x;
                    uint32_t i3 = i2 + 1;

                    mesh.indices.push_back(i0);
                    mesh.indices.push_back(i2);
                    mesh.indices.push_back(i1);
                    mesh.indices.push_back(i1);
                    mesh.indices.push_back(i2);
                    mesh.indices.push_back(i3);
                }
            }

            mesh.calculateBounds();
        }

        static void generateCylinderMesh(MeshComponent& mesh, float radius,
            float height, int segments) {
            glm::vec3 baseColor(0.8f);
            float halfH = height * 0.5f;

            // Side vertices
            for (int i = 0; i <= segments; i++) {
                float angle = (float)i / segments * 2.0f * 3.14159f;
                float x = std::cos(angle) * radius;
                float z = std::sin(angle) * radius;
                glm::vec3 normal = glm::normalize(glm::vec3(x, 0, z));

                // Bottom vertex
                MeshVertex vb;
                vb.position = glm::vec3(x, -halfH, z);
                vb.normal = normal;
                vb.color = baseColor * 0.8f;
                vb.uv = glm::vec2((float)i / segments, 0);
                mesh.vertices.push_back(vb);

                // Top vertex
                MeshVertex vt;
                vt.position = glm::vec3(x, halfH, z);
                vt.normal = normal;
                vt.color = baseColor * 0.9f;
                vt.uv = glm::vec2((float)i / segments, 1);
                mesh.vertices.push_back(vt);
            }

            // Side indices
            for (int i = 0; i < segments; i++) {
                uint32_t b0 = i * 2;
                uint32_t t0 = i * 2 + 1;
                uint32_t b1 = (i + 1) * 2;
                uint32_t t1 = (i + 1) * 2 + 1;

                mesh.indices.push_back(b0);
                mesh.indices.push_back(b1);
                mesh.indices.push_back(t0);
                mesh.indices.push_back(t0);
                mesh.indices.push_back(b1);
                mesh.indices.push_back(t1);
            }

            // Top and bottom caps
            uint32_t centerTopIdx = mesh.vertices.size();
            MeshVertex ct;
            ct.position = glm::vec3(0, halfH, 0);
            ct.normal = glm::vec3(0, 1, 0);
            ct.color = baseColor;
            ct.uv = glm::vec2(0.5f, 0.5f);
            mesh.vertices.push_back(ct);

            uint32_t centerBotIdx = mesh.vertices.size();
            MeshVertex cb;
            cb.position = glm::vec3(0, -halfH, 0);
            cb.normal = glm::vec3(0, -1, 0);
            cb.color = baseColor * 0.6f;
            cb.uv = glm::vec2(0.5f, 0.5f);
            mesh.vertices.push_back(cb);

            // Cap vertices and indices
            uint32_t capStart = mesh.vertices.size();
            for (int i = 0; i <= segments; i++) {
                float angle = (float)i / segments * 2.0f * 3.14159f;
                float x = std::cos(angle) * radius;
                float z = std::sin(angle) * radius;

                // Top cap vertex
                MeshVertex vt;
                vt.position = glm::vec3(x, halfH, z);
                vt.normal = glm::vec3(0, 1, 0);
                vt.color = baseColor;
                mesh.vertices.push_back(vt);

                // Bottom cap vertex  
                MeshVertex vb;
                vb.position = glm::vec3(x, -halfH, z);
                vb.normal = glm::vec3(0, -1, 0);
                vb.color = baseColor * 0.6f;
                mesh.vertices.push_back(vb);
            }

            for (int i = 0; i < segments; i++) {
                // Top cap
                mesh.indices.push_back(centerTopIdx);
                mesh.indices.push_back(capStart + i * 2);
                mesh.indices.push_back(capStart + (i + 1) * 2);

                // Bottom cap
                mesh.indices.push_back(centerBotIdx);
                mesh.indices.push_back(capStart + (i + 1) * 2 + 1);
                mesh.indices.push_back(capStart + i * 2 + 1);
            }

            mesh.calculateBounds();
        }
    };

} // namespace libre