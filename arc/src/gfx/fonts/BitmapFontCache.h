#pragma once

#include <vector>
#include "BitmapFont.h"
#include "TextMarkup.h"
#include "../../Color.h"
#include "../SpriteBatch.h"
#include "../Align.h"

namespace arc
{
    struct TextBounds
    {
        float x;
        float y;
        float width;
        float height;
    };

    class BitmapFont;
    class BitmapFontCache
    {
    public:
        BitmapFontCache(BitmapFont* font, bool integer);

        void setPosition(float x, float y);
        void translate(float xAmount, float yAmount);
        void tint(Color tint);
        void setColors(float color, int start, int end);
        void draw(SpriteBatch* spriteBatch);
        void clear();
        TextBounds setText(const std::string& str, float x, float y);
        TextBounds setText(const std::string& str, float x, float y, int start, int end);
        TextBounds addText(const std::string& str, float x, float y);
        TextBounds addText(const std::string& str, float x, float y, int start, int end);
        TextBounds setMultiLineText(const std::string& str, float x, float y);
        TextBounds setMultiLineText(const std::string& str, float x, float y, float alignmentWidth, Align alignment);
        TextBounds addMultiLineText(const std::string& str, float x, float y);
        TextBounds addMultiLineText(const std::string& str, float x, float y, float alignmentWidth, Align alignment);
        TextBounds setWrappedText(const std::string& str, float x, float y, float wrapWidth);
        TextBounds setWrappedText(const std::string& str, float x, float y, float wrapWidth, Align alignment);
        TextBounds addWrappedText(const std::string& str, float x, float y, float wrapWidth);
        TextBounds addWrappedText(const std::string& str, float x, float y, float wrapWidth, Align alignment);
        int getCharsCount();
    private:
        int countGlyphs(const std::string& seq, int start, int end);
        void requireSequence(const std::string& seg, int start, int end);
        void require(int page, int glyphCount);
        float addToCache(const std::string& str, float x, float y, int start, int end);
        void addGlyph(Glyph* glyph, float x, float y, float width, float height);

    private:
        BitmapFont* _font = nullptr;
        std::vector<std::vector<float>> _vertexData;
        std::vector<int> _idx;
        std::vector<int> _tmpGlyphCount;
        float _x{};
        float _y{};
        float _color{};
        bool _integer = true;
        int _glyphCount{};

        std::vector<std::vector<int>> _glyphIndices;

        bool _textChanged{};
        float _oldTint{};

        TextMarkup _markup{};

        int _charsCount{};
    };
}