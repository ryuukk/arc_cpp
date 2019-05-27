#pragma once

#include "InputEnums.h"

namespace arc
{
    class InputProcessor
    {
    public:
        virtual bool keyDown(Keys keycode) = 0;
        virtual bool keyUp(Keys keycode) = 0;
        virtual bool keyTyped(char character) = 0;
        virtual bool touchDown(int screenX, int screenY, int pointer, Buttons button) = 0;
        virtual bool touchUp(int screenX, int screenY, int pointer, Buttons button) = 0;
        virtual bool touchDragged(int screenX, int screenY, int pointer) = 0;
        virtual bool mouseMoved(int screenX, int screenY) = 0;
        virtual bool scrolled(int amount) = 0;
    };
}