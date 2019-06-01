#pragma once

namespace arc
{
    struct Affine2
    {
    public:
        float m00 = 1, m01 = 0, m02 = 0;
        float m10 = 0, m11 = 1, m12 = 0;
        Affine2& idt();
        static Affine2 identity;
    };
}



