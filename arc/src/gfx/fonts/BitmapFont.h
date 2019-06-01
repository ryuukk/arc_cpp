#pragma once

#include <vector>
#include "../../math/Rect.h"
#include "../SpriteBatch.h"
#include "../TextureRegion.h"
#include "BitmapFontData.h"
#include "BitmapFontCache.h"
#include "../Align.h"

namespace arc
{
    namespace font
    {
        static const int LOG2_PAGE_SIZE = 9;
        static const int PAGE_SIZE = 1 << LOG2_PAGE_SIZE;
        static const int PAGES = 0x10000 / PAGE_SIZE;
    }

    class BitmapFont
    {
    public:
        std::vector<TextureRegion*> regions;
        bool enableColorMarkup = true;

        BitmapFont(const std::string& file, bool flip, bool integer);

        ~BitmapFont()
        {
            delete _cache;
            for(auto* region : regions)
            {
                // todo: i don't like this
                if(_ownsTexture)
                    delete region->texture;
                delete region;
            }
        }

        Rect draw(SpriteBatch* batch, const std::string& str, float x, float y);
        Rect draw(SpriteBatch* batch, const std::string& str, float x, float y, int start, int end);

        Rect drawMultiLine(SpriteBatch* batch, const std::string& str, float x, float y);
        Rect drawMultiLine(SpriteBatch* batch, const std::string& str, float x, float y, float alignWidth, Align align);

        Rect drawWrapped(SpriteBatch* batch, const std::string& str, float x, float y, float wrapWidth);
        Rect drawWrapped(SpriteBatch* batch, const std::string& str, float x, float y, float wrapWidth, Align align);

        Rect getBounds(const std::string& str, int start, int end);
        int computeVisibleGlyphs(const std::string& str, int start, int end, float availableWidth);
        int indexOf(const std::string& str, char ch, int start);
        bool isWhitespace(char ch);

        BitmapFontData& getData()
        {
            return _data;
        }

    private:
        BitmapFontData _data;
        BitmapFontCache* _cache = nullptr;
        bool  _flipped{};
        bool _integer{};
        bool _ownsTexture = true;

        void load(BitmapFontData& data);
    };
}