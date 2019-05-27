#pragma once

#include <cstdint>
#include <vector>
#include "InputProcessor.h"
#include "InputEnums.h"

namespace arc
{
    namespace input
    {
        const static int KEY_DOWN = 0;
        const static int KEY_UP = 1;
        const static int KEY_TYPED = 2;
        const static int TOUCH_DOWN = 3;
        const static int TOUCH_UP = 4;
        const static int TOUCH_DRAGGED = 5;
        const static int MMOUSE_MOVED = 6;
        const static int SCROLLED = 7;
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


