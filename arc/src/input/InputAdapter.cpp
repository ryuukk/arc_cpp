#include "InputAdapter.h"

bool arc::InputAdapter::keyDown(arc::Keys keycode) {
    return false;
}

bool arc::InputAdapter::keyUp(arc::Keys keycode) {
    return false;
}

bool arc::InputAdapter::keyTyped(char character) {
    return false;
}

bool arc::InputAdapter::touchDown(int screenX, int screenY, int pointer, arc::Buttons button) {
    return false;
}

bool arc::InputAdapter::touchUp(int screenX, int screenY, int pointer, arc::Buttons button) {
    return false;
}

bool arc::InputAdapter::touchDragged(int screenX, int screenY, int pointer) {
    return false;
}

bool arc::InputAdapter::mouseMoved(int screenX, int screenY) {
    return false;
}

bool arc::InputAdapter::scrolled(int amount) {
    return false;
}
