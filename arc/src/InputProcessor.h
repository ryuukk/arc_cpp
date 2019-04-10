#pragma once

#include "Arc.h"
#include "Input.h"
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


    class InputEventQueue : public InputProcessor
    {
    public:
        void setProcessor(InputProcessor* processor);
        void drain();

        bool keyDown(Keys keycode) override {
            return false;
        }

        bool keyUp(Keys keycode) override {
            return false;
        }

        bool keyTyped(char character) override {
            return false;
        }

        bool touchDown(int screenX, int screenY, int pointer, Buttons button) override {
            return false;
        }

        bool touchUp(int screenX, int screenY, int pointer, Buttons button) override {
            return false;
        }

        bool touchDragged(int screenX, int screenY, int pointer) override {
            return false;
        }

        bool mouseMoved(int screenX, int screenY) override {
            return false;
        }

        bool scrolled(int amount) override {
            return false;
        }

    private:
        void queueTime()
        {};

    private:
        InputProcessor* _processor = nullptr;
        std::vector<int> _queue{};
        std::vector<int> _processingQueue{};
        uint64_t _currentEventTime{};
    };
}