#pragma once

#include <vector>
#include <string>
#include "../../math/Vec3.h"
#include "../../math/Quat.h"
#include "ModelNodePart.h"

namespace arc
{
    class ModelNode
    {
    public:
        std::string id;
        Vec3 translation{0,0,0};
        Quat rotation = Quat::identity();
        Vec3 scale{1, 1, 1};
        std::string meshId;
        std::vector<ModelNodePart> parts;
        std::vector<ModelNode> children;
    };
}

