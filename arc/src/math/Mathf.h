#pragma once

namespace arc
{
    class Mathf
    {
    public:
        static float FLOAT_ROUNDING_ERROR() { return 0.000001f; }

        static float PI() { return 3.1415927f; }

        static float PI2() { return PI() * 2; }

        static float DEG2RAD() { return PI() / 180.0f; }

        static float RAD2DEG() { return 180.0f / PI(); }

        static bool isEqual(float a, float b) {
            return abs(a - b) <= FLOAT_ROUNDING_ERROR();
        }

        static int round(float data)
        {
            return data >= 0.0f ? (int) (data + 0.5f) : (int) (data - 0.5f);
        }
        int floor(float data)
        {
            return data >= 0.0f ? (int) data : (int) data - 1;
        }
        static int abs(int v)
        {
            return v >= 0 ? v : -v;
        }
        static float abs(float v)
        {
            return v >= 0.0f ? v : -v;
        }
        static int max(int a, int b)
        {
            return a >= b ? a : b;
        }
        static int min(int a, int b)
        {
            return a >= b ? b : a;
        }

        static float max(float a, float b)
        {
            return a >= b ? a : b;
        }

        static float min(float a, float b)
        {
            return a >= b ? b : a;
        }
    };
}