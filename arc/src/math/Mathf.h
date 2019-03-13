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
    };
}