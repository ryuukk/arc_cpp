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

        ~MeshPart()
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

        void set(const std::string& id, Mesh* mesh, int offset, int size, int type) {
            this->id = id;
            this->mesh = mesh;
            this->offset = offset;
            this->size = size;
            this->primitiveType = type;
            this->center = {0, 0, 0};
            this->halfExtents = {0, 0, 0};
            this->radius = -1.0;
        }


        void render(ShaderProgram* shader, bool autoBind)
        {
            mesh->render(shader, primitiveType, offset, size, autoBind);
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

