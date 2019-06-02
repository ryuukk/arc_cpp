#include "PrimitiveDrawable.h"

arc::PrimitiveDrawable::PrimitiveDrawable(const arc::Color& color, float minWidth, float minHeight):
_color(color), _minWidth(minWidth), _minHeight(minHeight)
{
}

void arc::PrimitiveDrawable::draw(arc::SpriteBatch* batch, float x, float y, float width, float height) {

    auto* pixel = batch->getWhitePixel();

    batch->setColor(_color);
    batch->draw(pixel, {x, y}, {width, height});
}

float arc::PrimitiveDrawable::getLeftWidth() {
    return _leftWidth;
}

void arc::PrimitiveDrawable::setLeftWidth(float leftWidth) {
    _leftWidth = leftWidth;
}

float arc::PrimitiveDrawable::getRightWidth() {
    return _rightWidth;
}

void arc::PrimitiveDrawable::setRightWidth(float rightWidth) {
    _rightWidth = rightWidth;
}

float arc::PrimitiveDrawable::getTopHeight() {
    return _topHeight;
}

void arc::PrimitiveDrawable::setTopHeight(float topHeight) {
    _topHeight = topHeight;
}

float arc::PrimitiveDrawable::getBottomHeight() {
    return _bottomHeight;
}

void arc::PrimitiveDrawable::setBottomHeight(float bottomHeight) {
    _bottomHeight = bottomHeight;
}

float arc::PrimitiveDrawable::getMinWidth() {
    return _minWidth;
}

void arc::PrimitiveDrawable::setMinWidth(float minWidth) {
    _minWidth = minWidth;
}

float arc::PrimitiveDrawable::getMinHeight() {
    return _minHeight;
}

void arc::PrimitiveDrawable::setMinHeight(float minHeight) {
    _minHeight = minHeight;
}
