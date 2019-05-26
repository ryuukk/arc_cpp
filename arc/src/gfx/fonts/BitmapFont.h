#pragma once

#include <vector>
#include "../../math/Rect.h"
#include "../SpriteBatch.h"
#include "../TextureRegion.h"
#include "BitmapFontData.h"

namespace arc
{
    namespace font
    {
        static const int LOG2_PAGE_SIZE = 9;
        static const int PAGE_SIZE = 1 << LOG2_PAGE_SIZE;
        static const int PAGES = 0x10000 / PAGE_SIZE;
    }


    class BitmapFontCache;
    class BitmapFont
    {
    public:
        std::vector<TextureRegion*> regions;
        bool enableColorMarkup = false;

        BitmapFont(const std::string& file, bool flip, bool integer);

        ~BitmapFont()
        {
            delete _cache;
            for(auto* region : regions)
                delete region;
        }

        Rect draw(SpriteBatch* batch, const std::string& str, float x, float y);


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