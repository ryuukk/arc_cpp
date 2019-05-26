#pragma once

namespace arc
{
    enum class Align : int
    {
        center = 1 << 0,
        top = 1 << 1,
        bottom = 1 << 2,
        left = 1 << 3,
        right = 1 << 4,

        topLeft = top | left,
        topRight = top | right,
        bottomLeft = bottom | left,
        bottomRight = bottom | right
    };
    
    namespace align
    {
        inline static bool isLeft (int align) {
            return (align & (int) arc::Align::left) != 0;
        }

        inline static bool isRight (int align) {
            return (align & (int) arc::Align::right) != 0;
        }

        inline static bool isTop (int align) {
            return (align & (int) arc::Align::top) != 0;
        }

        inline static bool isBottom (int align) {
            return (align & (int) arc::Align::bottom) != 0;
        }

        inline static bool isCenterVertical (int align) {
            return (align & (int) arc::Align::top) == 0 && (align & (int) arc::Align::bottom) == 0;
        }

        inline static bool isCenterHorizontal (int align) {
            return (align & (int) arc::Align::left) == 0 && (align & (int) arc::Align::right) == 0;
        }

    }
}