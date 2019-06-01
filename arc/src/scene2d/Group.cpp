#include "Group.h"
#include "Stage.h"
#include <algorithm>

void arc::Group::act(float dt) {
    Actor::act(dt);
    for(auto* actor : _children)
        actor->act(dt);
}

void arc::Group::draw(arc::SpriteBatch* batch, float parentAlpha) {
    if(_transform) applyTransform(batch, computeTransform());
    drawChildren(batch, parentAlpha);
    if(_transform) resetTransform(batch);
}

void arc::Group::drawDebug(arc::ShapeRenderer* shapes) {
    drawDebugBounds(shapes);
    if (_transform) applyTransform(shapes, computeTransform());
    drawDebugChildren(shapes);
    if (_transform) resetTransform(shapes);
}

void arc::Group::setCullingArea(const arc::Rect& area) {
    _cullingArea = area;
}

arc::Actor* arc::Group::hit(float x, float y, bool touchable) {
    if (touchable && getTouchable() == Touchable::disabled) return nullptr;
    if (!isVisible()) return nullptr;
    for (int i = _children.size() - 1; i >= 0; i--) {
        auto* child = _children[i];
        auto point = child->parentToLocalCoordinates({x, y});
        auto* hit = child->hit(point.x, point.y, touchable);
        if (hit != nullptr) return hit;
    }
    return Actor::hit(x, y, touchable);
}

void arc::Group::addActor(arc::Actor* actor) {
    if (actor->getParent() != nullptr) {
        if (actor->getParent() == this) return;
        actor->getParent()->removeActor(actor, false);
    }
    _children.emplace_back(actor);
    actor->setParent(this);
    actor->setStage(getStage());
    childrenChanged();
}

// todo: verify insert
void arc::Group::addActorAt(int index, arc::Actor* actor) {
    if (actor->getParent() != nullptr) {
        if (actor->getParent() == this) return;
        actor->getParent()->removeActor(actor, false);
    }
    if (index >= _children.size())
        _children.emplace_back(actor);
    else
        _children.insert(_children.begin() + index, actor);
    actor->setParent(this);
    actor->setStage(getStage());
    childrenChanged();
}

bool arc::Group::removeActor(arc::Actor* actor, bool unfocus) {
    // todo: test this shitty way of doing things
    if (_children.erase(std::find(_children.begin(), _children.end(), actor)) == _children.end()) return false;
    if (unfocus) {
        auto* stage = getStage();
        if (stage != nullptr) stage->unfocus(actor);
    }
    actor->setParent(nullptr);
    actor->setStage(nullptr);
    childrenChanged();
    return true;
}

void arc::Group::clearChildren() {
    for(auto* actor : _children)
    {
        actor->setStage(nullptr);
        actor->setParent(nullptr);
    }
    childrenChanged();
}

void arc::Group::clear() {
    Actor::clear();
    clearChildren();
}

arc::Actor* arc::Group::findActor(const std::string& name) {
    return nullptr;
}

arc::Vec2 arc::Group::localToDescendantCoordinates(arc::Actor* descendant) {
    //auto* parent = descendant->getParent();
    //if (parent == nullptr) throw std::runtime_error("Child is not a descendant");
    //// First convert to the actor's parent coordinates.
    //if (parent != this) localToDescendantCoordinates(parent, localCoords);
    //// Then from each parent down to the descendant.
    //descendant.parentToLocalCoordinates(localCoords);
    //return localCoords;
    return {0,0}; // todo: finish
}

void arc::Group::setDebug(bool enabled, bool recursively) {
    // todo: finish
}

void arc::Group::drawChildren(arc::SpriteBatch* batch, float parentAlpha) {
    parentAlpha *= getColor().a / 255.0f; // todo: maybe we should have color with floats only ?

}

void arc::Group::drawDebugChildren(arc::ShapeRenderer* shapes) {

}

arc::Mat4 arc::Group::computeTransform() {
    return arc::Mat4();
}

void arc::Group::applyTransform(arc::SpriteBatch* batch, const arc::Mat4& transform) {

}

void arc::Group::resetTransform(arc::SpriteBatch* batch) {

}

void arc::Group::applyTransform(arc::ShapeRenderer* shapes, const arc::Mat4& transform) {

}

void arc::Group::resetTransform(arc::ShapeRenderer* shapes) {

}

void arc::Group::childrenChanged() {

}

void arc::Group::setStage(arc::Stage* stage) {
    Actor::setStage(stage);
}
