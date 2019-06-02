#pragma once

#include "WidgetGroup.h"
#include "IDrawable.h"
#include "Skin.h"
#include "Cell.h"
#include "../gfx/Align.h"
#include "../utils/object_pool.hpp"

namespace arc
{
    enum class TableDebug
    {
        none, all, table, cell, actor
    };

    struct DebugRect
    {
        Rect rect;
        Color color;
    };

    class Value
    {
    public:
        virtual float get(Actor* context) = 0;
    };

    class FixedValue : public Value
    {
    public:
        FixedValue(float value);
        float get(Actor* context) override;
    private:
        float _value{};
    };

    class BackgroundTopValue : public Value
    {
    public:
        float get(Actor* context) override;
    };
    class BackgroundBottomValue : public Value
    {
    public:
        float get(Actor* context) override;
    };
    class BackgroundLeftValue : public Value
    {
    public:
        float get(Actor* context) override;
    };
    class BackgroundRightValue : public Value
    {
    public:
        float get(Actor* context) override;
    };
    class MinWidthValue: public Value
    {
    public:
        float get(Actor* context) override;
    };
    class MinHeightValue: public Value
    {
    public:
        float get(Actor* context) override;
    };
    class PrefWidthValue: public Value
    {
    public:
        float get(Actor* context) override;
    };
    class PrefHeightValue: public Value
    {
    public:
        float get(Actor* context) override;
    };
    class MaxWidthValue: public Value
    {
    public:
        float get(Actor* context) override;
    };
    class MaxHeightValue: public Value
    {
    public:
        float get(Actor* context) override;
    };


    class Table : public WidgetGroup
    {
    public:
        Table(Skin* skin = nullptr);
        ~Table();

        void draw(SpriteBatch* batch, float parentAlpha);
        void drawBackground(SpriteBatch* batch, float parentAlpha, float x, float y);
        Actor* hit(float x, float y, bool touchable);
        void setClip(bool enabled);
        bool getClip();
        void invalidate();
        Cell& add(Actor* actor);
        IDrawable* getBackground();
    private:
        int columns{};
        int rows{};
        bool implicitEndRow{};

        std::vector<Cell*> cells;
        Cell cellDefaults{};
        std::vector<Cell*> columnDefaults;
        Cell rowDefaults{};

        bool sizeInvalid = true;
        std::vector<float> columnMinWidth;
        std::vector<float> rowMinHeight;
        std::vector<float> columnPrefWidth;
        std::vector<float> rowPrefHeight;
        std::vector<float> tableMinWidth;
        std::vector<float> tableMinHeight;
        std::vector<float> tablePrefWidth;
        std::vector<float> tablePrefHeight;
        std::vector<float> columnWidth;
        std::vector<float> rowHeight;
        std::vector<float> expandWidth;
        std::vector<float> expandHeight;

        Value* padTop;
        Value* padLeft;
        Value* padBottom;
        Value* padRight;
        int align = (int)Align::center;

        TableDebug debug = TableDebug::none;
        std::vector<DebugRect> debugRects;

        IDrawable* background = nullptr;
        bool clip = false;
        Skin* skin = nullptr;
        bool round = true;
        DynamicObjectPool<Cell> pool{64};
    };
}


