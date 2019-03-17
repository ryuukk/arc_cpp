#pragma once

#include <string>
#include "Mesh.h"

namespace arc
{
    class MeshPart
    {
    public:
        MeshPart()
        {};
        MeshPart(MeshPart* other) {
            set(other);
        };

        void set(MeshPart* other) {
            this->id = other->id;
            this->mesh = other->mesh;
            this->offset = other->offset;
            this->size = other->size;
            this->primitiveType = other->primitiveType;
            this->center = other->center;
            this->halfExtents = other->halfExtents;
            this->radius = other->radius;
        }

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

