#pragma once

#include <string>

namespace arc
{
    template<typename T>
    class ModelNodeKeyframe
    {
    public:
        float keytime{};
        T value;
    };
}


