#pragma once


namespace arc
{
    class Actor;

    class Table;

    class Cell
    {
    public:
        Actor* actor = nullptr;

        void setLayout(Table* table);

        // todo: switch to private
    public:
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


