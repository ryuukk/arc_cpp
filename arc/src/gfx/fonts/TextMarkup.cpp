#include <sstream>
#include "TextMarkup.h"
#include "BitmapFontCache.h"
//#include "../../utils/fmt/fmt/format.h"


void arc::TextMarkup::beginChunk(arc::Color color, int start) {
    _colorChunks.emplace_back(ColorChunk{start, color});
    _currentColorStack.push(_lastColor);
    _lastColor = color;
}

void arc::TextMarkup::endChunk(int start) {
    if (!_currentColorStack.empty())
    {
        _lastColor = _currentColorStack.top(); // todo: pop
        _currentColorStack.pop();

        _colorChunks.emplace_back(ColorChunk{start, _lastColor});
    }
}

void arc::TextMarkup::tint(arc::BitmapFontCache* cache, arc::Color tint) {
    int current = 0;
    float floatColor = (_defaultColor * tint).toFloatBits();
    for (auto& chunk : _colorChunks) {
        int next = chunk.start;
        if (current < next) {
            cache->setColors(floatColor, current, next);
            current = next;
        }
        floatColor = (chunk.color *tint).toFloatBits();
    }
    int charsCount = cache->getCharsCount();
    if (current < charsCount) {
        cache->setColors(floatColor, current, charsCount);
    }
}

void arc::TextMarkup::clear() {

    int size = _colorChunks.size();
    _colorChunks.clear();
    _currentColorStack = std::stack<Color>();
    setDefaultChunk(_defaultColor, 0);
}

void arc::TextMarkup::setDefaultChunk(arc::Color color, int start) {
    _colorChunks.emplace_back(start, color);
    setDefaultColor(color);
}

arc::Color arc::TextMarkup::getLastColor() {
    return _lastColor;
}

void arc::TextMarkup::setDefaultColor(arc::Color defaultColor) {
    if (_currentColorStack.empty())
    {
       _defaultColor = defaultColor;
       _lastColor = defaultColor;
    }
}

int arc::TextMarkup::parseColorTag(arc::TextMarkup* markup, const std::string& str, int nomarkupStart, int start, int end) {

    if (start < end) {
        Color hexColor;
        if (str[start] == '#') {
            // Parse hex color RRGGBBAA where AA is optional and defaults to 0xFF if less than 6 chars are used
            uint32_t colorInt = 0;
            for (int i = start + 1; i < end; i++) {
                char ch = str[i];
                if (ch == ']')
                {
                    if (i < start + 2 || i > start + 9)
                    {
                        printf("Hex color cannot have %d digits.\n", (i - start - 1));
                        //throw std::runtime_error(fmt::format("Hex color cannot have {0} digits.", (i - start - 1)));
                        throw std::runtime_error("Hex color wrong digits");
                    }
                    if (i <= start + 7) { // RRGGBB
                        hexColor = Color::fromRGB888(colorInt);
                    } else { // RRGGBBAA
                        hexColor = Color::fromRGB888(colorInt);
                    }
                    markup->beginChunk(hexColor, nomarkupStart);
                    return i - start;
                }
                if (ch >= '0' && ch <= '9')
                    colorInt =  (colorInt * 16 + (ch - '0'));
                else if (ch >= 'a' && ch <= 'f')
                    colorInt =  (colorInt * 16 + (ch - ('a' - 10)));
                else if (ch >= 'A' && ch <= 'F')
                    colorInt =  (colorInt * 16 + (ch - ('A' - 10)));
                else
                {
                    printf("Unexpected character in hex color: %s\n", ch);
                    throw std::runtime_error("wut");
                }
            }
        } else {
            // Parse named color
            //tempColorBuffer.Clear();
            std::stringstream ss;
            for (int i = start; i < end; i++) {
                char ch = str[i];
                if (ch == ']')
                {
                    if (ss.width() == 0) { // end tag []
                        markup->endChunk(nomarkupStart);
                    } else {
                        throw std::runtime_error("named color not impl");
                        //var colorString = tempColorBuffer.toString();
                        //Color newColor = Colors.get(colorString);
                        //if (newColor == null) throw new Exception("Unknown color: " + colorString);
                        //markup.beginChunk(newColor, nomarkupStart);
                    }
                    return i - start;
                } else {
                    ss << ch;
                }
            }
        }
    }
    throw std::runtime_error("Unclosed color tag"); // todo: figure out exceptions
}
