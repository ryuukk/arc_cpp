#pragma once

#include "Renderable.h"
#include "RenderContext.h"
#include "../Camera.h"

namespace arc
{
    class Renderable;
    class IShader
    {
    public:
        virtual void init() = 0;

        virtual int compareTo(IShader* other) = 0;

        virtual bool canRender(Renderable* renderable) = 0;

        virtual void begin(Camera* camera, RenderContext* context) = 0;

        virtual void render(Renderable* renderable) = 0;

        virtual void end() = 0;
    };
}