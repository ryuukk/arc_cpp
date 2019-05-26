#include "BitmapFont.h"
#include "BitmapFontCache.h"

arc::BitmapFont::BitmapFont(const std::string& file, bool flip, bool integer) : _data(file, flip), _integer(integer)
{
    _flipped = _data.flipped;

    for(auto& path : _data.imagePaths)
    {
        printf("INFO: Loading font texture at: %s\n", path.c_str());
        auto* texture = Texture2D::loadFromFile(path);
        regions.emplace_back(new TextureRegion(texture));
    }
    _ownsTexture = true;
    _cache = new BitmapFontCache(this, _integer);
    load(_data);
}

void arc::BitmapFont::load(BitmapFontData& data)  {
    for (auto& page:data.glyphs) {
        if (page.empty()) continue;

        for (auto* glyph : page) {
            if (glyph != nullptr)
            {
                data.setGlyphRegion(glyph, regions[glyph->page]);
            }
        }
    }
    if(data.missingGlyph != nullptr) data.setGlyphRegion(data.missingGlyph, regions[data.missingGlyph->page]);
}

arc::Rect arc::BitmapFont::draw(arc::SpriteBatch* batch, const std::string& str, float x, float y) {
    _cache->clear();
    auto bounds = _cache->addText(str, x, y, 0, str.size());
    _cache->draw(batch);
    return bounds;
}

arc::Rect arc::BitmapFont::draw(arc::SpriteBatch* batch, const std::string& str, float x, float y, int start, int end) {
    _cache->clear();
    auto bounds = _cache->addText(str, x, y, start, end);
    _cache->draw(batch);
    return bounds;
}

arc::Rect arc::BitmapFont::drawMultiLine(arc::SpriteBatch* batch, const std::string& str, float x, float y) {
    return arc::Rect();
}

arc::Rect
arc::BitmapFont::drawMultiLine(arc::SpriteBatch* batch, const std::string& str, float x, float y, float alignWidth,
                               arc::Align align) {
    return arc::Rect();
}

arc::Rect
arc::BitmapFont::drawWrapped(arc::SpriteBatch* batch, const std::string& str, float x, float y, float wrapWidth) {
    return arc::Rect();
}

arc::Rect
arc::BitmapFont::drawWrapped(arc::SpriteBatch* batch, const std::string& str, float x, float y, float wrapWidth,
                             arc::Align align) {
    return arc::Rect();
}

arc::Rect arc::BitmapFont::getBounds(const std::string& str, int start, int end) {
    return arc::Rect();
}

int arc::BitmapFont::computeVisibleGlyphs(const std::string& str, int start, int end, float availableWidth) {
    return 0;
}

int arc::BitmapFont::indexOf(const std::string& str, char ch, int start) {
    return 0;
}

bool arc::BitmapFont::isWhitespace(char ch) {
    return false;
}
