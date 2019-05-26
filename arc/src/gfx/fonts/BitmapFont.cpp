#include "BitmapFont.h"

arc::BitmapFont::BitmapFont(arc::BitmapFontData data, bool integer) : _data(std::move(data)), _integer(integer)
{
    _flipped = data.flipped;

    for(auto& path : data.imagePaths)
    {
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
            if (glyph != nullptr) {
                data.setGlyphRegion(glyph, regions[glyph->page]);
            }
        }
    }
    if(data.missingGlyph != nullptr) data.setGlyphRegion(data.missingGlyph, regions[data.missingGlyph->page]);
}
