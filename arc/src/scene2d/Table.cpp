#include "Table.h"

arc::NewValue arc::Table::zero = arc::NewValue::fixed(0.0f);
arc::NewValue arc::Table::minWidth = arc::NewValue::minWidth();
arc::NewValue arc::Table::maxWidth = arc::NewValue::maxWidth();
arc::NewValue arc::Table::prefWidth = arc::NewValue::prefWidth();
arc::NewValue arc::Table::minHeight = arc::NewValue::minHeight();
arc::NewValue arc::Table::maxHeight = arc::NewValue::maxHeight();
arc::NewValue arc::Table::prefHeight = arc::NewValue::prefHeight();

arc::NewValue arc::Table::backgroundTop = arc::NewValue::backgroundTop();
arc::NewValue arc::Table::backgroundLeft = arc::NewValue::backgroundLeft();
arc::NewValue arc::Table::backgroundBottom = arc::NewValue::backgroundBottom();
arc::NewValue arc::Table::backgroundRight = arc::NewValue::backgroundRight();

std::vector<float> arc::Table::columnWeightedWidth = {};
std::vector<float> arc::Table::rowWeightedHeight = {};
DynamicObjectPool<arc::Cell> arc::Table::pool{64};


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
            outer:
            {};
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
    padTop = backgroundTop;
    padLeft = backgroundLeft;
    padBottom = backgroundBottom;
    padRight = backgroundRight;
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


float arc::Table::getMinWidth() {
    if (sizeInvalid) computeSize();
    return tableMinWidth;
}

float arc::Table::getMinHeight() {
    if (sizeInvalid) computeSize();
    return tableMinHeight;
}

float arc::Table::getPrefWidth() {
    if (sizeInvalid) computeSize();
    float width = tablePrefWidth;
    if (background != nullptr) return Mathf::max(width, background->getMinWidth());
    return width;
}
float arc::Table::getPrefHeight() {
    if (sizeInvalid) computeSize();
    float height = tablePrefHeight;
    if (background != nullptr) return Mathf::max(height, background->getMinHeight());
    return height;
}

