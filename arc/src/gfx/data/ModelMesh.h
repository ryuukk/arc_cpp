#pragma once

#include <vector>
#include <string>
#include "../VertexAttributes.h"
#include "ModelMeshPart.h"

namespace arc
{
    class ModelMesh
    {
    public:
        std::string id;
        std::vector<VertexAttribute> attributes;
        std::vector<float> vertices;
        std::vector<ModelMeshPart> parts;
    };
}

