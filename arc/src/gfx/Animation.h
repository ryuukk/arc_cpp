#pragma once

#include <string>
#include <vector>
#include "nodes/NodeAnimation.h"

namespace arc
{
    class Animation
    {
    public:
        ~Animation()
        {
            for(auto& nanim : nodeAnimations)
                delete nanim;
        }
        std::string id;
        float duration{};
        std::vector<NodeAnimation*> nodeAnimations;
    };
}


