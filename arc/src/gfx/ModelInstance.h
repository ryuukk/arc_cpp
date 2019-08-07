#pragma once

#include "Model.h"
#include "rendering/Renderable.h"

namespace arc
{
    class ModelInstance : public IRenderableProvider
    {
    public:
        static bool defaultShareKeyframes;
    public:
        explicit ModelInstance(Model& model);
        ~ModelInstance()
        {
            for (auto* mat : materials)
                delete mat;
            for (auto* node : nodes)
                delete node;
            for (auto* anim : animations)
                delete anim;
        }

        std::vector<Material*> materials;
        std::vector<Node*> nodes;
        std::vector<Animation*> animations;
        Model& model;
        Mat4 transform = Mat4::identity();

        void calculateTransforms();
        Animation* getAnimation(const std::string& id);

    private:
        void copyNodes(std::vector<Node*>& nodes);
        void copyAnimations(std::vector<Animation*>& animations, bool shareKeyframes = false);

        void invalidate();
        void invalidate(Node* node);

        void getRenderables(DynamicObjectPool<Renderable>& pool, std::vector<Renderable*>& renderables) override;
    };
}


