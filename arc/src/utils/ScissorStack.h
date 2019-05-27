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
        bool pushScissors(const Rect& scissor);
        Rect calculateScissors(Camera* camera, float viewportX, float viewportY, float viewportWidth, float viewportHeight, const Mat4& batchTransform, const Rect& area);
    private:
        void fix(Rect& rect);
    private:
        Rect _viewport;
        std::vector<Rect> _scissors;
    };
}

