#include "Cell.h"

#include "Table.h"

void arc::Cell::setLayout(arc::Table* table) {

}

void arc::Cell::set(arc::Cell* cell) {
    minWidth = cell->minWidth;
    minHeight = cell->minHeight;
    prefWidth = cell->prefWidth;
    prefHeight = cell->prefHeight;
    maxWidth = cell->maxWidth;
    maxHeight = cell->maxHeight;
    spaceTop = cell->spaceTop;
    spaceLeft = cell->spaceLeft;
    spaceBottom = cell->spaceBottom;
    spaceRight = cell->spaceRight;
    padTop = cell->padTop;
    padLeft = cell->padLeft;
    padBottom = cell->padBottom;
    padRight = cell->padRight;
    fillX = cell->fillX;
    fillY = cell->fillY;
    align = cell->align;
    expandX = cell->expandX;
    expandY = cell->expandY;
    colspan = cell->colspan;
    uniformX = cell->uniformX;
    uniformY = cell->uniformY;
}

void arc::Cell::merge(arc::Cell* cell) {
    if (cell == nullptr) return;
    if (cell->minWidth.has_value()) minWidth = cell->minWidth;
    if (cell->minHeight.has_value()) minHeight = cell->minHeight;
    if (cell->prefWidth.has_value()) prefWidth = cell->prefWidth;
    if (cell->prefHeight.has_value()) prefHeight = cell->prefHeight;
    if (cell->maxWidth.has_value()) maxWidth = cell->maxWidth;
    if (cell->maxHeight.has_value()) maxHeight = cell->maxHeight;
    if (cell->spaceTop.has_value()) spaceTop = cell->spaceTop;
    if (cell->spaceLeft.has_value()) spaceLeft = cell->spaceLeft;
    if (cell->spaceBottom.has_value()) spaceBottom = cell->spaceBottom;
    if (cell->spaceRight.has_value()) spaceRight = cell->spaceRight;
    if (cell->padTop.has_value()) padTop = cell->padTop;
    if (cell->padLeft.has_value()) padLeft = cell->padLeft;
    if (cell->padBottom.has_value()) padBottom = cell->padBottom;
    if (cell->padRight.has_value()) padRight = cell->padRight;

    if (cell->fillX.has_value()) fillX = cell->fillX;
    if (cell->fillY.has_value()) fillY = cell->fillY;
    if (cell->align.has_value()) align = cell->align;
    if (cell->expandX.has_value()) expandX = cell->expandX;
    if (cell->expandY.has_value()) expandY = cell->expandY;
    if (cell->colspan.has_value()) colspan = cell->colspan;
    if (cell->uniformX.has_value()) uniformX = cell->uniformX;
    if (cell->uniformY.has_value()) uniformY = cell->uniformY;
}

void arc::Cell::reset() {
    actor = nullptr;
    table = nullptr;
    endRow = false;
    cellAboveIndex = -1;
    defaults();
}

void arc::Cell::defaults() {
    minWidth = Table::minWidth;
    minHeight = Table::minHeight;
    prefWidth = Table::prefWidth;
    prefHeight = Table::prefHeight;
    maxWidth = Table::maxWidth;
    maxHeight = Table::maxHeight;
    spaceTop = Table::zero;
    spaceLeft = Table::zero;
    spaceBottom = Table::zero;
    spaceRight = Table::zero;
    padTop = Table::zero;
    padLeft = Table::zero;
    padBottom = Table::zero;
    padRight = Table::zero;
    fillX = 0.0f;
    fillY = 0.0f;
    align = 1;
    expandX = 0;
    expandY = 0;
    colspan = 1;
    uniformX = {};
    uniformY = {};
}

void arc::Cell::clear()
{
    minWidth = {};
    minHeight = {};
    prefWidth = {};
    prefHeight = {};
    maxWidth = {};
    maxHeight = {};
    spaceTop = {};
    spaceLeft = {};
    spaceBottom = {};
    spaceRight = {};
    padTop = {};
    padLeft = {};
    padBottom = {};
    padRight = {};
    fillX = {};
    fillY = {};
    align = {};
    expandX = {};
    expandY = {};
    colspan = {};
    uniformX = {};
    uniformY = {};
}

arc::Cell& arc::Cell::minSize(const arc::NewValue& value) {
    minWidth = value;
    minHeight = value;
    return *this;
}

arc::Cell& arc::Cell::minSize(float size) {
    return minSize(NewValue::fixed(size));
}

arc::Cell& arc::Cell::pad(float value) {
    return pad(NewValue::fixed(value));
}

arc::Cell& arc::Cell::pad(const arc::NewValue& pad) {
    padTop = pad;
    padLeft = pad;
    padBottom = pad;
    padRight = pad;
    return *this;
}


arc::Cell::Cell() {
    cellAboveIndex = -1;
    defaults();
}

void arc::Cell::setActorBounds(float x, float y, float width, float height) {
    actorX = x;
    actorY = y;
    actorWidth = width;
    actorHeight = height;
}

void arc::Cell::setActorX(float x) {
    actorX = x;
}

void arc::Cell::setActorY(float y) {
    actorY = y;
}
