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
        ~NodePart()
        {
            // delete meshPart;
        }

        MeshPart* meshPart = nullptr;
        Material* material = nullptr;

        std::vector<std::pair<Node*, Mat4>> invBoneTransforms;
        std::vector<Mat4> bones;
        bool enabled = true;

        void set(NodePart* other) {
            meshPart = new MeshPart(other->meshPart);
            material = other->material;
            enabled = other->enabled;

            if(!other->invBoneTransforms.empty())
            {
                invBoneTransforms.resize(other->invBoneTransforms.size());
                bones.resize(other->invBoneTransforms.size());
                for (auto i = 0; i < other->invBoneTransforms.size(); ++i) {
                    auto& entry = other->invBoneTransforms[i];
                    invBoneTransforms[i] = {entry.first, entry.second};
                }

                for (auto j = 0; j < bones.size(); ++j) {
                    bones[j] = Mat4::identity();
                }
            }
        }

        NodePart* copy() {
            auto* ret = new NodePart();
            ret->set(this);
            return ret;
        }
    };

}