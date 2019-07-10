#include <algorithm>
#include "Group.h"
#include "Stage.h"

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
    arc::Actor::drawDebugBounds(shapes);
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
        if (stage != nullptr)
            stage->unfocus(actor);
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
    parentAlpha *= getColor().a; // todo: maybe we should have color with floats only ?

    if(!_cullingArea.isZero())
    {
        // Draw children only if inside culling area.
        float cullLeft = _cullingArea.x;
        float cullRight = cullLeft + _cullingArea.width;
        float cullBottom = _cullingArea.y;
        float cullTop = cullBottom + _cullingArea.height;
        if (_transform) {
            for (int i = 0, n = _children.size(); i < n; i++) {
                auto* child = _children[i];
                if (!child->isVisible()) continue;
                float cx = child->getX(), cy = child->getY();
                if (cx <= cullRight && cy <= cullTop && cx + child->getWidth() >= cullLeft && cy + child->getHeight() >= cullBottom)
                    child->draw(batch, parentAlpha);
            }
        } else {
            // No transform for this group, offset each child.
            float offsetX = getX(), offsetY = getY();
            setX(0.0f);
            setY(0.0f);
            for (int i = 0, n = _children.size(); i < n; i++) {
                auto* child = _children[i];
                if (!child->isVisible()) continue;
                float cx = child->getX(), cy = child->getY();
                if (cx <= cullRight && cy <= cullTop && cx + child->getWidth() >= cullLeft && cy + child->getHeight() >= cullBottom) {
                    child->setX(cx + offsetX);
                    child->setX(cy + offsetY);
                    child->draw(batch, parentAlpha);
                    child->setX(cx);
                    child->setY(cy);
                }
            }
            setX(offsetX);
            setY(offsetY);
        }
    } else {
        // No culling, draw all children.
        if (_transform) {
            for (int i = 0, n = _children.size(); i < n; i++) {
                auto* child = _children[i];
                if (!child->isVisible()) continue;
                child->draw(batch, parentAlpha);
            }
        } else {
            // No transform for this group, offset each child.
            float offsetX = getX(), offsetY = getY();
            setX(0);
            setY(0);
            for (int i = 0, n = _children.size(); i < n; i++) {
                auto* child = _children[i];
                if (!child->isVisible()) continue;
                float cx = child->getX(), cy = child->getY();
                child->setX(cx + offsetX);
                child->setY(cy + offsetY);
                child->draw(batch, parentAlpha);
                child->setX(cx);
                child->setY(cy);
            }
            setX(offsetX);
            setY(offsetY);
        }
    }
}

void arc::Group::drawDebugChildren(arc::ShapeRenderer* shapes) {
    // No culling, draw all children.
    if (_transform) {
        for (int i = 0, n = _children.size(); i < n; i++) {
            auto* child = _children[i];
            if (!child->isVisible()) continue;
            if (!child->getDebug() && !(dynamic_cast<Group*>(child))) continue;
            child->drawDebug(shapes);
        }
        shapes->flush();
    } else {
        // No transform for this group, offset each child.
        float offsetX = getX(), offsetY = getY();
        setX(0);
        setY(0);
        for (int i = 0, n = _children.size(); i < n; i++) {
            auto* child = _children[i];
            if (!child->isVisible()) continue;
            if (!child->getDebug() && !(dynamic_cast<Group*>(child))) continue;
            float cx = child->getX(), cy = child->getY();
            child->setX(cx + offsetX);
            child->setY(cy + offsetY);
            child->drawDebug(shapes);
            child->setX(cx);
            child->setY(cy);
        }
        setX(offsetX);
        setY(offsetY);
    }
}

arc::Mat4 arc::Group::computeTransform() {

    _worldTransform.setToTrnRotScl(getX() + getOriginX(), getY() + getOriginY(), getRotation(), getScaleX(), getScaleY());
    if(getOriginX() != 0 || getOriginY() != 0) _worldTransform.translate(-getOriginX(), -getOriginY());

    // Find the first parent that transforms.
    auto* parentGroup = getParent();
    while (parentGroup != nullptr) {
        if (parentGroup->_transform) break;
        parentGroup = parentGroup->getParent();
    }
    if (parentGroup != nullptr) _worldTransform.preMul(parentGroup->_worldTransform);

    _computedTransform.set(_worldTransform);
    return _computedTransform;
}

void arc::Group::applyTransform(arc::SpriteBatch* batch, const arc::Mat4& transform) {
    _oldTransform = batch->getTransformMatrix();
    batch->setTransformMatrix(transform);
}

void arc::Group::resetTransform(arc::SpriteBatch* batch) {
    batch->setTransformMatrix(_oldTransform);
}

void arc::Group::applyTransform(arc::ShapeRenderer* shapes, const arc::Mat4& transform) {
    _oldTransform = shapes->getTransformMatrix();
    shapes->setTransformMatrix(transform);
    shapes->flush();
}

void arc::Group::resetTransform(arc::ShapeRenderer* shapes) {
    shapes->setTransformMatrix(_oldTransform);
}

void arc::Group::childrenChanged() {

}

void arc::Group::setStage(arc::Stage* stage) {
    Actor::setStage(stage);
    for (int i = 0, n = _children.size(); i < n; i++)
        _children[i]->setStage(stage); // StackOverflowError here means the group is its own ancestor.
}

bool arc::Group::isTransform() {
    return _transform;
}

void arc::Group::setTransform(bool transform)
{
    _transform = transform;
}

std::vector<arc::Actor*>& arc::Group::getChildren() {
    return _children;
}

arc::Group::Group() {

}

arc::Group::~Group() {
    for(auto* child : _children)
        delete child;
}
