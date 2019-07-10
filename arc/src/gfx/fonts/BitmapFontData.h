#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "../TextureRegion.h"
#include "Glyph.h"

namespace arc
{
    class BitmapFontData
    {
    public:
        // info
        std::string fontFace{};
        int fontSize{};
        bool bold{};
        bool italic{};
        // --

        std::vector<std::string> imagePaths;
        std::string fontFile{};
        bool flipped{};

        float padTop{};
        float padRight{};
        float padBottom{};
        float padLeft{};

        float lineHeight{};

        float capHeight = 1;
        float ascent{};
        float descent{};
        float down{};

        float blankLineScale = 1;
        float scaleX = 1, scaleY = 1;

        bool markupEnabled{};

        float cursorX{};

        std::vector<std::vector<Glyph*>> glyphs;
        Glyph* missingGlyph = nullptr;
        float spaceXadvance{};
        float xHeight = 1;

        std::vector<uint8_t> breakChars;
        std::vector<uint8_t> xChars = {'x', 'e', 'a', 'o', 'n', 's', 'r', 'c', 'u', 'm', 'v', 'w', 'z'};

        std::vector<uint8_t> capChars =
                {
                        'M', 'N', 'B', 'D', 'C', 'E', 'F', 'K', 'A', 'G', 'H', 'I', 'J', 'L', 'O', 'P', 'Q', 'R', 'S',
                        'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
                };

        BitmapFontData(const std::string& fontFile, bool flip);
        ~BitmapFontData()
        {
            for(auto& row : glyphs)
            {
                for(auto* glyph : row)
                {
                    if(glyph != nullptr)
                        delete glyph;
                }
            }
            if(missingGlyph != nullptr)
            delete missingGlyph;
        }

        void load(const std::string& fontFile, bool flip);

        void setGlyph(uint8_t ch, Glyph* glyph);

        Glyph* getGlyph(uint8_t ch);

        void setGlyphRegion(Glyph* glyph, TextureRegion* region);

        Glyph* getFirstGlyph();

        bool hasGlyph(uint8_t ch)
        {
            if (missingGlyph != nullptr) return true;
            return getGlyph(ch) != nullptr;
        }

        bool isBreakChar(uint8_t c)
        {
            if (breakChars.empty()) return false;
            for (int i = 0; i < breakChars.size(); i++)
            {
                if (breakChars[i] == c) return true;
            }
            return false;
        }

        bool isWhitespace(uint8_t c)
        {
            switch (c)
            {
                case '\n':
                case '\r':
                case '\t':
                case ' ':
                    return true;
                default:
                    return false;
            }
        }

    };
}