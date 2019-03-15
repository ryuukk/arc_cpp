#pragma once

#include <string>
#include "ModelNodeAnimation.h"

namespace arc
{
    class ModelAnimation
    {
    public:
        std::string id;
        std::vector<ModelNodeAnimation> nodeAnimations;
    };
}



