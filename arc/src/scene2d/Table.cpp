#include "Table.h"

arc::FixedValue* arc::Table::zero = new arc::FixedValue(0.0f);
arc::Value* arc::Table::minWidth = new arc::MinWidthValue();
arc::Value* arc::Table::maxWidth = new arc::MaxWidthValue();
arc::Value* arc::Table::prefWidth = new arc::PrefWidthValue();
arc::Value* arc::Table::minHeight = new arc::MinHeightValue();
arc::Value* arc::Table::maxHeight = new arc::MaxHeightValue();
arc::Value* arc::Table::prefHeight = new arc::PrefHeightValue();

arc::Value* arc::Table::backgroundTop = new arc::BackgroundTopValue();
arc::Value* arc::Table::backgroundLeft = new arc::BackgroundLeftValue();
arc::Value* arc::Table::backgroundBottom = new arc::BackgroundBottomValue();
arc::Value* arc::Table::backgroundRight = new arc::BackgroundRightValue();


arc::FixedValue::FixedValue(float value) : _value(value) {

}
float arc::FixedValue::get(arc::Actor* context) {
    return _value;
}
float arc::BackgroundTopValue::get(arc::Actor* context) {
    auto* background = dynamic_cast<Table*>(context)->getBackground();
    return background == nullptr ? 0 : background->getTopHeight();
}
float arc::BackgroundLeftValue::get(arc::Actor* context) {
    auto* background = dynamic_cast<Table*>(context)->getBackground();
    return background == nullptr ? 0 : background->getLeftWidth();
}
float arc::BackgroundBottomValue::get(arc::Actor* context) {
    auto* background = dynamic_cast<Table*>(context)->getBackground();
    return background == nullptr ? 0 : background->getBottomHeight();
}
float arc::BackgroundRightValue::get(arc::Actor* context) {
    auto* background = dynamic_cast<Table*>(context)->getBackground();
    return background == nullptr ? 0 : background->getRightWidth();
}
float arc::MinWidthValue::get(arc::Actor* context) {
    if (auto* layout = dynamic_cast<ILayout*>(context))
        return layout->getMinWidth();
    return context == nullptr ? 0 : context->getWidth();
}
float arc::MinHeightValue::get(arc::Actor* context) {
    if (auto* layout = dynamic_cast<ILayout*>(context))
        return layout->getMinHeight();
    return context == nullptr ? 0 : context->getHeight();
}
float arc::PrefWidthValue::get(arc::Actor* context) {
    if (auto* layout = dynamic_cast<ILayout*>(context))
        return layout->getPrefWidth();
    return context == nullptr ? 0 : context->getWidth();
}
float arc::PrefHeightValue::get(arc::Actor* context) {
    if (auto* layout = dynamic_cast<ILayout*>(context))
        return layout->getPrefHeight();
    return context == nullptr ? 0 : context->getHeight();
}
float arc::MaxWidthValue::get(arc::Actor* context) {
    if (auto* layout = dynamic_cast<ILayout*>(context))
        return layout->getMaxWidth();
    return context == nullptr ? 0 : context->getWidth();
}
float arc::MaxHeightValue::get(arc::Actor* context) {
    if (auto* layout = dynamic_cast<ILayout*>(context))
        return layout->getMaxHeight();
    return context == nullptr ? 0 : context->getHeight();
}

arc::IDrawable* arc::Table::getBackground() {
    return background;
}

arc::Table::Table(arc::Skin* skin) : skin(skin) {

    cellDefaults = pool.new_object();
    cellDefaults->reset();
    cellDefaults->setLayout(this);
    setTransform(false);
    setTouchable(Touchable::childrenOnly);
}

arc::Table::~Table() {
    //delete padTop;
    //delete padLeft;
    //delete padBottom;
    //delete padRight;
}

void arc::Table::draw(arc::SpriteBatch* batch, float parentAlpha) {
    validate();
    if (isTransform()) {
        applyTransform(batch, computeTransform());
        drawBackground(batch, parentAlpha, 0, 0);
        if (clip) {
            batch->flush();
            float padLeft = this->padLeft.get(this);
            float padBottom = this->padBottom.get(this);
            if (clipBegin(padLeft, padBottom, getWidth() - padLeft - padRight.get(this),
                          getHeight() - padBottom - padTop.get(this))) {
                drawChildren(batch, parentAlpha);
                batch->flush();
                clipEnd();
            }
        } else
            drawChildren(batch, parentAlpha);
        resetTransform(batch);
    } else {
        drawBackground(batch, parentAlpha, getX(), getY());
        arc::WidgetGroup::draw(batch, parentAlpha);
    }
}

void arc::Table::drawBackground(arc::SpriteBatch* batch, float parentAlpha, float x, float y)
{
    if (background == nullptr) return;
    auto color = getColor();
    batch->setColor({color.r, color.g, color.b, color.a * parentAlpha});
    background->draw(batch, x, y, getWidth(), getHeight());
}

