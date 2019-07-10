#include "Actor.h"
#include "Group.h"
#include "Stage.h"
#include "../gfx/Align.h"
#include "../utils/ScissorStack.h"

arc::Actor::Actor() {

}

arc::Actor::~Actor() {

}

void arc::Actor::draw(arc::SpriteBatch* batch, float parentAlpha) {

}

void arc::Actor::act(float dt) {

}

arc::Actor* arc::Actor::hit(float x, float y, bool touchable) {
    if (touchable && _touchable != Touchable::enabled) return nullptr;
    if (!isVisible()) return nullptr;
    return x >= 0 && x < _width && y >= 0 && y < _height ? this : nullptr;
}

bool arc::Actor::remove() {
    if (_parent != nullptr) return _parent->removeActor(this, true);
    return false;
}

void arc::Actor::clear() {
    // todo: uncommant when action / event impl
    // clearActions();
    // clearListeners();
}

arc::Stage* arc::Actor::getStage() {
    return _stage;
}

void arc::Actor::setStage(arc::Stage* stage) {
    _stage = stage;
}

bool arc::Actor::isDescendantOf(arc::Actor* actor) {
    if (actor == nullptr) throw std::runtime_error("actor cannot be null.");
    auto* parent = this;
    do {
        if (parent == actor) return true;
        parent = parent->getParent();
    } while (parent != nullptr);
    return false;
}

bool arc::Actor::isAscendantOf(arc::Actor* actor) {
    if (actor == nullptr) throw std::runtime_error("actor cannot be null.");
    do {
        if (actor == this) return true;
        actor = actor->getParent();
    } while (actor != nullptr);
    return false;
}

bool arc::Actor::hasParent() {
    return _parent != nullptr;
}

arc::Group* arc::Actor::getParent() {
    return _parent;
}

void arc::Actor::setParent(arc::Group* parent) {
    _parent = parent;
}

bool arc::Actor::isTouchable() {
    return _touchable == Touchable::enabled;
}

void arc::Actor::setTouchable(arc::Touchable touchable) {
    _touchable = touchable;
}

bool arc::Actor::isVisible() {
    return _visible;
}

void arc::Actor::setVisible(bool visible) {
    _visible = visible;
}

bool arc::Actor::ancestorsVisible() {
    auto* actor = this;
    do {
        if (!actor->isVisible()) return false;
        actor = actor->getParent();
    } while (actor != nullptr);
    return true;
}

bool arc::Actor::hasKeyboardFocus() {
    auto* stage = getStage();
    return stage != nullptr && stage->getKeyboardFocus() == this;
}

bool arc::Actor::hasScrollFocus() {
    auto* stage = getStage();
    return stage != nullptr && stage->getScrollFocus() == this;
}

bool arc::Actor::isTouchFocusTarget() {
    auto* stage = getStage();
    if (stage == nullptr) return false;
    for (int i = 0, n = stage->getTouchFocuses().size(); i < n; i++)
        if (stage->getTouchFocuses()[i].target == this) return true;
    return false;
}

void arc::Actor::setSize(float width, float height) {
    if (this->_width != width || this->_height != height) {
        this->_width = width;
        this->_height = height;
        sizeChanged();
    }
}

float arc::Actor::getX() {
    return _x;
}

void arc::Actor::setX(float x) {

    if (this->_x != x) {
        this->_x = x;
        positionChanged();
    }
}

void arc::Actor::setX(float x, int align) {
    if ((align & (int)Align::right) != 0)
        x -= _width;
    else if ((align & (int)Align::left) == 0) //
        x -= _width / 2;

    if (this->_x != x) {
        this->_x = x;
        positionChanged();
    }
}

float arc::Actor::getY() {
    return _y;
}

void arc::Actor::setY(float y) {
    if (this->_y != y) {
        this->_y = y;
        positionChanged();
    }
}

void arc::Actor::setY(float y, int align) {
    if ((align & (int)Align::top) != 0)
        y -= _height;
    else if ((align & (int)Align::bottom) == 0) //
        y -= _height / 2;

    if (this->_y != y) {
        this->_y = y;
        positionChanged();
    }
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
bool arc::Actor::clipBegin() {
    return clipBegin(_x, _y, _width, _height);
}

bool arc::Actor::clipBegin(float x, float y, float width, float height) {
    if (width <= 0 || height <= 0) return false;
    Rect tableBounds;
    tableBounds.x = x;
    tableBounds.y = y;
    tableBounds.width = width;
    tableBounds.height = height;
    Rect scissorBounds = _stage->calculateScissors(tableBounds);
    if (arc::ScissorStack::pushScissors(scissorBounds)) return true;
    return false;
}

void arc::Actor::clipEnd() {
    arc::ScissorStack::popScissors();
}

void arc::Actor::positionChanged() {

}

void arc::Actor::sizeChanged() {

}

void arc::Actor::rotationChanged() {

}

arc::Touchable arc::Actor::getTouchable() {
    return _touchable;
}

arc::Vec2 arc::Actor::parentToLocalCoordinates(const arc::Vec2& parentCoords) {
    // todo: figure out what to do
    return arc::Vec2();
}

arc::Color& arc::Actor::getColor() {
    return _color;
}
float arc::Actor::getWidth() {
    return _width;
}

float arc::Actor::getHeight(){
    return _height;
}

bool arc::Actor::getDebug() {
    return _debug;
}

float arc::Actor::getOriginX() {
    return _originX;
}

float arc::Actor::getOriginY() {
    return _originY;
}

float arc::Actor::getRotation() {
    return _rotation;
}

float arc::Actor::getScaleX() {
    return _scaleX;
}

float arc::Actor::getScaleY() {
    return _scaleY;
}

void arc::Actor::drawDebug(arc::ShapeRenderer* shapes) {

}

void arc::Actor::drawDebugBounds(arc::ShapeRenderer* shapes) {

}

void arc::Actor::setDebug(bool enabled) {
    _debug = enabled;
    if(enabled) arc::Stage::debug = true;
}

void arc::Actor::setBounds(float x, float y, float width, float height)
{
    if (this->_x != x || this->_y != y) {
        this->_x = x;
        this->_y = y;
        positionChanged();
    }
    if (this->_width != width || this->_height != height) {
        this->_width = width;
        this->_height = height;
        sizeChanged();
    }
}
