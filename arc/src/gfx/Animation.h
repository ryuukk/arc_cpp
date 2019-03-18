#pragma once

#include <string>
#include <vector>
#include "nodes/NodeAnimation.h"

namespace arc
{
    class Animation
    {
    public:
        std::string id;
        float duration{};
        std::vector<NodeAnimation*> nodeAnimations;
    };
}


