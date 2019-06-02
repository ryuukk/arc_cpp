#include "Cell.h"

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
