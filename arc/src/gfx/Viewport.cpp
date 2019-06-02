#include "Viewport.h"
#include "../utils/HdpiUtils.h"
#include "../utils/ScissorStack.h"

void arc::Viewport::apply(bool centerCamera) {
    arc::hdpi::glViewportt(_screenX, _screenY, _screenWidth, _screenHeight);
    _camera->viewportWidth = _worldWidth;
    _camera->viewportHeight = _worldHeight;
    if(centerCamera)
        _camera->position = {_worldWidth / 2, _worldHeight / 2, 0};
    _camera->update();
}

void arc::Viewport::update(int screenWidth, int screenHeight, bool centerCamera) {
    apply(centerCamera);
}

void arc::Viewport::setScreenBounds(int screenX, int screenY, int screenWidth, int screenHeight) {
    _screenX = screenX;
    _screenY = screenY;
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
}

void arc::Viewport::setScreenSize(int screenWidth, int screenHeight) {
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
}

void arc::Viewport::setScreenPosition(int screenX, int screenY) {
   _screenX = screenX;
   _screenY = screenY;
}

void arc::Viewport::setWorldSize(float worldWidth, float worldHeight) {
    _worldWidth = worldWidth;
    _worldHeight = worldHeight;
}

void arc::Viewport::setCamera(arc::Camera* camera) {
    _camera = camera;
}
arc::Camera* arc::Viewport::getCamera() {
    return _camera;
}

float arc::Viewport::getWorldWidth() {
    return _worldWidth;
}
float arc::Viewport::getWorldHeight() {
    return _worldHeight;
}

arc::Rect arc::Viewport::calculateScissors(const arc::Mat4& batchTransform, const arc::Rect& area) {
    return arc::ScissorStack::calculateScissors(_camera, _screenX, _screenY, _screenWidth, _screenHeight, batchTransform, area);
}


void arc::ScreenViewport::update(int screenWidth, int screenHeight, bool centerCamera) {

    setScreenBounds(0, 0, screenWidth, screenHeight);
    setWorldSize(screenWidth * _unitsPerPixel, screenHeight * _unitsPerPixel);
    apply(centerCamera);
}

arc::ScalingViewport::ScalingViewport(arc::Scaling scaling, float worldWidth, float worldHeight, arc::Camera* camera) {
    _scaling = scaling;
    setWorldSize(worldWidth, worldHeight);
    setCamera(camera);
}

void arc::ScalingViewport::update(int screenWidth, int screenHeight, bool centerCamera) {
    auto scaled = arc::scaling::apply(_scaling, getWorldWidth(), getWorldHeight(), screenWidth, screenHeight);
    int viewportWidth = arc::Mathf::round(scaled.x);
    int viewportHeight = arc::Mathf::round(scaled.y);

    // Center.
    setScreenBounds((screenWidth - viewportWidth) / 2, (screenHeight - viewportHeight) / 2, viewportWidth, viewportHeight);

    apply(centerCamera);
}
