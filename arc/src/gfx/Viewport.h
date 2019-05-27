#pragma once

#include "Camera.h"

namespace arc
{
    class Viewport
    {
        public

    private:
        Camera* _camera = nullptr;
        float _worldWidth{};
        float _worldHeight{};
        int _screenX{};
        int _screenY{};
        int _screenWidth{};
        int _screenHeight{};
    };
}