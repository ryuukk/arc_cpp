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

        float r = 1.0f;
        float g = 1.0f;
        float b = 1.0f;
        float a = 1.0f;

        Color() = default;
        Color(uint32_t value)
        {
            r = ((value & 0xff000000U) >> 24U) / 255.0f;
            g = ((value & 0x00ff0000U) >> 16U) / 255.0f;
            b = ((value & 0x0000ff00U) >> 8U) / 255.0f;
            a = ((value & 0x000000ffU)) / 255.0f;
        }

        Color(float r, float g, float b, float a): r(r), g(g), b(b), a(a)
        {}

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
            color.r = ((value & 0x00ff0000u) >> 16u) / 255.0f;
            color.g = ((value & 0x0000ff00u) >> 8u) / 255.0f;
            color.b = ((value & 0x000000ffu)) / 255.0f;
            color.a = 1.0f;
            return color;
        }

        float toFloatBits()
        {
            uint32_t color = ((uint32_t) (a * 255) << 24u) |
                    ((uint32_t) (b * 255) << 16u) |
                    ((uint32_t) (g * 255) << 8u) |
                    ((uint32_t) (r * 255));

            Stuff s{};
            s.packed = color;
            return s.floatBits;
        }
    };
}
