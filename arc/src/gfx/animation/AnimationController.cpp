#include "AnimationController.h"

void arc::AnimationController::update(float delta)
{
    if (paused) return;
    if (previous != nullptr && ((transitionCurrentTime += delta) >= transitionTargetTime)) {
        removeAnimation(*previous->animation);
        justChangedAnimation = true;

        // pool ?
        delete previous;
        previous = nullptr;
    }
    if (justChangedAnimation) {
        target.calculateTransforms();
        justChangedAnimation = false;
    }
    if (current == nullptr || current->loopCount == 0 || current->animation == nullptr) return;
    float remain = current->update(delta);
    if (remain != 0.0f && queued != nullptr) {
        inAction = false;
        animate(*queued, queuedTransitionTime);
        queued = nullptr;
        update(remain);
        return;
    }
    if (previous != nullptr)
    {
        // todo: finish blending
        //applyAnimations(previous->animation, previous->offset + previous->time, current->animation, current->offset + current->time, transitionCurrentTime / transitionTargetTime);
    }
    else
        applyAnimation(*current->animation, current->offset + current->time);

}

arc::AnimationDesc& arc::AnimationController::animate(arc::AnimationDesc& anim, float transitionTime) {
    if (current == nullptr)
        current = &anim;
    else if (inAction)
        queue(anim, transitionTime);
    else if (!allowSameAnimation  && current->animation == anim.animation) {
        anim.time = current->time;

        // pool ?
        delete current;

        current = &anim;
    } else {
        if (previous != nullptr) {
            removeAnimation(*previous->animation);

            // pool ?
            delete previous;
        }
        previous = current;
        current = &anim;
        transitionCurrentTime = 0.0f;
        transitionTargetTime = transitionTime;
    }
    return anim;
}

arc::AnimationDesc& arc::AnimationController::queue(arc::AnimationDesc& anim, float transitionTime) {
    if (current == nullptr || current->loopCount == 0)
        animate(anim, transitionTime);
    else {
        if (queued != nullptr)
        {
            // pool ?
            delete queued;
        }
        queued = &anim;
        queuedTransitionTime = transitionTime;
        if (current->loopCount < 0) current->loopCount = 1;
    }
    return anim;
}

arc::AnimationDesc*
arc::AnimationController::animate(const std::string& id, float offset, float duration, int loopCount, float speed,
                                  float transitionTime) {

    auto* animation = target.getAnimation(id);
    if(animation == nullptr)
        return nullptr;

    auto* desc = new AnimationDesc;
    desc->animation = animation;
    desc->loopCount = loopCount;
    desc->speed = speed;
    desc->offset = offset;
    desc->duration = duration < 0 ? (animation->duration - offset) : duration;
    desc->time = speed < 0 ? desc->duration : 0.0f;

    animate(*desc, transitionTime);

    return desc;
}
