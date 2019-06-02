#include "Cell.h"
#include "Table.h"

arc::Cell* arc::Cell::_defaults = nullptr;

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
    if (cell->minWidth != nullptr) minWidth = cell->minWidth;
    if (cell->minHeight != nullptr) minHeight = cell->minHeight;
    if (cell->prefWidth != nullptr) prefWidth = cell->prefWidth;
    if (cell->prefHeight != nullptr) prefHeight = cell->prefHeight;
    if (cell->maxWidth != nullptr) maxWidth = cell->maxWidth;
    if (cell->maxHeight != nullptr) maxHeight = cell->maxHeight;
    if (cell->spaceTop != nullptr) spaceTop = cell->spaceTop;
    if (cell->spaceLeft != nullptr) spaceLeft = cell->spaceLeft;
    if (cell->spaceBottom != nullptr) spaceBottom = cell->spaceBottom;
    if (cell->spaceRight != nullptr) spaceRight = cell->spaceRight;
    if (cell->padTop != nullptr) padTop = cell->padTop;
    if (cell->padLeft != nullptr) padLeft = cell->padLeft;
    if (cell->padBottom != nullptr) padBottom = cell->padBottom;
    if (cell->padRight != nullptr) padRight = cell->padRight;

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
    auto* d = defaults();
    if(d != nullptr)
        set(d);
}

arc::Cell* arc::Cell::defaults() {
    if(arc::Cell::_defaults == nullptr)
    {
        arc::Cell::_defaults = new Cell();
        arc::Cell::_defaults->reset();
    }

    arc::Cell::_defaults->minWidth = Table::minWidth;
    arc::Cell::_defaults->minHeight = Table::minHeight;
    arc::Cell::_defaults->prefWidth = Table::prefWidth;
    arc::Cell::_defaults->prefHeight = Table::prefHeight;
    arc::Cell::_defaults->maxWidth = Table::maxWidth;
    arc::Cell::_defaults->maxHeight = Table::maxHeight;
    arc::Cell::_defaults->spaceTop = Table::zero;
    arc::Cell::_defaults->spaceLeft = Table::zero;
    arc::Cell::_defaults->spaceBottom = Table::zero;
    arc::Cell::_defaults->spaceRight = Table::zero;
    arc::Cell::_defaults->padTop = Table::zero;
    arc::Cell::_defaults->padLeft = Table::zero;
    arc::Cell::_defaults->padBottom = Table::zero;
    arc::Cell::_defaults->padRight = Table::zero;
    arc::Cell::_defaults->fillX = 0.0f;
    arc::Cell::_defaults->fillY = 0.0f;
    arc::Cell::_defaults->align = 1;
    arc::Cell::_defaults->expandX = 0;
    arc::Cell::_defaults->expandY = 0;
    arc::Cell::_defaults->colspan = 1;
    arc::Cell::_defaults->uniformX = nullptr;
    arc::Cell::_defaults->uniformY = nullptr;

    return arc::Cell::_defaults;
}

void arc::Cell::clear()
{
    minWidth = nullptr;
    minHeight = nullptr;
    prefWidth = nullptr;
    prefHeight = nullptr;
    maxWidth = nullptr;
    maxHeight = nullptr;
    spaceTop = nullptr;
    spaceLeft = nullptr;
    spaceBottom = nullptr;
    spaceRight = nullptr;
    padTop = nullptr;
    padLeft = nullptr;
    padBottom = nullptr;
    padRight = nullptr;
    fillX = {};
    fillY = {};
    align = {};
    expandX = {};
    expandY = {};
    colspan = {};
    uniformX = nullptr;
    uniformY = nullptr;
}

arc::Cell& arc::Cell::minSize(arc::Value* value) {
    if(minWidth) delete minWidth;
    if(minHeight) delete minHeight;
    minWidth = value;
    minHeight = value;
    return *this;
}

arc::Cell& arc::Cell::minSize(float size) {
    return minSize(new FixedValue(size));
}

arc::Cell::Cell() {
}
