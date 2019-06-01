#pragma once

#include "../math/Vec2.h"

namespace arc
{
    enum class Scaling
    {
        fit,
        fill,
        fillX,
        fillY,
        stretch,
        stretchX,
        stretchY,
        none,
    };

    namespace scaling
    {
        inline static Vec2 apply(arc::Scaling scaling,float sourceWidth, float sourceHeight, float targetWidth, float targetHeight) {
            Vec2 temp;
            switch (scaling)
            {
                case arc::Scaling::fit: {
                    float targetRatio = targetHeight / targetWidth;
                    float sourceRatio = sourceHeight / sourceWidth;
                    float scale = targetRatio > sourceRatio ? targetWidth / sourceWidth : targetHeight / sourceHeight;
                    temp.x = sourceWidth * scale;
                    temp.y = sourceHeight * scale;
                    break;
                }
                case arc::Scaling::fill: {
                    float targetRatio = targetHeight / targetWidth;
                    float sourceRatio = sourceHeight / sourceWidth;
                    float scale = targetRatio < sourceRatio ? targetWidth / sourceWidth : targetHeight / sourceHeight;
                    temp.x = sourceWidth * scale;
                    temp.y = sourceHeight * scale;
                    break;
                }
                case arc::Scaling::fillX: {
                    float scale = targetWidth / sourceWidth;
                    temp.x = sourceWidth * scale;
                    temp.y = sourceHeight * scale;
                    break;
                }
                case arc::Scaling::fillY: {
                    float scale = targetHeight / sourceHeight;
                    temp.x = sourceWidth * scale;
                    temp.y = sourceHeight * scale;
                    break;
                }
                case arc::Scaling::stretch:
                    temp.x = targetWidth;
                    temp.y = targetHeight;
                    break;
                case arc::Scaling::stretchX:
                    temp.x = targetWidth;
                    temp.y = sourceHeight;
                    break;
                case arc::Scaling::stretchY:
                    temp.x = sourceWidth;
                    temp.y = targetHeight;
                    break;
                case arc::Scaling::none:
                    temp.x = sourceWidth;
                    temp.y = sourceHeight;
                    break;
            }
            return temp;
        }
    }
}