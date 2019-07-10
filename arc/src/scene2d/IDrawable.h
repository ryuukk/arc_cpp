#pragma once

#include "../gfx/SpriteBatch.h"

namespace arc
{
    class IDrawable
    {
    public:
        virtual void draw(SpriteBatch* batch, float x, float y, float width, float height) = 0;
        virtual float getLeftWidth() = 0;
        virtual void setLeftWidth(float leftWidth) = 0;
        virtual float getRightWidth() = 0;
        virtual void setRightWidth(float rightWidth) = 0;
        virtual float getTopHeight() = 0;
        virtual void setTopHeight(float topHeight) = 0;
        virtual float getBottomHeight() = 0;
        virtual void setBottomHeight(float bottomHeight) = 0;
        virtual float getMinWidth() = 0;
        virtual void setMinWidth(float minWidth) = 0;
        virtual float getMinHeight() = 0;
        virtual void setMinHeight(float minHeight) = 0;
    };
}