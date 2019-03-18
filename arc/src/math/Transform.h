#pragma once

#include "Vec3.h"
#include "Quat.h"
#include "Mat4.h"

namespace arc
{
    struct Transform
    {
        Vec3 translation{0,0,0};
        Quat rotation{0,0,0,1};
        Vec3 scale{1,1,1};

        static Transform idt()
        {
            return { {0,0,0}, {0,0,0,1},  {1,1,1}  };
        }

        static Mat4 toMat4(const Transform& transform)
        {
            return Mat4::identity().set(transform.translation, transform.rotation, transform.scale);
        }
    };
}