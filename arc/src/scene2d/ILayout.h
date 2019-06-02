#pragma once

namespace arc
{
    class ILayout
    {
    public:
        virtual void layout() = 0;
        virtual void invalidate() = 0;
        virtual void invalidateHierarchy() = 0;
        virtual void validate() = 0;
        virtual void pack() = 0;
        virtual void setFillParent(bool fillParent) = 0;
        virtual void setLayoutEnabled(bool enabled) = 0;
        virtual float getMinWidth() = 0;
        virtual float getMinHeight() = 0;
        virtual float getPrefWidth() = 0;
        virtual float getPrefHeight() = 0;
        virtual float getMaxWidth() = 0;
        virtual float getMaxHeight() = 0;
    };
}