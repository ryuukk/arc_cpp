#pragma once

namespace arc
{
    struct Affine2
    {
    public:
        float m00 = 1, m01 = 0, m02 = 0;
        float m10 = 0, m11 = 1, m12 = 0;
        Affine2& idt();
        Affine2& setToTrnRotScl (float x, float y, float degrees, float scaleX, float scaleY);
        Affine2& translate (float x, float y);
        Affine2& preMul (const Affine2& other);
        static Affine2 identity;
    };
}



