#pragma once

#include <vector>
#include <utility>
#include "../MeshPart.h"
#include "../Material.h"

namespace arc
{
    class Node;

    class NodePart
    {
    public:
        MeshPart* meshPart;
        Material* material;

        std::vector<std::pair<Node*, Mat4>> invBoneTransforms;
        std::vector<Mat4> bones;
        bool enabled = true;

        void set(NodePart* other) {

        }

        NodePart* copy() {
            auto* ret = new NodePart();
            ret->set(this);
            return ret;
        }
    };

}