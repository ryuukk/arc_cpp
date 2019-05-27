#pragma once

#include <cstdint>
#include <vector>
#include "InputProcessor.h"
#include "InputEnums.h"

namespace arc
{
    namespace input
    {
        static const int KEY_DOWN      = 0;
        static const int KEY_UP        = 1;
        static const int KEY_TYPED     = 2;
        static const int TOUCH_DOWN    = 3;
        static const int TOUCH_UP      = 4;
        static const int TOUCH_DRAGGED = 5;
        static const int MOUSE_MOVED   = 6;
        static const int SCROLLED      = 7;
    }

    class InputEventQueue
    {
    public:
        void setProcessor(InputProcessor* processor);
        void drain();
        bool keyDown(Keys keycode);
        bool keyUp(Keys keycode);
        bool keyTyped(char character);
        bool touchDown(int screenX, int screenY, int pointer, Buttons button);
        bool touchUp(int screenX, int screenY, int pointer, Buttons button);
        bool touchDragged(int screenX, int screenY, int pointer);
        bool mouseMoved(int screenX, int screenY);
        bool scrolled(int amount);
        uint64_t getCurrentEventTime();

    private:
        void queueTime();

    private:
        InputProcessor* _processor = nullptr;
        std::vector<int> _queue;
        std::vector<int> _processingQueue;
        uint64_t _currentEventTime{};
    };
}


