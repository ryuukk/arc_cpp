#pragma once

#include <string>
#include "Mesh.h"

namespace arc
{
    class MeshPart
    {
    public:
        std::string id;
        int primitiveType{};
        int offset{};
        int size{};
        Mesh* mesh = nullptr;

        Vec3 center{};
        Vec3 halfExtents{};
        float radius = 1.0f;
    };
}

