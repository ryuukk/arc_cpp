#include "Stage.h"
#include "../Core.h"

bool arc::Stage::debug = false;

arc::Stage::Stage(arc::Viewport* viewport) {

    if(viewport == nullptr)
        viewport = new arc::ScalingViewport(arc::Scaling::stretch, arc::Core::graphics->getWidth(), arc::Core::graphics->getHeight(), new arc::OrthographicCamera());

    _viewport = viewport;
    _batch = new arc::SpriteBatch();
    _ownsBatch = true;

    _root.setStage(this);

    _viewport->update(arc::Core::graphics->getWidth(), arc::Core::graphics->getHeight(), true);
}

arc::Stage::~Stage() {

}

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
arc::Group& arc::Stage::getRoot() {
    return _root;
}

float arc::Stage::getWidth() {
    return _viewport->getWorldWidth();
}

float arc::Stage::getHeight(){
    return _viewport->getWorldHeight();
}

void arc::Stage::render(float dt) {
    auto* camera = _viewport->getCamera();
    camera->update();

    if(!_root.isVisible()) return;

    _batch->setProjectionMatrix(camera->combined);
    _batch->begin();
    _root.draw(_batch, 1.0f);
    _batch->end();
}

void arc::Stage::act(float dt) {
    _root.act(dt);
}

void arc::Stage::unfocus(arc::Actor* actor) {

}

void arc::Stage::addActor(arc::Actor* actor) {
    _root.addActor(actor);
}
