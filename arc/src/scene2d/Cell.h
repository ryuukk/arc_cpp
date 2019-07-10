#pragma once

#include "Value.h"
#include <optional>

namespace arc
{
    class NewValue;
    class Actor;
    class Table;
    class Cell
    {
    public:
        Actor* actor = nullptr;

        Cell();
        void setLayout(Table* table);
        void set(Cell* cell);
        void merge(Cell* cell);
        void reset();
        void clear();
        void defaults();

        Cell& minSize(const NewValue& value);
        Cell& minSize(float size);

        Cell& pad(float value);
        Cell& pad(const NewValue& value);

        void setActorBounds(float x, float y, float width, float height);
        void setActorX(float x);
        void setActorY(float y);


        // todo: switch to private
    public:
        std::optional<NewValue> minWidth{};
        std::optional<NewValue> minHeight{};
        std::optional<NewValue> prefWidth{};
        std::optional<NewValue> prefHeight{};
        std::optional<NewValue> maxWidth{};
        std::optional<NewValue> maxHeight{};
        std::optional<NewValue> spaceTop{};
        std::optional<NewValue> spaceLeft{};
        std::optional<NewValue> spaceBottom{};
        std::optional<NewValue> spaceRight{};
        std::optional<NewValue> padTop{};
        std::optional<NewValue> padLeft{};
        std::optional<NewValue> padBottom{};
        std::optional<NewValue> padRight{};
        std::optional<float> fillX{};
        std::optional<float> fillY{};
        std::optional<int> align{};
        std::optional<int> expandX{};
        std::optional<int> expandY{};
        std::optional<int> colspan{};
        std::optional<bool> uniformX{};
        std::optional<bool> uniformY{};

        float actorX{};
        float actorY{};
        float actorWidth{};
        float actorHeight{};

        Table* table = nullptr;
        bool endRow{};
        int column{};
        int row{};
        int cellAboveIndex{};
        float computedPadTop{};
        float computedPadLeft{};
        float computedPadBottom{};
        float computedPadRight{};
    };
}


