#pragma once

#include <cmath>
#ifdef WIN32
#undef min
#undef max
#endif
namespace arc
{
    // todo: use const instead of inline static methods
    class Mathf
    {
    public:
        inline static float FLOAT_ROUNDING_ERROR() { return 0.000001f; }

        inline static float PI() { return 3.1415927f; }

        inline static float PI2() { return PI() * 2; }

        inline static float DEG2RAD() { return PI() / 180.0f; }

        inline static float RAD2DEG() { return 180.0f / PI(); }

        inline static bool isEqual(float a, float b) {
            return abs(a - b) <= FLOAT_ROUNDING_ERROR();
        }

        inline static int round(float data)
        {
            return data >= 0.0f ? (int) (data + 0.5f) : (int) (data - 0.5f);
        }
        inline static int floor(float data)
        {
            return data >= 0.0f ? (int) data : (int) data - 1;
        }
        inline static int abs(int v)
        {
            return v >= 0 ? v : -v;
        }
        inline static float abs(float v)
        {
            return v >= 0.0f ? v : -v;
        }
        inline static int max(int a, int b)
        {
            return a >= b ? a : b;
        }
        inline static int min(int a, int b)
        {
            return a >= b ? b : a;
        }

        inline static float max(float a, float b)
        {
            return a >= b ? a : b;
        }

        inline static float min(float a, float b)
        {
            return a >= b ? b : a;
        }
        inline static int sign(float v)
        {
            return v > 0.0f ? 1 : (v < 0.0f ? -1 : 0);
        }
        inline static int sign(double v)
        {
            return v > 0.0 ? 1 : (v < 0.0 ? -1 : 0);
        }

        inline static int sign(int v)
        {
            return v > 0 ? 1 : (v < 0 ? -1 : 0);
        }

        inline static float sinDeg(float deg)
        {
            return std::sin(DEG2RAD() * deg);
        }

        inline static float cosDeg(float deg)
        {
            return std::cos(DEG2RAD() * deg);
        }

        inline static float sin(float rad)
        {
            return std::sin(rad);
        }

        inline static float cos(float rad)
        {
            return std::cos(rad);
        }

        inline static int clamp(int data, int min, int max)
        {
            if (data < min)
            {
                return min;
            }
            else
            {
                if (data > max)
                {
                    return max;
                }

                return data;
            }
        }

    };
}