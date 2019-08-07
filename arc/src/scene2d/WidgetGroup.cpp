#include "WidgetGroup.h"
#include "Stage.h"

void arc::WidgetGroup::layout() {

}

void arc::WidgetGroup::invalidate() {
    _needsLayout = true;
}

void arc::WidgetGroup::invalidateHierarchy() {
    invalidate();
    auto* parent = getParent();
    if (ILayout* layout = dynamic_cast<ILayout*>(parent)) layout->invalidateHierarchy();
}

void arc::WidgetGroup::validate() {
    if (!_layoutEnabled) return;

    auto parent = getParent();
    if (_fillParent && parent != nullptr) {
        float parentWidth, parentHeight;
        auto* stage = getStage();
        if (stage != nullptr && parent == &stage->getRoot()) {
            parentWidth = stage->getWidth();
            parentHeight = stage->getHeight();
        } else {
            parentWidth = parent->getWidth();
            parentHeight = parent->getHeight();
        }
        if (getWidth() != parentWidth || getHeight() != parentHeight) {
            setSize(parentWidth, parentHeight);
            invalidate();
        }
    }

    if (!_needsLayout) return;
    _needsLayout = false;
    layout();

    // Widgets may call invalidateHierarchy during layout (eg, a wrapped label). The root-most widget group retries layout a
    // reasonable number of times.
    if (_needsLayout) {
        if (dynamic_cast<WidgetGroup*>(parent)) return; // The parent widget will layout again.
        for (int i = 0; i < 5; i++) {
            _needsLayout = false;
            layout();
            if (!_needsLayout) break;
        }
    }
}

void arc::WidgetGroup::pack() {
    setSize(getPrefWidth(), getPrefHeight());
    validate();
    // Validating the layout may change the pref size. Eg, a wrapped label doesn't know its pref height until it knows its
    // width, so it calls invalidateHierarchy() in layout() if its pref height has changed.
    setSize(getPrefWidth(), getPrefHeight());
    validate();
}

void arc::WidgetGroup::setFillParent(bool fillParent) {
    _fillParent = fillParent;
}

void arc::WidgetGroup::setLayoutEnabled(bool enabled) {
    if (_layoutEnabled == enabled) return;
    _layoutEnabled = enabled;
    setLayoutEnabled(this, enabled);
}

float arc::WidgetGroup::getMinWidth() {
    return getPrefWidth();
}

float arc::WidgetGroup::getMinHeight() {
    return getPrefHeight();
}

float arc::WidgetGroup::getPrefWidth() {
    return 0;
}

float arc::WidgetGroup::getPrefHeight() {
    return 0;
}

float arc::WidgetGroup::getMaxWidth() {
    return 0;
}

float arc::WidgetGroup::getMaxHeight() {
    return 0;
}

bool arc::WidgetGroup::needsLayout() {
    return _needsLayout;
}

void arc::WidgetGroup::draw(arc::SpriteBatch* batch, float parentAlpha) {
    validate();
    Group::draw(batch, parentAlpha);
}

void arc::WidgetGroup::setLayoutEnabled(arc::Group* parent, bool enabled) {
    auto& children = parent->getChildren();
    for (int i = 0, n = children.size(); i < n; i++) {
        auto* actor = children[i];
        if (auto* layout = dynamic_cast<ILayout*>(actor))
            layout->setLayoutEnabled(enabled);
        else if (auto* group = dynamic_cast<Group*>(actor)) //
            setLayoutEnabled(group, enabled);
    }
}

void arc::WidgetGroup::childrenChanged() {
    invalidateHierarchy();
}

void arc::WidgetGroup::sizeChanged() {
    invalidate();
}
