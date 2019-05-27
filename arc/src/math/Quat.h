#pragma once

#include "Vec3.h"
#include "Mathf.h"

namespace arc {

    struct Vec3;
    struct Quat
    {
    public:
        Quat();
        Quat(float x, float y, float z, float w)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;
        float len2 ()
        {
            return x * x + y * y + z * z + w * w;
        }
        Quat& nor()
        {
            float len = len2();
            if (len != 0.0f && !Mathf::isEqual(len, 1.0f))
            {
                len = std::sqrt(len);
                w /= len;
                x /= len;
                y /= len;
                z /= len;
            }
            return *this;
        }

        static Quat identity()
        {
            return Quat(0,0,0,1.0f);
        }

        static Quat fromAxis(Vec3 axis, float rad);

    };
}
