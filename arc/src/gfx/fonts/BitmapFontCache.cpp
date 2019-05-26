#include "BitmapFontCache.h"

arc::BitmapFontCache::BitmapFontCache(arc::BitmapFont* font, bool integer) :
        _font(font), _integer(integer) {

    int regionsLength = font->regions.size();
    if(regionsLength == 0) printf("ERROR: font must contain regions\n");

    _vertexData.resize(regionsLength);
    _idx.resize(regionsLength);

    int vertexDataLength = _vertexData.size();
    if(vertexDataLength > 1)
    {
        _glyphIndices.resize(vertexDataLength);
        _tmpGlyphCount.resize(vertexDataLength);
    }
}

void arc::BitmapFontCache::setPosition(float x, float y) {

    translate(x - _x, y - _y);
}

void arc::BitmapFontCache::translate(float xAmount, float yAmount) {
    if (xAmount == 0 && yAmount == 0) return;
    if (_integer)
    {
        xAmount = Mathf::round(xAmount);
        yAmount = Mathf::round(yAmount);
    }

    _x += xAmount;
    _y += yAmount;

    for (int j = 0, length = _vertexData.size(); j < length; j++)
    {
        auto& vertices = _vertexData[j];
        for (int i = 0, n = _idx[j]; i < n; i += 5)
        {
            vertices[i] += xAmount;
            vertices[i + 1] += yAmount;
        }
    }
}

void arc::BitmapFontCache::tint(arc::Color tint) {
    float floatTint = tint.toFloatBits();
    if (_textChanged || _oldTint != floatTint)
    {
        _textChanged = false;
        _oldTint = floatTint;

        _markup.tint(this, tint);
    }
}

void arc::BitmapFontCache::setColors(float color, int start, int end) {
    if (_vertexData.size() == 1)
    {
        // only one page...
        auto& vertices = _vertexData[0];
        for (int i = start * 20 + 2, n = end * 20; i < n; i += 5)
            vertices[i] = color;
    }
    else
    {
        int pageCount = _vertexData.size();

        // for each page...
        for (int i = 0; i < pageCount; i++)
        {
            auto& vertices = _vertexData[i];

            // we need to loop through the indices and determine whether the glyph is inside begin/end
            for (int j = 0, n = _glyphIndices[i].size(); j < n; j++)
            {
                int gInd = _glyphIndices[i][j];

                // break early if the glyph is outside our bounds
                if (gInd >= end) break;

                // if the glyph is inside start and end, then change it's colour
                if (gInd >= start)
                {
                    // && gInd < end
                    // modify color index
                    for (int off = 0; off < 20; off += 5)
                        vertices[off + (j * 20 + 2)] = color;
                }
            }
        }
    }
}

void arc::BitmapFontCache::draw(arc::SpriteBatch* spriteBatch) {

    auto regions = _font->regions;
    for (int j = 0, n = _vertexData.size(); j < n; j++)
    {
        if (_idx[j] > 0)
        {
            // ignore if this texture has no glyphs
            auto& vertices = _vertexData[j];
            spriteBatch->draw(regions[j]->texture, vertices, 0, _idx[j]);
        }
    }
}

void arc::BitmapFontCache::clear() {
    _x = 0;
    _y = 0;
    _glyphCount = 0;
    _charsCount = 0;
    _markup.clear();
    for (int i = 0, n = _idx.size(); i < n; i++)
    {
        if (!_glyphIndices.empty()) _glyphIndices[i].clear();
        _idx[i] = 0;
    }
}

arc::TextBounds arc::BitmapFontCache::setText(const std::string& str, float x, float y) {
    return arc::TextBounds();
}

arc::TextBounds arc::BitmapFontCache::setText(const std::string& str, float x, float y, int start, int end) {
    return arc::TextBounds();
}

arc::TextBounds arc::BitmapFontCache::addText(const std::string& str, float x, float y) {
    return arc::TextBounds();
}

arc::TextBounds arc::BitmapFontCache::addText(const std::string& str, float x, float y, int start, int end) {
    return arc::TextBounds();
}

arc::TextBounds arc::BitmapFontCache::setMultiLineText(const std::string& str, float x, float y) {
    return arc::TextBounds();
}

arc::TextBounds arc::BitmapFontCache::setMultiLineText(const std::string& str, float x, float y, float alignmentWidth,
                                                       arc::Align alignment) {
    return arc::TextBounds();
}

arc::TextBounds arc::BitmapFontCache::addMultiLineText(const std::string& str, float x, float y) {
    return arc::TextBounds();
}

arc::TextBounds arc::BitmapFontCache::addMultiLineText(const std::string& str, float x, float y, float alignmentWidth,
                                                       arc::Align alignment) {
    return arc::TextBounds();
}

arc::TextBounds arc::BitmapFontCache::setWrappedText(const std::string& str, float x, float y, float wrapWidth) {
    return arc::TextBounds();
}

arc::TextBounds
arc::BitmapFontCache::setWrappedText(const std::string& str, float x, float y, float wrapWidth, arc::Align alignment) {
    return arc::TextBounds();
}

arc::TextBounds arc::BitmapFontCache::addWrappedText(const std::string& str, float x, float y, float wrapWidth) {
    return arc::TextBounds();
}

arc::TextBounds
arc::BitmapFontCache::addWrappedText(const std::string& str, float x, float y, float wrapWidth, arc::Align alignment) {
    return arc::TextBounds();
}

int arc::BitmapFontCache::getCharsCount() {
    return 0;
}

int arc::BitmapFontCache::countGlyphs(const std::string& seq, int start, int end) {
    int count = end - start;
    while (start < end)
    {
        char ch = seq[start++];
        if (ch == '[')
        {
            count--;
            if (!(start < end && seq[start] == '['))
            {
                // non escaped '['
                while (start < end && seq[start] != ']')
                {
                    start++;
                    count--;
                }

                count--;
            }

            start++;
        }
    }
    return count;
}

void arc::BitmapFontCache::requireSequence(const std::string& seg, int start, int end) {
    if (_vertexData.size() == 1)
    {
        // don't scan sequence if we just have one page and markup is disabled
        int newGlyphCount = _font->enableColorMarkup ? countGlyphs(seg, start, end) : end - start;
        require(0, newGlyphCount);
    }
    else
    {
        for (int i = 0, n = _tmpGlyphCount.size(); i < n; i++)
            _tmpGlyphCount[i] = 0;

        // determine # of glyphs in each page
        while (start < end)
        {
            char ch = seg[start++];
            if (ch == '[' && _font->enableColorMarkup)
            {
                if (!(start < end && seg[start] == '['))
                {
                    // non escaped '['
                    while (start < end && seg[start] != ']')
                        start++;
                    start++;
                    continue;
                }

                start++;
            }

            Glyph* g = _font->getData().getGlyph(ch);
            if (g == nullptr) continue;
            _tmpGlyphCount[g->page]++;
        }

        // require that many for each page
        for (int i = 0, n = _tmpGlyphCount.size(); i < n; i++)
            require(i, _tmpGlyphCount[i]);
    }
}

void arc::BitmapFontCache::require(int page, int glyphCount) {

}

float arc::BitmapFontCache::addToCache(const std::string& str, float x, float y, int start, int end) {
    return 0;
}

void arc::BitmapFontCache::addGlyph(arc::Glyph* glyph, float x, float y, float width, float height) {

}