void arc::Table::computeSize() {
    sizeInvalid = false;

    int cellCount = cells.size();
    if(cellCount > 0 && !cells.back()->endRow)
    {
        endRow();
        implicitEndRow = true;
    }
    columnMinWidth.resize(columns);
    rowMinHeight.resize(rows);
    columnPrefWidth.resize(columns);
    rowPrefHeight.resize(rows);
    columnWidth.resize(columns);
    rowHeight.resize(rows);
    expandWidth.resize(columns);
    expandHeight.resize(rows);


    float spaceRightLast = 0;
    for (int i = 0; i < cellCount; i++)
    {
        auto* c = cells[i];
        int column = c->column;
        int row = c->row;
        int colspan = c->colspan.value();

        auto* a = c->actor;

        // Collect rows that expand and colspan=1 columns that expand.
        if (c->expandY != 0 && expandHeight[row] == 0) expandHeight[row] = c->expandY.value();
        if (colspan == 1 && c->expandX != 0 && expandWidth[column] == 0) expandWidth[column] = c->expandX.value();


        // Compute combined padding/spacing for cells.
        // Spacing between actors isn't additive, the larger is used. Also, no spacing around edges.
        c->computedPadLeft = c->padLeft->get(a) + (column == 0 ? 0 : Mathf::max(0.0f, c->spaceLeft->get(a) - spaceRightLast));
        c->computedPadTop = c->padTop->get(a);
        if (c->cellAboveIndex != -1) {
            auto* above = cells[c->cellAboveIndex];
            c->computedPadTop += Mathf::max(0.0f, c->spaceTop->get(a) - above->spaceBottom->get(a));
        }
        float spaceRight = c->spaceRight->get(a);
        c->computedPadRight = c->padRight->get(a) + ((column + colspan) == columns ? 0 : spaceRight);
        c->computedPadBottom = c->padBottom->get(a) + (row == rows - 1 ? 0 : c->spaceBottom->get(a));
        spaceRightLast = spaceRight;

        // Determine minimum and preferred cell sizes.
        float prefWidth = c->prefWidth->get(a);
        float prefHeight = c->prefHeight->get(a);
        float minWidth = c->minWidth->get(a);
        float minHeight = c->minHeight->get(a);
        float maxWidth = c->maxWidth->get(a);
        float maxHeight = c->maxHeight->get(a);
        if (prefWidth < minWidth) prefWidth = minWidth;
        if (prefHeight < minHeight) prefHeight = minHeight;
        if (maxWidth > 0 && prefWidth > maxWidth) prefWidth = maxWidth;
        if (maxHeight > 0 && prefHeight > maxHeight) prefHeight = maxHeight;

        if (colspan == 1) { // Spanned column min and pref width is added later.
            float hpadding = c->computedPadLeft + c->computedPadRight;
            columnPrefWidth[column] = Mathf::max(columnPrefWidth[column], prefWidth + hpadding);
            columnMinWidth[column] = Mathf::max(columnMinWidth[column], minWidth + hpadding);
        }
        float vpadding = c->computedPadTop + c->computedPadBottom;
        rowPrefHeight[row] = Mathf::max(rowPrefHeight[row], prefHeight + vpadding);
        rowMinHeight[row] = Mathf::max(rowMinHeight[row], minHeight + vpadding);
    }
    float uniformMinWidth = 0, uniformMinHeight = 0;
    float uniformPrefWidth = 0, uniformPrefHeight = 0;
    for (int i = 0; i < cellCount; i++)
    {
        auto* c = cells[i];
        int column = c->column;

        // Colspan with expand will expand all spanned columns if none of the spanned columns have expand.
        int expandX = c->expandX.value();
        if (expandX != 0) {
            int nn = column + c->colspan.value();
            for (int ii = column; ii < nn; ii++)
                if (expandWidth[ii] != 0) goto outer;
            for (int ii = column; ii < nn; ii++)
                expandWidth[ii] = expandX;
        }
        outer:
        {};

        // Collect uniform sizes.
        if (c->uniformX.has_value() && c->colspan.value() == 1) {
            float hpadding = c->computedPadLeft + c->computedPadRight;
            uniformMinWidth = Mathf::max(uniformMinWidth, columnMinWidth[column] - hpadding);
            uniformPrefWidth = Mathf::max(uniformPrefWidth, columnPrefWidth[column] - hpadding);
        }
        if (c->uniformY.has_value()) {
            float vpadding = c->computedPadTop + c->computedPadBottom;
            uniformMinHeight = Mathf::max(uniformMinHeight, rowMinHeight[c->row] - vpadding);
            uniformPrefHeight = Mathf::max(uniformPrefHeight, rowPrefHeight[c->row] - vpadding);
        }
    }

    // Size uniform cells to the same width/height.
    if (uniformPrefWidth > 0 || uniformPrefHeight > 0) {
        for (int i = 0; i < cellCount; i++) {
            auto* c = cells[i];
            if (uniformPrefWidth > 0 && c->uniformX.has_value() && c->colspan.value() == 1) {
                float hpadding = c->computedPadLeft + c->computedPadRight;
                columnMinWidth[c->column] = uniformMinWidth + hpadding;
                columnPrefWidth[c->column] = uniformPrefWidth + hpadding;
            }
            if (uniformPrefHeight > 0 && c->uniformY.has_value()) {
                float vpadding = c->computedPadTop + c->computedPadBottom;
                rowMinHeight[c->row] = uniformMinHeight + vpadding;
                rowPrefHeight[c->row] = uniformPrefHeight + vpadding;
            }
        }
    }

    // Distribute any additional min and pref width added by colspanned cells to the columns spanned.
    for (int i = 0; i < cellCount; i++) {
        auto* c = cells[i];
        int colspan = c->colspan.value();
        if (colspan == 1) continue;
        int column = c->column;

        auto* a = c->actor;
        float minWidth = c->minWidth->get(a);
        float prefWidth = c->prefWidth->get(a);
        float maxWidth = c->maxWidth->get(a);
        if (prefWidth < minWidth) prefWidth = minWidth;
        if (maxWidth > 0 && prefWidth > maxWidth) prefWidth = maxWidth;

        float spannedMinWidth = -(c->computedPadLeft + c->computedPadRight), spannedPrefWidth = spannedMinWidth;
        float totalExpandWidth = 0;
        for (int ii = column, nn = ii + colspan; ii < nn; ii++) {
            spannedMinWidth += columnMinWidth[ii];
            spannedPrefWidth += columnPrefWidth[ii];
            totalExpandWidth += expandWidth[ii]; // Distribute extra space using expand, if any columns have expand.
        }

        float extraMinWidth = Mathf::max(0.0f, minWidth - spannedMinWidth);
        float extraPrefWidth = Mathf::max(0.0f, prefWidth - spannedPrefWidth);
        for (int ii = column, nn = ii + colspan; ii < nn; ii++) {
            float ratio = totalExpandWidth == 0 ? 1.0f / colspan : expandWidth[ii] / totalExpandWidth;
            columnMinWidth[ii] += extraMinWidth * ratio;
            columnPrefWidth[ii] += extraPrefWidth * ratio;
        }
    }

    // Determine table min and pref size.
    tableMinWidth = 0;
    tableMinHeight = 0;
    tablePrefWidth = 0;
    tablePrefHeight = 0;
    for (int i = 0; i < columns; i++) {
        tableMinWidth += columnMinWidth[i];
        tablePrefWidth += columnPrefWidth[i];
    }
    for (int i = 0; i < rows; i++) {
        tableMinHeight += rowMinHeight[i];
        tablePrefHeight += Mathf::max(rowMinHeight[i], rowPrefHeight[i]);
    }
    float hpadding = padLeft.get(this) + padRight.get(this);
    float vpadding = padTop.get(this) + padBottom.get(this);
    tableMinWidth = tableMinWidth + hpadding;
    tableMinHeight = tableMinHeight + vpadding;
    tablePrefWidth = Mathf::max(tablePrefWidth + hpadding, tableMinWidth);
    tablePrefHeight = Mathf::max(tablePrefHeight + vpadding, tableMinHeight);


}

