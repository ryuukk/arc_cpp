#pragma once

#include <string>

namespace arc
{
    template<typename T>
    struct ModelNodeKeyframe
    {
        float keytime{};
        T value;
    };
}


