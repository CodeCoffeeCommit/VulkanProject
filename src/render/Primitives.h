#pragma once

#include "Mesh.h"

class VulkanContext;

namespace Primitives {
    Mesh* createCube(VulkanContext* context, float size = 1.0f);
    Mesh* createSphere(VulkanContext* context, float radius = 1.0f, int segments = 32, int rings = 16);
    Mesh* createPlane(VulkanContext* context, float size = 1.0f, int subdivisions = 1);
}