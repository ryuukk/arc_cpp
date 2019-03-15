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
        Vec3 translation = Vec3();
        Quat rotation = Quat::identity();
        Vec3 scale = Vec3(1, 1, 1);
        std::string meshId;
        std::vector<ModelNodePart*> parts;
        std::vector<ModelNode*> children;
    };
}

