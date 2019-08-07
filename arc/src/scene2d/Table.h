#pragma once

#include "WidgetGroup.h"
#include "IDrawable.h"
#include "Skin.h"
#include "Value.h"
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


    class Table : public WidgetGroup
    {
    public:
        static NewValue zero;
        static NewValue minWidth;
        static NewValue maxWidth;
        static NewValue prefWidth;
        static NewValue minHeight;
        static NewValue maxHeight;
        static NewValue prefHeight;

        static NewValue backgroundTop;
        static NewValue backgroundLeft;
        static NewValue backgroundBottom;
        static NewValue backgroundRight;
    private:
        static std::vector<float> columnWeightedWidth;
        static std::vector<float> rowWeightedHeight;
        static DynamicObjectPool<Cell> pool;

    public:
        Table(Skin* skin = nullptr);
        ~Table() override;

        void draw(SpriteBatch* batch, float parentAlpha) override;
        void drawBackground(SpriteBatch* batch, float parentAlpha, float x, float y);
        Actor* hit(float x, float y, bool touchable) override;
        void setClip(bool enabled);
        bool getClip();
        void invalidate() override;
        Cell& add(Actor* actor);
        Cell& row();
        void setBackground(IDrawable* background);
        IDrawable* getBackground();
        void clearChildren() override;
        void reset();
        Table& debug(TableDebug debug);

        float getPadTop();
        float getPadLeft();
        float getPadBottom();
        float getPadRight();

        float getMinWidth() override;
        float getMinHeight() override;
        float getPrefWidth() override;
        float getPrefHeight() override;

        void layout() override;
    private:
        void clearDebugRects();
        void endRow();
        void computeSize();
        void layout(float layoutX, float layoutY, float layoutWidth, float layoutHeight);
    private:
        int columns{};
        int rows{};
        bool implicitEndRow{};

        std::vector<Cell*> cells;
        Cell* cellDefaults = nullptr;
        std::vector<Cell*> columnDefaults;
        Cell* rowDefaults = nullptr;

        bool sizeInvalid = true;
        std::vector<float> columnMinWidth{};
        std::vector<float> rowMinHeight{};

        std::vector<float> columnPrefWidth{};
        std::vector<float> rowPrefHeight{};

        float tableMinWidth{};
        float tableMinHeight{};
        float tablePrefWidth{};
        float tablePrefHeight{};

        std::vector<float> columnWidth{};
        std::vector<float> rowHeight{};
        std::vector<float> expandWidth{};
        std::vector<float> expandHeight{};

        NewValue& padTop = backgroundTop;
        NewValue& padLeft = backgroundLeft;
        NewValue& padBottom = backgroundBottom;
        NewValue& padRight = backgroundRight;

        int align = (int)Align::center;

        TableDebug _debug = TableDebug::none;
        std::vector<DebugRect> debugRects;

        IDrawable* background = nullptr;
        bool clip = false;
        Skin* skin = nullptr;
        bool round = true;
    };
}


