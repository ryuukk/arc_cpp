#pragma once


namespace arc
{
    enum class ValueType
    {
        Fixed,
        BackgroundTop,
        BackgroundBottom,
        BackgroundLeft,
        BackgroundRight,
        MinWidth,
        MinHeight,
        PrefWidth,
        PrefHeight,
        MaxWidth,
        MaxHeight
    };

    class Table;
    class Actor;

    struct NewValue
    {
    public:
        static NewValue fixed(float value);

        static NewValue backgroundTop();

        static NewValue backgroundBottom();

        static NewValue backgroundLeft();

        static NewValue backgroundRight();

        static NewValue minWidth();

        static NewValue minHeight();

        static NewValue prefWidth();

        static NewValue prefHeight();

        static NewValue maxWidth();

        static NewValue maxHeight();

    public:
        NewValue();
        NewValue(const NewValue& a) : type(a.type), value(a.value) {};
        ~NewValue() {}
        ValueType type{};
        float value{};

        float get(Actor* context);
    };
}