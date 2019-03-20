#pragma once

#include "Renderable.h"

namespace arc
{
    class IShaderProvider
    {
    public:
        virtual IShader* getShader(Renderable* renderable) = 0;
    };
}