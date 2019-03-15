#pragma once

#include <string>
#include <vector>
#include "ModelNodeKeyframe.h"
#include "../../math/Vec3.h"
#include "../../math/Quat.h"

namespace arc
{
    class ModelNodeAnimation
    {
    public:
        std::string nodeId;
        std::vector<ModelNodeKeyframe<Vec3>> translation;
        std::vector<ModelNodeKeyframe<Quat>> rotation;
        std::vector<ModelNodeKeyframe<Vec3>> scaling;
    };
}

