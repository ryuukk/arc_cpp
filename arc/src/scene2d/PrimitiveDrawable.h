#pragma once

#include "IDrawable.h"
#include "../Color.h"

namespace arc
{
    class PrimitiveDrawable : public IDrawable
    {
    public:
        PrimitiveDrawable(const Color& color, float minWidth, float minHeight);
        void draw(SpriteBatch* batch, float x, float y, float width, float height) override;

        float getLeftWidth() override;

        void setLeftWidth(float leftWidth) override;

        float getRightWidth() override;

        void setRightWidth(float rightWidth) override;

        float getTopHeight() override;

        void setTopHeight(float topHeight) override;

        float getBottomHeight() override;

        void setBottomHeight(float bottomHeight) override;

        float getMinWidth() override;

        void setMinWidth(float minWidth) override;

        float getMinHeight() override;

        void setMinHeight(float minHeight) override;

    private:
        Color _color = Color::WHITE;
        float _minWidth{};
        float _minHeight{};
        float _leftWidth{};
        float _rightWidth{};
        float _topHeight{};
        float _bottomHeight{};
    };
}


