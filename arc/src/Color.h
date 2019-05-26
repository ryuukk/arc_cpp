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

        uint8_t r = 255;
        uint8_t g = 255;
        uint8_t b = 255;
        uint8_t a = 255;

        Color() = default;
        Color(uint32_t value)
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

        static Color fromRGB888(uint32_t value)
        {
            Color color{};
            color.r = ((value & 0x00ff0000u) >> 16u);
            color.g = ((value & 0x0000ff00u) >> 8u);
            color.b = ((value & 0x000000ffu));
            color.a = 255;
            return color;
        }

        float toFloatBits()
        {
            uint32_t color = ((uint32_t) (a) << 24u) |
                    ((uint32_t) (b) << 16u) |
                    ((uint32_t) (g) << 8u) |
                    ((uint32_t) (r));

            Stuff s{};
            s.packed = color;
            return s.floatBits;
        }
    };
}
