#pragma once

#include "InputProcessor.h"

namespace arc
{
    class InputAdapter : public InputProcessor
    {
        virtual bool keyDown(Keys keycode);
        virtual bool keyUp(Keys keycode);
        virtual bool keyTyped(char character);
        virtual bool touchDown(int screenX, int screenY, int pointer, Buttons button);
        virtual bool touchUp(int screenX, int screenY, int pointer, Buttons button);
        virtual bool touchDragged(int screenX, int screenY, int pointer);
        virtual bool mouseMoved(int screenX, int screenY);
        virtual bool scrolled(int amount);
    };
}


