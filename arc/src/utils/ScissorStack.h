#pragma once

#include <vector>
#include "../gfx/Camera.h"
#include "../math/Rect.h"
#include "../math/Mat4.h"

namespace arc
{
    class ScissorStack
    {
    public:
        static bool pushScissors(const Rect& scissor);
        static Rect calculateScissors(Camera* camera, const Mat4& batchTransform, const Rect& area);
        static Rect calculateScissors(Camera* camera, float viewportX, float viewportY, float viewportWidth, float viewportHeight, const Mat4& batchTransform, const Rect& area);
        static Rect popScissors();
    private:
        static void fix(Rect& rect);
    private:
        static Rect _viewport;
        static std::vector<Rect> _scissors;
    };
}

