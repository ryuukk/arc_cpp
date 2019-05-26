#include "Actor.h"

void arc::Actor::draw(arc::SpriteBatch* batch, float parentAlpha) {

}

void arc::Actor::act(float dt) {

}

arc::Actor* arc::Actor::hit(float x, float y, bool touchable) {
    return nullptr;
}

bool arc::Actor::remove() {
    return false;
}

void arc::Actor::clear() {

}

arc::Stage* arc::Actor::getStage() {
    return nullptr;
}

void arc::Actor::setStage(arc::Stage* stage) {

}

bool arc::Actor::isDescendantOf(arc::Actor* actor) {
    return false;
}

bool arc::Actor::isAscendantOf(arc::Actor* actor) {
    return false;
}

bool arc::Actor::hasParent() {
    return false;
}

arc::Group* arc::Actor::getParent() {
    return nullptr;
}

void arc::Actor::setParent(arc::Group* parent) {

}

bool arc::Actor::isTouchable() {
    return false;
}

void arc::Actor::setTouchable(arc::Touchable touchable) {

}

bool arc::Actor::isVisible() {
    return false;
}

void arc::Actor::setVisible(bool visible) {

}

bool arc::Actor::ancestorsVisible() {
    return false;
}

bool arc::Actor::hasKeyboardFocus() {
    return false;
}

bool arc::Actor::hasScrollFocus() {
    return false;
}

bool arc::Actor::isTouchFocusTarget() {
    return false;
}

float arc::Actor::getX() {
    return 0;
}

void arc::Actor::setX(float x) {

}

void arc::Actor::setX(float x, int align) {

}

float arc::Actor::getY() {
    return 0;
}

void arc::Actor::setY(float Y) {

}

void arc::Actor::setY(float Y, int align) {

}

void arc::Actor::toFront() {

}

void arc::Actor::toBack() {

}

bool arc::Actor::setZIndex(int index) {
    return false;
}

int arc::Actor::getZIndex() {
    return 0;
}

bool arc::Actor::clipBegin(float x, float y, float width, float height) {
    return false;
}

void arc::Actor::clipEnd() {

}

void arc::Actor::positionChanged() {

}

void arc::Actor::sizeChanged() {

}

void arc::Actor::rotationChanged() {

}
