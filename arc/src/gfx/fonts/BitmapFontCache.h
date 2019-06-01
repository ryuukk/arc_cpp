#pragma once

#include <vector>
#include "TextMarkup.h"
#include "../../Color.h"
#include "../SpriteBatch.h"
#include "../Align.h"
#include "../../math/Rect.h"

namespace arc
{
    class BitmapFont;
    class Glyph;
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
        Rect setText(const std::string& str, float x, float y);
        Rect setText(const std::string& str, float x, float y, int start, int end);
        Rect addText(const std::string& str, float x, float y);
        Rect addText(const std::string& str, float x, float y, int start, int end);
        Rect setMultiLineText(const std::string& str, float x, float y);
        Rect setMultiLineText(const std::string& str, float x, float y, float alignmentWidth, Align alignment);
        Rect addMultiLineText(const std::string& str, float x, float y);
        Rect addMultiLineText(const std::string& str, float x, float y, float alignmentWidth, Align alignment);
        Rect setWrappedText(const std::string& str, float x, float y, float wrapWidth);
        Rect setWrappedText(const std::string& str, float x, float y, float wrapWidth, Align alignment);
        Rect addWrappedText(const std::string& str, float x, float y, float wrapWidth);
        Rect addWrappedText(const std::string& str, float x, float y, float wrapWidth, Align alignment);
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
        float _color = Color::WHITE.toFloatBits();
        bool _integer = true;
        int _glyphCount{};

        std::vector<std::vector<int>> _glyphIndices;

        bool _textChanged{};
        float _oldTint{};

        TextMarkup _markup{};

        int _charsCount{};
    };
}