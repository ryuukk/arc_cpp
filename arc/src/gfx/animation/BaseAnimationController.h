#pragma once

#include <string>
#include <unordered_map>
#include "../ModelInstance.h"
#include "../../math/Transform.h"

namespace arc
{
    class BaseAnimationController
    {
    public:
        typedef std::unordered_map<Node*, Transform> TransformMap;
        static TransformMap transforms;

        template <typename T>
        inline static int getFirstKeyframeIndexAtTime(std::vector<NodeKeyframe<T>> arr, float time);

        inline static Vec3 getTranslationAtTime(NodeAnimation& nodeAnim, float time);
        inline static Quat getRotationAtTime(NodeAnimation& nodeAnim, float time);
        inline static Vec3 getScalingAtTime(NodeAnimation& nodeAnim, float time);

        inline static Transform getNodeAnimationTransform(NodeAnimation& nodeAnim, float time);

        static void applyNodeAnimationDirectly(NodeAnimation& nodeAnim, float time);
        static void applyNodeAnimationDirectly(NodeAnimation& nodeAnim, std::unordered_map<Node*, Transform>* out, float alpha, float time);

        static void applyAnimation(std::unordered_map<Node*, Transform>* out, float alpha, Animation& animation, float time);

    public:
        BaseAnimationController(ModelInstance& target);
        ModelInstance& target;

    protected:
        void begin();
        void apply(Animation& animation, float time, float weight);
        void end();

        void applyAnimation(Animation& animation, float time);
        void removeAnimation(Animation& animation);

    private:
        bool _applying = false;
    };
}


