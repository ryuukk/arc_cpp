#pragma once

#include <vector>
#include "InputProcessor.h"

namespace arc
{
    class Input
    {
    public:
        Input() = default;

        void update();
        void prepareNext();

    private:
        void resetPollingStates()
        {
            _justTouched = false;
            _keyJustPressed = false;
            for (int i = 0; i < _justPressedKeys.size(); i++)
            {
                _justPressedKeys[i] = false;
            }

            _eventQueue.setProcessor(nullptr);
            _eventQueue.drain();
        }

    private:
        GLFWwindow* _window = nullptr;
        InputProcessor* _inputProcessor = nullptr;

        InputEventQueue _eventQueue{};
        int _mouseX{};
        int _mouseY {};
        int _mousePressed {};
        int _deltaX{};
        int _deltaY{};
        bool _justTouched {};
        int _pressedKeys {};
        bool _keyJustPressed {};
        std::vector<bool> _justPressedKeys {};
        char _lastCharacter {};
    };
}