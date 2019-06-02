#pragma once

namespace arc
{
    struct Rect
    {
        float x{};
        float y{};
        float width{};
        float height{};

        Rect()
        {}
        Rect(float x, float y, float width, float height): x(x), y(y), width(width), height(height)
        {}

        bool isZero();
    };
}