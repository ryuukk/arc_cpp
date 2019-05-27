#pragma once

#include "../Core.h"

namespace arc
{
    namespace hdpi
    {
        inline static int toLogicalX(int backBufferX)
        {
            return (int) (backBufferX *  Core::graphics->getWidth() / (float) Core::graphics->getBackBufferWidth());
        }
        inline static int toLogicalY(int backBufferY)
        {
            return (int) (backBufferY * Core::graphics->getHeight() / (float) Core::graphics->getBackBufferHeight());
        }

        inline static int toBackBufferX(int logicalX)
        {
            return (int) (logicalX * Core::graphics->getBackBufferWidth() / (float) Core::graphics->getWidth());
        }

        inline static int toBackBufferY(int logicalY)
        {
            return (int) (logicalY * Core::graphics->getBackBufferHeight() / (float) Core::graphics->getHeight());
        }

        // todo: figure out how to be able to use same name..
        inline static void glScissorr(int x, int y, int width, int height)
        {
            if (Core::graphics->getWidth() != Core::graphics->getBackBufferWidth()
                || Core::graphics->getHeight() != Core::graphics->getBackBufferHeight())
            {
                glScissor(toBackBufferX(x), toBackBufferY(y), toBackBufferX(width), toBackBufferY(height));
            }
            else
            {
                glScissor(x, y, width, height);
            }
        }

        inline static void glViewportt(int x, int y, int width, int height)
        {
            if (Core::graphics->getWidth() != Core::graphics->getBackBufferWidth()
                || Core::graphics->getHeight() != Core::graphics->getBackBufferHeight())
            {
                glViewport(toBackBufferX(x), toBackBufferY(y), toBackBufferX(width), toBackBufferY(height));
            }
            else
            {
                glViewport(x, y, width, height);
            }
        }
    }
}