#include "Input.h"
#include "Core.h"
#include "math/Mathf.h"

void arc::Input::windowHandleChanged(GLFWwindow* window) {
    _window = window;
    resetPollingStates();

    glfwSetWindowUserPointer(_window, this);

    glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        auto pointer = static_cast<Input*>(glfwGetWindowUserPointer(window));
        pointer->keyCallback(window, key, scancode, action, mode);
    });

    glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int codepoint)
    {
        auto pointer = static_cast<Input*>(glfwGetWindowUserPointer(window));
        pointer->charCallback(window, codepoint);
    });

    glfwSetScrollCallback(_window, [](GLFWwindow* window, double xpos, double ypos)
    {
        auto pointer = static_cast<Input*>(glfwGetWindowUserPointer(window));
        pointer->scrollCallback(window, xpos, ypos);
    });

    glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xpos, double ypos)
    {
        auto pointer = static_cast<Input*>(glfwGetWindowUserPointer(window));
        pointer->cursorPosCallback(window, xpos, ypos);
    });

    glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int state, int mods)
    {
        auto pointer = static_cast<Input*>(glfwGetWindowUserPointer(window));
        pointer->mouseButtonCallback(window, button, state, mods);
    });

}

void arc::Input::update() {
    _eventQueue.setProcessor(_inputProcessor);
    _eventQueue.drain();
}

void arc::Input::prepareNext() {

    _justTouched = false;

    if (_keyJustPressed)
    {
        _keyJustPressed = false;
        for (int i = 0; i < 256; i++)
        {
            _justPressedKeys[i] = false;
        }
    }

    _deltaX = 0;
    _deltaY = 0;
}

int arc::Input::getX() {
    return _mouseX;
}

int arc::Input::getY() {
    return _mouseY;
}

int arc::Input::getDeltaX() {
    return _deltaX;
}

int arc::Input::getDeltaY() {
    return _deltaY;
}

bool arc::Input::isTouched() {
    return glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_1) ||
           glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_2) ||
           glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_3) ||
           glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_4) ||
           glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_5);
}

bool arc::Input::justTouched() {
    return _justTouched;
}

bool arc::Input::isButtonPressed(arc::Buttons button) {
    return glfwGetMouseButton(_window, (int) button);
}

bool arc::Input::isKeyPressed(arc::Keys key) {
    if(key == Keys::ANY_KEY) return _pressedKeys > 0;
    if(key == Keys::SYM)
    {
        return glfwGetKey(_window, GLFW_KEY_LEFT_SUPER) ||
               glfwGetKey(_window, GLFW_KEY_RIGHT_SUPER);
    }
    return glfwGetKey(_window, arc::input::getGlfwKeyCode(key));
}

bool arc::Input::isKeyJustPressed(arc::Keys key) {

    if(key == Keys::ANY_KEY) return _keyJustPressed;
    if((int)key < 0 || (int) key > 256) return false;

    return _justPressedKeys[(int) key];

}

// private

void arc::Input::resetPollingStates() {
    _justTouched = false;
    _keyJustPressed = false;

    for (int i = 0; i < 256; i++)
    {
        _justPressedKeys[i] = false;
    }

    _eventQueue.setProcessor(nullptr);
    _eventQueue.drain();
}

void arc::Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    // todo: use switch
    if(action == GLFW_PRESS)
    {
        auto k = arc::input::getEnumKeyCode(key);
        _eventQueue.keyDown(k);
        _pressedKeys++;
        _keyJustPressed = true;
        _justPressedKeys[(int)k] = true;
        _lastCharacter = (char) 0;
        char character = arc::input::characterForKeyCode(k);
        if(character != 0) charCallback(window, character);
    }
    else if(action == GLFW_RELEASE)
    {
        _pressedKeys--;
        _eventQueue.keyUp(arc::input::getEnumKeyCode(key));
    }
    else if(action == GLFW_REPEAT)
    {
        if(_lastCharacter != 0)
        {
            _eventQueue.keyTyped(_lastCharacter);
        }
    }
}

void arc::Input::charCallback(GLFWwindow* window, uint32_t codepoint) {
    if ((codepoint & 0xff00u) == 0xf700) return;
    _lastCharacter = (char) codepoint;
    //window.getGraphics().requestRendering();
    _eventQueue.keyTyped((char) codepoint);
}

void arc::Input::scrollCallback(GLFWwindow* window, double xpos, double ypos) {
    _eventQueue.scrolled(-Mathf::sign(ypos));
}

void arc::Input::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    _deltaX = (int) xpos - _logicalMouseX;
    _deltaY = (int) ypos - _logicalMouseY;
    _mouseX = _logicalMouseX = (int) xpos;
    _mouseY = _logicalMouseY = (int) ypos;

    auto gfx = Core::graphics;
    if (gfx->getHdpiMode() == HdpiMode::Pixels)
    {
        float xScale = gfx->getBackBufferWidth() / (float) gfx->getLogicalWidth();
        float yScale = gfx->getBackBufferHeight() / (float) gfx->getLogicalHeight();
        _deltaX = (int) (_deltaX * xScale);
        _deltaY = (int) (_deltaY * yScale);
        _mouseX = (int) (_mouseX * xScale);
        _mouseY = (int) (_mouseY * yScale);
    }

    //gfx.requestRendering();
    if (_mousePressed > 0)
    {
        _eventQueue.touchDragged(_mouseX, _mouseY, 0);
    }
    else
    {
        _eventQueue.mouseMoved(_mouseX, _mouseY);
    }

}

void arc::Input::mouseButtonCallback(GLFWwindow* window, int button, int state, int mods) {
    auto b = arc::input::toEnumButton(button);
    if (b == Buttons::UKNOWN) return;

    if (state == GLFW_PRESS)
    {
        _mousePressed++;
        _justTouched = true;
        //window.getGraphics().requestRendering();
        _eventQueue.touchDown(_mouseX, _mouseY, 0, b);
    }
    else
    {
        _mousePressed = Mathf::max(0, _mousePressed - 1);
        //window.getGraphics().requestRendering();
        _eventQueue.touchUp(_mouseX, _mouseY, 0, b);
    }
}

void arc::Input::setInputProcessor(arc::InputProcessor* processor) {
    _inputProcessor = processor;
}