void arc::Table::layout() {
    float width = getWidth();
    float height = getHeight();

    printf("layout on table: %f:%f %llu\n", width, height, cells.size());
    layout(0, 0, width, height);
    if (round) {
        for (int i = 0, n = cells.size(); i < n; i++) {
            auto* c = cells[i];
            float actorWidth = Mathf::round(c->actorWidth);
            float actorHeight = Mathf::round(c->actorHeight);
            float actorX = Mathf::round(c->actorX);
            float actorY = height - Mathf::round(c->actorY) - actorHeight;
            c->setActorBounds(actorX, actorY, actorWidth, actorHeight);
            auto* actor = c->actor;
            if (actor != nullptr) actor->setBounds(actorX, actorY, actorWidth, actorHeight);
        }
    } else {
        for (int i = 0, n = cells.size(); i < n; i++) {
            auto* c = cells[i];
            float actorHeight = c->actorHeight;
            float actorY = height - c->actorY - actorHeight;
            c->setActorY(actorY);
            auto* actor = c->actor;
            if (actor != nullptr) actor->setBounds(c->actorX, actorY, c->actorWidth, actorHeight);
        }
    }
    // Validate children separately from sizing actors to ensure actors without a cell are validated.
    auto& children = getChildren();
    for (int i = 0, n = children.size(); i < n; i++) {
        auto* child = children[i];
        if (auto* layout = dynamic_cast<ILayout*>(child)) layout->validate();
    }
}

