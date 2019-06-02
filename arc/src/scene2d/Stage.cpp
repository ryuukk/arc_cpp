#include "Stage.h"

arc::Actor* arc::Stage::getKeyboardFocus() {
    return _keyboardFocus;
}
arc::Actor* arc::Stage::getScrollFocus() {
    return _scrollFocus;
}

std::vector<arc::TouchFocus>& arc::Stage::getTouchFocuses() {
    return _touchFocuses;
}

arc::Rect arc::Stage::calculateScissors(const arc::Rect& local) {

    Mat4 transformMatrix;
    if (/*_debugShapes != nullptr && */_debugShapes.isDrawing())
        transformMatrix = _debugShapes.getTransformMatrix();
    else
        transformMatrix = _batch->getTransformMatrix();
    return _viewport->calculateScissors(transformMatrix, local);
}