void arc::Table::setBackground(arc::IDrawable* background) {
    if(this->background == background) return;
    float padTopOld = getPadTop(), padLeftOld = getPadLeft(), padBottomOld = getPadBottom(), padRightOld = getPadRight();
    this->background = background; // The default pad values use the background's padding.
    float padTopNew = getPadTop(), padLeftNew = getPadLeft(), padBottomNew = getPadBottom(), padRightNew = getPadRight();
    if (padTopOld + padBottomOld != padTopNew + padBottomNew || padLeftOld + padRightOld != padLeftNew + padRightNew)
        invalidateHierarchy();
    else if (padTopOld != padTopNew || padLeftOld != padLeftNew || padBottomOld != padBottomNew || padRightOld != padRightNew)
        invalidate();
}

arc::Actor* arc::Table::hit(float x, float y, bool touchable) {
    if (clip) {
        if (touchable && getTouchable() == Touchable::disabled) return nullptr;
        if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight()) return nullptr;
    }
    return Group::hit(x, y, touchable);
}

void arc::Table::setClip(bool enabled) {
    clip = enabled;
    setTransform(enabled);
    invalidate();
}

bool arc::Table::getClip() {
    return clip;
}

void arc::Table::invalidate() {
    sizeInvalid = true;
    WidgetGroup::invalidate();
}

arc::Cell& arc::Table::add(arc::Actor* actor) {
    auto* cell = pool.new_object();
    cell->reset();
    cell->setLayout(this);
    cell->actor = actor;

    // The row was ended for layout, not by the user, so revert it.
    if (implicitEndRow) {
        implicitEndRow = false;
        rows--;
        cells.back()->endRow = false;
    }

    // todo: inside it's using std::optional, check if it is safe to call .value()
    int cellCount = cells.size();
    if (cellCount > 0) {
        // Set cell column and row.
        auto* lastCell = cells.back();
        if (!lastCell->endRow) {
            cell->column = lastCell->column + lastCell->colspan.value();
            cell->row = lastCell->row;
        } else {
            cell->column = 0;
            cell->row = lastCell->row + 1;
        }
        // Set the index of the cell above.
        if (cell->row > 0) {
            outer:
            for (int i = cellCount - 1; i >= 0; i--) {
                auto* other = cells[i];
                for (int column = other->column, nn = column + other->colspan.value(); column < nn; column++) {
                    if (column == cell->column) {
                        cell->cellAboveIndex = i;
                        //break outer;
                        goto outer; // todo: is it similar to above ?
                    }
                }
            }
        }
    } else {
        cell->column = 0;
        cell->row = 0;
    }
    cells.emplace_back(cell);

    cell->set(cellDefaults);
    if (cell->column < columnDefaults.size()) {
        auto columnCell = columnDefaults[cell->column];
        if (columnCell != nullptr) cell->merge(columnCell);
    }
    cell->merge(rowDefaults);

    if (actor != nullptr) addActor(actor);

    return *cell;
}

void arc::Table::clearChildren() {

    for (int i = cells.size() - 1; i >= 0; i--) {
        auto* cell = cells[i];
        auto* actor = cell->actor;
        if (actor != nullptr) actor->remove();
    }
    pool.delete_all();
    cells.clear();
    rows = 0;
    columns = 0;
    rowDefaults = nullptr;
    implicitEndRow = false;

    Group::clearChildren();
}

void arc::Table::reset() {
    clearChildren();
    padTop = *backgroundTop;
    padLeft = *backgroundLeft;
    padBottom = *backgroundBottom;
    padRight = *backgroundRight;
    align = (int)Align::center;
    debug(TableDebug::none);
    cellDefaults->reset();
    for (int i = 0, n = columnDefaults.size(); i < n; i++) {
        auto* columnCell = columnDefaults[i];
        if (columnCell != nullptr) pool.delete_object(columnCell);
    }
    columnDefaults.clear();
}

arc::Table& arc::Table::debug(arc::TableDebug debug) {
    arc::Actor::setDebug(debug != TableDebug::none);
    if(_debug != debug)
    {
        _debug = debug;
        if(debug == TableDebug::none)
            clearDebugRects();
        else
            invalidate();
    }
    return *this;
}

void arc::Table::clearDebugRects() {
    if(debugRects.empty()) return;
    debugRects.clear();
    // todo: maybe pool ?
}

float arc::Table::getPadTop() {
    return padTop.get(this);
}

float arc::Table::getPadLeft() {
    return padLeft.get(this);
}

float arc::Table::getPadBottom() {
    return padBottom.get(this);
}

float arc::Table::getPadRight() {
    return padRight.get(this);
}

arc::Cell& arc::Table::row() {
    if (cells.size() > 0) {
        if (!implicitEndRow)
        {
            if (cells.back()->endRow)
                return *rowDefaults; // Row was already ended.
            endRow();
        }
        invalidate();
    }
    implicitEndRow = false;
    if (rowDefaults != nullptr) pool.delete_object(rowDefaults);
    rowDefaults = pool.new_object();
    rowDefaults->reset();
    rowDefaults->setLayout(this);
    rowDefaults->clear();
    return *rowDefaults;
}

void arc::Table::endRow() {
    int rowColumns = 0;
    for (int i = cells.size() - 1; i >= 0; i--) {
        auto* cell = cells[i];
        if (cell->endRow) break;
        rowColumns += cell->colspan.value();
    }
    columns = Mathf::max(columns, rowColumns);
    rows++;
    cells.back()->endRow = true;

}