void arc::Table::layout(float layoutX, float layoutY, float layoutWidth, float layoutHeight) {
    int cellCount = cells.size();

    if (sizeInvalid) computeSize();

    float padLeft = this->padLeft.get(this);
    float hpadding = padLeft + padRight.get(this);
    float padTop = this->padTop.get(this);
    float vpadding = padTop + padBottom.get(this);

    float totalExpandWidth = 0, totalExpandHeight = 0;
    for (int i = 0; i < columns; i++)
        totalExpandWidth += expandWidth[i];
    for (int i = 0; i < rows; i++)
        totalExpandHeight += expandHeight[i];

    // Size columns and rows between min and pref size using (preferred - min) size to weight distribution of extra space.
    std::vector<float>& columnWeightedWidth = columnMinWidth;

    float totalGrowWidth = tablePrefWidth - tableMinWidth;
    if (totalGrowWidth == 0)
        columnWeightedWidth = columnMinWidth;
    else {
        float extraWidth = Mathf::min(totalGrowWidth, Mathf::max(0.0f, layoutWidth - tableMinWidth));

        arc::Table::columnWeightedWidth.resize(columns);
        columnWeightedWidth = arc::Table::columnWeightedWidth;

        for (int i = 0; i < columns; i++) {
            float growWidth = columnPrefWidth[i] - columnMinWidth[i];
            float growRatio = growWidth / totalGrowWidth;
            columnWeightedWidth[i] = columnMinWidth[i] + extraWidth * growRatio;
        }
    }

    auto& rowWeightedHeight = rowMinHeight;
    float totalGrowHeight = tablePrefHeight - tableMinHeight;
    if (totalGrowHeight == 0)
        rowWeightedHeight = rowMinHeight;
    else {
        Table::rowWeightedHeight.resize(rows);
        rowWeightedHeight = Table::rowWeightedHeight;
        float extraHeight = Mathf::min(totalGrowHeight, Mathf::max(0.0f, layoutHeight - tableMinHeight));
        for (int i = 0; i < rows; i++) {
            float growHeight = rowPrefHeight[i] - rowMinHeight[i];
            float growRatio = growHeight / totalGrowHeight;
            rowWeightedHeight[i] = rowMinHeight[i] + extraHeight * growRatio;
        }
    }
    // Determine actor and cell sizes (before expand or fill).
    for (int i = 0; i < cellCount; i++) {
        auto* c = cells[i];
        int column = c->column, row = c->row;
        auto* a = c->actor;

        float spannedWeightedWidth = 0;
        int colspan = c->colspan.value();
        for (int ii = column, nn = ii + colspan; ii < nn; ii++)
            spannedWeightedWidth += columnWeightedWidth[ii];
        float weightedHeight = rowWeightedHeight[row];

        float prefWidth = c->prefWidth->get(a);
        float prefHeight = c->prefHeight->get(a);
        float minWidth = c->minWidth->get(a);
        float minHeight = c->minHeight->get(a);
        float maxWidth = c->maxWidth->get(a);
        float maxHeight = c->maxHeight->get(a);
        if (prefWidth < minWidth) prefWidth = minWidth;
        if (prefHeight < minHeight) prefHeight = minHeight;
        if (maxWidth > 0 && prefWidth > maxWidth) prefWidth = maxWidth;
        if (maxHeight > 0 && prefHeight > maxHeight) prefHeight = maxHeight;

        c->actorWidth = Mathf::min(spannedWeightedWidth - c->computedPadLeft - c->computedPadRight, prefWidth);
        c->actorHeight = Mathf::min(weightedHeight - c->computedPadTop - c->computedPadBottom, prefHeight);

        if (colspan == 1) columnWidth[column] = Mathf::max(columnWidth[column], spannedWeightedWidth);
        rowHeight[row] = Mathf::max(rowHeight[row], weightedHeight);
    }

    // Distribute remaining space to any expanding columns/rows.
    if (totalExpandWidth > 0) {
        float extra = layoutWidth - hpadding;
        for (int i = 0; i < columns; i++)
            extra -= columnWidth[i];
        if (extra > 0) { // layoutWidth < tableMinWidth.
            float used = 0;
            int lastIndex = 0;
            for (int i = 0; i < columns; i++) {
                if (expandWidth[i] == 0) continue;
                float amount = extra * expandWidth[i] / totalExpandWidth;
                columnWidth[i] += amount;
                used += amount;
                lastIndex = i;
            }
            columnWidth[lastIndex] += extra - used;
        }
    }
    if (totalExpandHeight > 0) {
        float extra = layoutHeight - vpadding;
        for (int i = 0; i < rows; i++)
            extra -= rowHeight[i];
        if (extra > 0) { // layoutHeight < tableMinHeight.
            float used = 0;
            int lastIndex = 0;
            for (int i = 0; i < rows; i++) {
                if (expandHeight[i] == 0) continue;
                float amount = extra * expandHeight[i] / totalExpandHeight;
                rowHeight[i] += amount;
                used += amount;
                lastIndex = i;
            }
            rowHeight[lastIndex] += extra - used;
        }
    }

    // Distribute any additional width added by colspanned cells to the columns spanned.
    for (int i = 0; i < cellCount; i++) {
        auto* c = cells[i];
        int colspan = c->colspan.value();
        if (colspan == 1) continue;

        float extraWidth = 0;
        for (int column = c->column, nn = column + colspan; column < nn; column++)
            extraWidth += columnWeightedWidth[column] - columnWidth[column];
        extraWidth -= Mathf::max(0.0f, c->computedPadLeft + c->computedPadRight);

        extraWidth /= colspan;
        if (extraWidth > 0) {
            for (int column = c->column, nn = column + colspan; column < nn; column++)
                columnWidth[column] += extraWidth;
        }
    }

    // Determine table size.
    float tableWidth = hpadding, tableHeight = vpadding;
    for (int i = 0; i < columns; i++)
        tableWidth += columnWidth[i];
    for (int i = 0; i < rows; i++)
        tableHeight += rowHeight[i];

    // Position table within the container.
    int align = this->align;
    float x = layoutX + padLeft;
    if ((align & (int)Align::right) != 0)
        x += layoutWidth - tableWidth;
    else if ((align & (int)Align::left) == 0) // Center
        x += (layoutWidth - tableWidth) / 2;

    float y = layoutY + padTop;
    if ((align & (int)Align::bottom) != 0)
        y += layoutHeight - tableHeight;
    else if ((align & (int)Align::top) == 0) // Center
        y += (layoutHeight - tableHeight) / 2;

    // Position actors within cells.
    float currentX = x, currentY = y;
    for (int i = 0; i < cellCount; i++) {
        auto* c = cells[i];

        float spannedCellWidth = 0;
        for (int column = c->column, nn = column + c->colspan.value(); column < nn; column++)
            spannedCellWidth += columnWidth[column];
        spannedCellWidth -= c->computedPadLeft + c->computedPadRight;

        currentX += c->computedPadLeft;

        float fillX = c->fillX.value(), fillY = c->fillY.value();
        if (fillX > 0) {
            c->actorWidth = Mathf::max(spannedCellWidth * fillX, c->minWidth->get(c->actor));
            float maxWidth = c->maxWidth->get(c->actor);
            if (maxWidth > 0) c->actorWidth = Mathf::min(c->actorWidth, maxWidth);
        }
        if (fillY > 0) {
            c->actorHeight = Mathf::max(rowHeight[c->row] * fillY - c->computedPadTop - c->computedPadBottom, c->minHeight->get(c->actor));
            float maxHeight = c->maxHeight->get(c->actor);
            if (maxHeight > 0) c->actorHeight = Mathf::min(c->actorHeight, maxHeight);
        }

        align = c->align.value();
        if ((align & (int)Align::left) != 0)
            c->actorX = currentX;
        else if ((align & (int)Align::right) != 0)
            c->actorX = currentX + spannedCellWidth - c->actorWidth;
        else
            c->actorX = currentX + (spannedCellWidth - c->actorWidth) / 2;

        if ((align & (int) Align::top) != 0)
            c->actorY = currentY + c->computedPadTop;
        else if ((align & (int)Align::bottom) != 0)
            c->actorY = currentY + rowHeight[c->row] - c->actorHeight - c->computedPadBottom;
        else
            c->actorY = currentY + (rowHeight[c->row] - c->actorHeight + c->computedPadTop - c->computedPadBottom) / 2;

        if (c->endRow) {
            currentX = x;
            currentY += rowHeight[c->row];
        } else
            currentX += spannedCellWidth + c->computedPadRight;
    }


}

