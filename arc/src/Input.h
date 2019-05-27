#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "input/InputProcessor.h"
#include "input/InputEventQueue.h"
#include "input/InputEnums.h"

namespace arc
{
    namespace input
    {
        inline static char characterForKeyCode(Keys key)
        {
            switch (key)
            {
                case Keys::BACKSPACE:
                    return (char) 8;
                case Keys::TAB:
                    return '\t';
                case Keys::FORWARD_DEL:
                    return (char) 127;
                case Keys::ENTER:
                    return '\n';
            }

            return (char) 0;
        }

        inline static Keys getEnumKeyCode(int keycode)
        {
            switch (keycode)
            {
                case GLFW_KEY_SPACE:
                    return Keys::SPACE;
                case GLFW_KEY_APOSTROPHE:
                    return Keys::APOSTROPHE;
                case GLFW_KEY_COMMA:
                    return Keys::COMMA;
                case GLFW_KEY_MINUS:
                    return Keys::MINUS;
                case GLFW_KEY_PERIOD:
                    return Keys::PERIOD;
                case GLFW_KEY_SLASH:
                    return Keys::SLASH;
                case GLFW_KEY_0:
                    return Keys::NUM_0;
                case GLFW_KEY_1:
                    return Keys::NUM_1;
                case GLFW_KEY_2:
                    return Keys::NUM_2;
                case GLFW_KEY_3:
                    return Keys::NUM_3;
                case GLFW_KEY_4:
                    return Keys::NUM_4;
                case GLFW_KEY_5:
                    return Keys::NUM_5;
                case GLFW_KEY_6:
                    return Keys::NUM_6;
                case GLFW_KEY_7:
                    return Keys::NUM_7;
                case GLFW_KEY_8:
                    return Keys::NUM_8;
                case GLFW_KEY_9:
                    return Keys::NUM_9;
                case GLFW_KEY_SEMICOLON:
                    return Keys::SEMICOLON;
                case GLFW_KEY_EQUAL:
                    return Keys::EQUALS;
                case GLFW_KEY_A:
                    return Keys::A;
                case GLFW_KEY_B:
                    return Keys::B;
                case GLFW_KEY_C:
                    return Keys::C;
                case GLFW_KEY_D:
                    return Keys::D;
                case GLFW_KEY_E:
                    return Keys::E;
                case GLFW_KEY_F:
                    return Keys::F;
                case GLFW_KEY_G:
                    return Keys::G;
                case GLFW_KEY_H:
                    return Keys::H;
                case GLFW_KEY_I:
                    return Keys::I;
                case GLFW_KEY_J:
                    return Keys::J;
                case GLFW_KEY_K:
                    return Keys::K;
                case GLFW_KEY_L:
                    return Keys::L;
                case GLFW_KEY_M:
                    return Keys::M;
                case GLFW_KEY_N:
                    return Keys::N;
                case GLFW_KEY_O:
                    return Keys::O;
                case GLFW_KEY_P:
                    return Keys::P;
                case GLFW_KEY_Q:
                    return Keys::Q;
                case GLFW_KEY_R:
                    return Keys::R;
                case GLFW_KEY_S:
                    return Keys::S;
                case GLFW_KEY_T:
                    return Keys::T;
                case GLFW_KEY_U:
                    return Keys::U;
                case GLFW_KEY_V:
                    return Keys::V;
                case GLFW_KEY_W:
                    return Keys::W;
                case GLFW_KEY_X:
                    return Keys::X;
                case GLFW_KEY_Y:
                    return Keys::Y;
                case GLFW_KEY_Z:
                    return Keys::Z;
                case GLFW_KEY_LEFT_BRACKET:
                    return Keys::LEFT_BRACKET;
                case GLFW_KEY_BACKSLASH:
                    return Keys::BACKSLASH;
                case GLFW_KEY_RIGHT_BRACKET:
                    return Keys::RIGHT_BRACKET;
                case GLFW_KEY_GRAVE_ACCENT:
                    return Keys::GRAVE;
                case GLFW_KEY_WORLD_1:
                case GLFW_KEY_WORLD_2:
                    return Keys::UNKNOWN;
                case GLFW_KEY_ESCAPE:
                    return Keys::ESCAPE;
                case GLFW_KEY_ENTER:
                    return Keys::ENTER;
                case GLFW_KEY_TAB:
                    return Keys::TAB;
                case GLFW_KEY_BACKSPACE:
                    return Keys::BACKSPACE;
                case GLFW_KEY_INSERT:
                    return Keys::INSERT;
                case GLFW_KEY_DELETE:
                    return Keys::FORWARD_DEL;
                case GLFW_KEY_RIGHT:
                    return Keys::RIGHT;
                case GLFW_KEY_LEFT:
                    return Keys::LEFT;
                case GLFW_KEY_DOWN:
                    return Keys::DOWN;
                case GLFW_KEY_UP:
                    return Keys::UP;
                case GLFW_KEY_PAGE_UP:
                    return Keys::PAGE_UP;
                case GLFW_KEY_PAGE_DOWN:
                    return Keys::PAGE_DOWN;
                case GLFW_KEY_HOME:
                    return Keys::HOME;
                case GLFW_KEY_END:
                    return Keys::END;
                case GLFW_KEY_CAPS_LOCK:
                case GLFW_KEY_SCROLL_LOCK:
                case GLFW_KEY_NUM_LOCK:
                case GLFW_KEY_PRINT_SCREEN:
                case GLFW_KEY_PAUSE:
                    return Keys::UNKNOWN;
                case GLFW_KEY_F1:
                    return Keys::F1;
                case GLFW_KEY_F2:
                    return Keys::F2;
                case GLFW_KEY_F3:
                    return Keys::F3;
                case GLFW_KEY_F4:
                    return Keys::F4;
                case GLFW_KEY_F5:
                    return Keys::F5;
                case GLFW_KEY_F6:
                    return Keys::F6;
                case GLFW_KEY_F7:
                    return Keys::F7;
                case GLFW_KEY_F8:
                    return Keys::F8;
                case GLFW_KEY_F9:
                    return Keys::F9;
                case GLFW_KEY_F10:
                    return Keys::F10;
                case GLFW_KEY_F11:
                    return Keys::F11;
                case GLFW_KEY_F12:
                    return Keys::F12;
                case GLFW_KEY_F13:
                case GLFW_KEY_F14:
                case GLFW_KEY_F15:
                case GLFW_KEY_F16:
                case GLFW_KEY_F17:
                case GLFW_KEY_F18:
                case GLFW_KEY_F19:
                case GLFW_KEY_F20:
                case GLFW_KEY_F21:
                case GLFW_KEY_F22:
                case GLFW_KEY_F23:
                case GLFW_KEY_F24:
                case GLFW_KEY_F25:
                    return Keys::UNKNOWN;
                case GLFW_KEY_KP_0:
                    return Keys::NUMPAD_0;
                case GLFW_KEY_KP_1:
                    return Keys::NUMPAD_1;
                case GLFW_KEY_KP_2:
                    return Keys::NUMPAD_2;
                case GLFW_KEY_KP_3:
                    return Keys::NUMPAD_3;
                case GLFW_KEY_KP_4:
                    return Keys::NUMPAD_4;
                case GLFW_KEY_KP_5:
                    return Keys::NUMPAD_5;
                case GLFW_KEY_KP_6:
                    return Keys::NUMPAD_6;
                case GLFW_KEY_KP_7:
                    return Keys::NUMPAD_7;
                case GLFW_KEY_KP_8:
                    return Keys::NUMPAD_8;
                case GLFW_KEY_KP_9:
                    return Keys::NUMPAD_9;
                case GLFW_KEY_KP_DECIMAL:
                    return Keys::PERIOD;
                case GLFW_KEY_KP_DIVIDE:
                    return Keys::SLASH;
                case GLFW_KEY_KP_MULTIPLY:
                    return Keys::STAR;
                case GLFW_KEY_KP_SUBTRACT:
                    return Keys::MINUS;
                case GLFW_KEY_KP_ADD:
                    return Keys::PLUS;
                case GLFW_KEY_KP_ENTER:
                    return Keys::ENTER;
                case GLFW_KEY_KP_EQUAL:
                    return Keys::EQUALS;
                case GLFW_KEY_LEFT_SHIFT:
                    return Keys::SHIFT_LEFT;
                case GLFW_KEY_LEFT_CONTROL:
                    return Keys::CONTROL_LEFT;
                case GLFW_KEY_LEFT_ALT:
                    return Keys::ALT_LEFT;
                case GLFW_KEY_LEFT_SUPER:
                    return Keys::SYM;
                case GLFW_KEY_RIGHT_SHIFT:
                    return Keys::SHIFT_RIGHT;
                case GLFW_KEY_RIGHT_CONTROL:
                    return Keys::CONTROL_RIGHT;
                case GLFW_KEY_RIGHT_ALT:
                    return Keys::ALT_RIGHT;
                case GLFW_KEY_RIGHT_SUPER:
                    return Keys::SYM;
                case GLFW_KEY_MENU:
                    return Keys::MENU;
                default:
                    return Keys::UNKNOWN;
            }
        }
        inline static int getGlfwKeyCode(Keys keycode)
        {
            switch (keycode)
            {
                case Keys::SPACE:
                    return (int) GLFW_KEY_SPACE;
                case Keys::APOSTROPHE:
                    return (int) GLFW_KEY_APOSTROPHE;
                case Keys::COMMA:
                    return (int) GLFW_KEY_COMMA;
                case Keys::PERIOD:
                    return (int) GLFW_KEY_PERIOD;
                case Keys::NUM_0:
                    return (int) GLFW_KEY_0;
                case Keys::NUM_1:
                    return (int) GLFW_KEY_1;
                case Keys::NUM_2:
                    return (int) GLFW_KEY_2;
                case Keys::NUM_3:
                    return (int) GLFW_KEY_3;
                case Keys::NUM_4:
                    return (int) GLFW_KEY_4;
                case Keys::NUM_5:
                    return (int) GLFW_KEY_5;
                case Keys::NUM_6:
                    return (int) GLFW_KEY_6;
                case Keys::NUM_7:
                    return (int) GLFW_KEY_7;
                case Keys::NUM_8:
                    return (int) GLFW_KEY_8;
                case Keys::NUM_9:
                    return (int) GLFW_KEY_9;
                case Keys::SEMICOLON:
                    return (int) GLFW_KEY_SEMICOLON;
                case Keys::EQUALS:
                    return (int) GLFW_KEY_EQUAL;
                case Keys::A:
                    return (int) GLFW_KEY_A;
                case Keys::B:
                    return (int) GLFW_KEY_B;
                case Keys::C:
                    return (int) GLFW_KEY_C;
                case Keys::D:
                    return (int) GLFW_KEY_D;
                case Keys::E:
                    return (int) GLFW_KEY_E;
                case Keys::F:
                    return (int) GLFW_KEY_F;
                case Keys::G:
                    return (int) GLFW_KEY_G;
                case Keys::H:
                    return (int) GLFW_KEY_H;
                case Keys::I:
                    return (int) GLFW_KEY_I;
                case Keys::J:
                    return (int) GLFW_KEY_J;
                case Keys::K:
                    return (int) GLFW_KEY_K;
                case Keys::L:
                    return (int) GLFW_KEY_L;
                case Keys::M:
                    return (int) GLFW_KEY_M;
                case Keys::N:
                    return (int) GLFW_KEY_N;
                case Keys::O:
                    return (int) GLFW_KEY_O;
                case Keys::P:
                    return (int) GLFW_KEY_P;
                case Keys::Q:
                    return (int) GLFW_KEY_Q;
                case Keys::R:
                    return (int) GLFW_KEY_R;
                case Keys::S:
                    return (int) GLFW_KEY_S;
                case Keys::T:
                    return (int) GLFW_KEY_T;
                case Keys::U:
                    return (int) GLFW_KEY_U;
                case Keys::V:
                    return (int) GLFW_KEY_V;
                case Keys::W:
                    return (int) GLFW_KEY_W;
                case Keys::X:
                    return (int) GLFW_KEY_X;
                case Keys::Y:
                    return (int) GLFW_KEY_Y;
                case Keys::Z:
                    return (int) GLFW_KEY_Z;
                case Keys::LEFT_BRACKET:
                    return (int) GLFW_KEY_LEFT_BRACKET;
                case Keys::BACKSLASH:
                    return (int) GLFW_KEY_BACKSLASH;
                case Keys::RIGHT_BRACKET:
                    return (int) GLFW_KEY_RIGHT_BRACKET;
                case Keys::GRAVE:
                    return (int) GLFW_KEY_GRAVE_ACCENT;
                case Keys::ESCAPE:
                    return (int) GLFW_KEY_ESCAPE;
                case Keys::ENTER:
                    return (int) GLFW_KEY_ENTER;
                case Keys::TAB:
                    return (int) GLFW_KEY_TAB;
                case Keys::BACKSPACE:
                    return (int) GLFW_KEY_BACKSPACE;
                case Keys::INSERT:
                    return (int) GLFW_KEY_INSERT;
                case Keys::FORWARD_DEL:
                    return (int) GLFW_KEY_DELETE;
                case Keys::RIGHT:
                    return (int) GLFW_KEY_RIGHT;
                case Keys::LEFT:
                    return (int) GLFW_KEY_LEFT;
                case Keys::DOWN:
                    return (int) GLFW_KEY_DOWN;
                case Keys::UP:
                    return (int) GLFW_KEY_UP;
                case Keys::PAGE_UP:
                    return (int) GLFW_KEY_PAGE_UP;
                case Keys::PAGE_DOWN:
                    return (int) GLFW_KEY_PAGE_DOWN;
                case Keys::HOME:
                    return (int) GLFW_KEY_HOME;
                case Keys::END:
                    return (int) GLFW_KEY_END;
                case Keys::F1:
                    return (int) GLFW_KEY_F1;
                case Keys::F2:
                    return (int) GLFW_KEY_F2;
                case Keys::F3:
                    return (int) GLFW_KEY_F3;
                case Keys::F4:
                    return (int) GLFW_KEY_F4;
                case Keys::F5:
                    return (int) GLFW_KEY_F5;
                case Keys::F6:
                    return (int) GLFW_KEY_F6;
                case Keys::F7:
                    return (int) GLFW_KEY_F7;
                case Keys::F8:
                    return (int) GLFW_KEY_F8;
                case Keys::F9:
                    return (int) GLFW_KEY_F9;
                case Keys::F10:
                    return (int) GLFW_KEY_F10;
                case Keys::F11:
                    return (int) GLFW_KEY_F11;
                case Keys::F12:
                    return (int) GLFW_KEY_F12;
                case Keys::NUMPAD_0:
                    return (int) GLFW_KEY_KP_0;
                case Keys::NUMPAD_1:
                    return (int) GLFW_KEY_KP_1;
                case Keys::NUMPAD_2:
                    return (int) GLFW_KEY_KP_2;
                case Keys::NUMPAD_3:
                    return (int) GLFW_KEY_KP_3;
                case Keys::NUMPAD_4:
                    return (int) GLFW_KEY_KP_4;
                case Keys::NUMPAD_5:
                    return (int) GLFW_KEY_KP_5;
                case Keys::NUMPAD_6:
                    return (int) GLFW_KEY_KP_6;
                case Keys::NUMPAD_7:
                    return (int) GLFW_KEY_KP_7;
                case Keys::NUMPAD_8:
                    return (int) GLFW_KEY_KP_8;
                case Keys::NUMPAD_9:
                    return (int) GLFW_KEY_KP_9;
                case Keys::SLASH:
                    return (int) GLFW_KEY_KP_DIVIDE;
                case Keys::STAR:
                    return (int) GLFW_KEY_KP_MULTIPLY;
                case Keys::MINUS:
                    return (int) GLFW_KEY_KP_SUBTRACT;
                case Keys::PLUS:
                    return (int) GLFW_KEY_KP_ADD;
                case Keys::SHIFT_LEFT:
                    return (int) GLFW_KEY_LEFT_SHIFT;
                case Keys::CONTROL_LEFT:
                    return (int) GLFW_KEY_LEFT_CONTROL;
                case Keys::ALT_LEFT:
                    return (int) GLFW_KEY_LEFT_ALT;
                case Keys::SYM:
                    return (int) GLFW_KEY_LEFT_SUPER;
                case Keys::SHIFT_RIGHT:
                    return (int) GLFW_KEY_RIGHT_SHIFT;
                case Keys::CONTROL_RIGHT:
                    return (int) GLFW_KEY_RIGHT_CONTROL;
                case Keys::ALT_RIGHT:
                    return (int) GLFW_KEY_RIGHT_ALT;
                case Keys::MENU:
                    return (int) GLFW_KEY_MENU;
                default:
                    return 0;
            }
        }
        inline static Buttons toEnumButton(int button)
        {
            if (button == GLFW_MOUSE_BUTTON_LEFT) return Buttons::LEFT;
            if (button == GLFW_MOUSE_BUTTON_RIGHT) return Buttons::RIGHT;
            if (button == GLFW_MOUSE_BUTTON_MIDDLE) return Buttons::MIDDLE;
            return Buttons::UKNOWN;
        }
    }

    class Input
    {
    public:
        Input() = default;

        void setInputProcessor(InputProcessor* processor);

        void windowHandleChanged(GLFWwindow* window);
        void update();
        void prepareNext();

        int getX();
        int getY();
        int getDeltaX();
        int getDeltaY();
        bool isTouched();
        bool justTouched();
        bool isButtonPressed(Buttons button);
        bool isKeyPressed(Keys key);
        bool isKeyJustPressed(Keys key);

    private:
        void resetPollingStates();

        void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void charCallback(GLFWwindow* window, uint32_t codepoint);
        void scrollCallback(GLFWwindow* window, double xpos, double ypos);
        void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        void mouseButtonCallback(GLFWwindow* window, int button, int state, int mods);

    private:
        GLFWwindow* _window = nullptr;
        InputProcessor* _inputProcessor = nullptr;
        InputEventQueue _eventQueue{};

        int _mouseX{};
        int _mouseY{};
        int _mousePressed{};
        int _deltaX{};
        int _deltaY{};
        bool _justTouched{};
        int _pressedKeys{};
        bool _keyJustPressed{};
        bool _justPressedKeys[256];
        char _lastCharacter{};

        int _logicalMouseX{};
        int _logicalMouseY{};
    };
}