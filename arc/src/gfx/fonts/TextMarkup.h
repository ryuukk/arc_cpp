#pragma once

#include <vector>
#include <stack>
#include <string>
#include "../../Color.h"

namespace arc
{
    class BitmapFontCache;
    class TextMarkup
    {
    public:
        static int parseColorTag(TextMarkup* markup, const std::string& str, int nomarkupStart, int start, int end);

    public:
        void beginChunk (Color color, int start);
        void endChunk(int start);
        void tint(BitmapFontCache* cache, Color tint);
        void clear();
        void setDefaultChunk (Color color, int start);
        Color getLastColor();
    private:
        void setDefaultColor(Color defaultColor);

    private:
        struct ColorChunk
        {
            int start;
            Color color;

            ColorChunk(){}
            ColorChunk(int start, Color color): start(start), color(color)
            {}
        };

        std::vector<ColorChunk> _colorChunks{};
        std::stack<Color> _currentColorStack{};
        Color _lastColor = Color::WHITE;
        Color _defaultColor = Color::WHITE;
    };

}

