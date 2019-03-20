#pragma once

#include "BaseAnimationController.h"

namespace arc
{

    struct AnimationDesc
    {
        Animation* animation = nullptr;
        float speed{};
        float time{};
        float offset{};
        float duration{};
        int loopCount{};

        float update(float delta)
        {
            if(loopCount != 0 && animation != nullptr)
            {
                int loops;
                float diff = speed * delta;
                if(duration != 0.0f)
                {
                    time += diff;
                    loops = (int) std::fabs(time / duration);
                    if(time < 0.0f)
                    {
                        loops++;
                        while(time < 0.0f)
                            time += duration;
                    }
                    time = std::fabs( fmodf(time,duration) );
                } else loops = 1;

                for (int i = 0; i < loops; ++i) {
                    if(loopCount > 0) loopCount--;
                    // if(loopCount != 0) && listener != nullptr) listener.onLoop(this); // todo: figure out callbacks
                    if(loopCount == 0)
                    {
                        float result = ((loops - 1) - i) * duration + (diff < 0.0f ? duration - time : time);
                        time = (diff < 0.0f) ? 0.0f : duration;
                        // if (listener != nullptr) listener.onEnd(this); // todo: figure out callbacks
                        return result;
                    }
                }
                return 0.0f;
            } else
                return delta;
        }
    };

    class AnimationController : public BaseAnimationController
    {
    public:
        AnimationController(ModelInstance& target) : BaseAnimationController(target) {}
        ~AnimationController()
        {
            delete current;
            delete queued;
            delete previous;
        };
        AnimationDesc* current = nullptr;
        AnimationDesc* queued = nullptr;
        float queuedTransitionTime{};
        AnimationDesc* previous = nullptr;
        float transitionCurrentTime{};
        float transitionTargetTime{};
        bool inAction{};
        bool paused{};
        bool allowSameAnimation{};

        void update(float delta);

        AnimationDesc* animate(const std::string& id, float offset = 0.0f, float duration = -1.0f, int loopCount = -1, float speed = 1,
                               float transitionTime = 0.0f);

    protected:
        AnimationDesc& animate(AnimationDesc& anim, float transitionTime);
        AnimationDesc& queue(AnimationDesc& anim, float transitionTime);

    private:
        bool justChangedAnimation = false;
    };
}


