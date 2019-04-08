#include <cassert>
#include "BaseAnimationController.h"

arc::BaseAnimationController::TransformMap arc::BaseAnimationController::transforms{};

arc::BaseAnimationController::BaseAnimationController(arc::ModelInstance& target) : target(target) {

}

void arc::BaseAnimationController::begin() {
    assert(!_applying);
    _applying = true;
}

void arc::BaseAnimationController::apply(arc::Animation& animation, float time, float weight) {
    assert(!_applying);
    applyAnimation(&transforms, weight, animation, time);
}

void arc::BaseAnimationController::end() {
    assert(_applying);
    transforms.clear();
    target.calculateTransforms();
    _applying = false;
}

void arc::BaseAnimationController::applyAnimation(arc::Animation& animation, float time) {
    assert(!_applying);

    applyAnimation(nullptr, 1.0f, animation, time);

    target.calculateTransforms();
}

void arc::BaseAnimationController::removeAnimation(arc::Animation& animation){
    for(auto& nodeAnim : animation.nodeAnimations)
        nodeAnim->node->isAnimated = false;
}

// STATIC STUFF

template<typename T>
inline int arc::BaseAnimationController::getFirstKeyframeIndexAtTime(std::vector<arc::NodeKeyframe<T>> arr, float time) {
    for (int i = 0; i < arr.size() - 1; ++i) {
        if(time > arr[i].keytime && time <= arr[i + 1].keytime)
            return i;
    }
    return 0;
}

inline arc::Vec3 arc::BaseAnimationController::getTranslationAtTime(arc::NodeAnimation& nodeAnim, float time) {

    if(nodeAnim.translation.empty()) return nodeAnim.node->translation;
    if(nodeAnim.translation.size() == 1) return nodeAnim.translation[0].value;

    int index = getFirstKeyframeIndexAtTime(nodeAnim.translation, time);
    auto& firstKeyframe = nodeAnim.translation[index];

    auto out = firstKeyframe.value;

    if(++index < nodeAnim.translation.size())
    {
        //auto& secondKeyframe = nodeAnim.translation[index];
        //float t = (time - firstKeyframe.keytime) / (secondKeyframe.keytime - firstKeyframe.keytime);
        // todo: lerp
    }

    return out;
}

inline arc::Quat arc::BaseAnimationController::getRotationAtTime(arc::NodeAnimation& nodeAnim, float time) {

    if(nodeAnim.rotation.empty()) return nodeAnim.node->rotation;
    if(nodeAnim.rotation.size() == 1) return nodeAnim.rotation[0].value;

    int index = getFirstKeyframeIndexAtTime(nodeAnim.rotation, time);
    auto& firstKeyframe = nodeAnim.rotation[index];

    auto out = firstKeyframe.value;

    if(++index < nodeAnim.rotation.size())
    {
        //auto& secondKeyframe = nodeAnim.rotation[index];
        //float t = (time - firstKeyframe.keytime) / (secondKeyframe.keytime - firstKeyframe.keytime);
        // todo: lerp
    }

    return out;
}

inline arc::Vec3 arc::BaseAnimationController::getScalingAtTime(arc::NodeAnimation& nodeAnim, float time) {

    if(nodeAnim.scaling.empty()) return nodeAnim.node->scale;
    if(nodeAnim.scaling.size() == 1) return nodeAnim.scaling[0].value;

    int index = getFirstKeyframeIndexAtTime(nodeAnim.scaling, time);
    auto& firstKeyframe = nodeAnim.scaling[index];

    auto out = firstKeyframe.value;

    if(++index < nodeAnim.scaling.size())
    {
        //auto& secondKeyframe = nodeAnim.scaling[index];
        //float t = (time - firstKeyframe.keytime) / (secondKeyframe.keytime - firstKeyframe.keytime);
        // todo: lerp
    }

    return out;
}

inline arc::Transform arc::BaseAnimationController::getNodeAnimationTransform(arc::NodeAnimation& nodeAnim, float time) {
    auto transform = Transform::idt();
    transform.translation = getTranslationAtTime(nodeAnim, time);
    transform.rotation = getRotationAtTime(nodeAnim, time);
    transform.scale = getScalingAtTime(nodeAnim, time);
    return transform;
}

void arc::BaseAnimationController::applyNodeAnimationDirectly(arc::NodeAnimation& nodeAnim, float time) {
    auto* node = nodeAnim.node;
    node->isAnimated = true;
    auto transform = getNodeAnimationTransform(nodeAnim, time);
    node->localTransform = Transform::toMat4(transform);
}

void arc::BaseAnimationController::applyNodeAnimationDirectly(arc::NodeAnimation& nodeAnim,
                                                          std::unordered_map<arc::Node*, arc::Transform>* out,
                                                          float alpha, float time) {
    auto* node = nodeAnim.node;
    node->isAnimated = true;
    auto transform = getNodeAnimationTransform(nodeAnim, time);

    // todo: finish blending
}

void arc::BaseAnimationController::applyAnimation(std::unordered_map<arc::Node*, arc::Transform>* out, float alpha,
                                              arc::Animation& animation, float time) {
    if(out == nullptr)
    {
        for(auto& nodeAnim : animation.nodeAnimations)
            applyNodeAnimationDirectly(*nodeAnim, time);
    } else
    {
        // todo: finish blending
    }
}
