#pragma once

#include <string>
#include <vector>
#include "../../math/Mat4.h"

namespace arc
{
    class ModelNodePart
    {
    public:
        std::string materialId;
        std::string meshPartId;
        std::vector<std::pair<std::string, Mat4>> bones;
        //int[][] uvMapping;
    };
}
