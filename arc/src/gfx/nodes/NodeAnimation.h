#pragma once

#include <vector>
#include "Node.h"
#include "../../math/Vec3.h"
#include "../../math/Quat.h"

namespace arc
{
    template<typename T>
    struct NodeKeyframe
    {
        float keytime{};
        T value;
    };

    class NodeAnimation
    {
    public:
        Node* node = nullptr;
        std::vector<NodeKeyframe<Vec3>> translation;
        std::vector<NodeKeyframe<Quat>> rotation;
        std::vector<NodeKeyframe<Vec3>> scaling;
    };
}