#pragma once

#include <optional>

namespace arc
{
    class Value;
    class Actor;
    class Table;
    class Cell
    {
    private:
        static Cell* _defaults;
    public:
        Actor* actor = nullptr;

        Cell();
        void setLayout(Table* table);
        void set(Cell* cell);
        void merge(Cell* cell);
        void reset();
        void clear();
        Cell* defaults();

        Cell& minSize(Value* value);
        Cell& minSize(float size);

        // todo: switch to private
    public:
        Value* minWidth = nullptr;
        Value* minHeight = nullptr;
        Value* prefWidth = nullptr;
        Value* prefHeight = nullptr;
        Value* maxWidth = nullptr;
        Value* maxHeight = nullptr;
        Value* spaceTop = nullptr;
        Value* spaceLeft = nullptr;
        Value* spaceBottom = nullptr;
        Value* spaceRight = nullptr;
        Value* padTop = nullptr;
        Value* padLeft = nullptr;
        Value* padBottom = nullptr;
        Value* padRight = nullptr;
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


