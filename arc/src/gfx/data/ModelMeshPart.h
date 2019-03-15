#pragma once

#include <vector>
#include <string>

namespace arc
{
    class ModelMeshPart
    {
    public:
        std::string id;
        std::vector<short> indices;
        int primitiveType;
    };
}



