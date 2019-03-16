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

        Vec3 center{0,0,0};
        Vec3 halfExtents{0,0,0};
        float radius = 1.0f;
    };
}

