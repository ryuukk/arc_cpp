#pragma once

#include "Group.h"
#include "ILayout.h"

namespace arc
{
    class WidgetGroup : public Group, public ILayout
    {
    public:
        void layout() override;
        void invalidate() override;
        void invalidateHierarchy() override;
        void validate() override;
        void pack() override;
        void setFillParent(bool fillParent) override;
        void setLayoutEnabled(bool enabled) override;
        float getMinWidth() override;
        float getMinHeight() override;
        float getPrefWidth() override;
        float getPrefHeight() override;
        float getMaxWidth() override;
        float getMaxHeight() override;

    public:
        bool needsLayout();
        void draw(SpriteBatch* batch, float parentAlpha);

    private:
        void setLayoutEnabled(Group* parent, bool enabled);

    protected:
        void childrenChanged();
        void sizeChanged();

    private:
        bool _needsLayout = true;
        bool _fillParent = false;
        bool _layoutEnabled = true;
    };
}


