#pragma once

#include <cstdint>

namespace arc
{
    struct Color
    {
        union Stuff
        {
            uint32_t packed;
            float floatBits;
        };

        static Color BLACK;
        static Color WHITE;
        static Color RED;
        static Color GREEN;
        static Color BLUE;

        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        Color() = default;
        Color(const uint32_t& value)
        {
            r = (uint8_t)((value & 0xff000000U) >> 24U);
            g = (uint8_t)((value & 0x00ff0000U) >> 16U);
            b = (uint8_t)((value & 0x0000ff00U) >> 8U);
            a = (uint8_t)((value & 0x000000ffU));
        }

        Color operator * (const Color& v) const
        {
            Color ret{};
            ret.r = r * v.r;
            ret.g = g * v.g;
            ret.b = b * v.b;
            ret.a = a * v.a;
            return ret;
        }

        Color& operator *= (const Color& rhs)
        {
            r *= rhs.r;
            g *= rhs.g;
            b *= rhs.b;
            a *= rhs.a;
            return *this;
        }

        float toFloatBits()
        {
            auto s = Stuff();
            s.packed = (uint32_t)((r << 24U) | (g << 16U) | (b << 8U) | (a));
            return s.floatBits;
        }
    };
}
