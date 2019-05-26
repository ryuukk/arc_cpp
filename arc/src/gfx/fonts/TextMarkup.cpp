#include "TextMarkup.h"
#include "BitmapFontCache.h"


void arc::TextMarkup::beginChunk(arc::Color color, int start) {
    ColorChunk newChunk{};
    newChunk.color = color;
    newChunk.start = start;
    _colorChunks.emplace_back(newChunk);
    _currentColorStack.push(_lastColor);
    _lastColor = newChunk.color;
}

void arc::TextMarkup::endChunk(int start) {
    if (!_currentColorStack.empty())
    {
        _lastColor = _currentColorStack.top(); // todo: pop
        _currentColorStack.pop();

        ColorChunk newChunk{};
        newChunk.color = _lastColor;
        newChunk.start = start;
        _colorChunks.emplace_back(newChunk);
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
    _currentColorStack = std::stack<Color>(); // todo: check memory ?
    setDefaultChunk(_defaultColor, 0);
}

void arc::TextMarkup::setDefaultChunk(arc::Color color, int start) {
    ColorChunk newChunk{};
    newChunk.color = color;
    newChunk.start = start;
    _colorChunks.emplace_back(newChunk);
    setDefaultColor(newChunk.color);
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

    if(start < end)
    {
        Color hexColor;
        if(str[start] == '#')
        {
            uint32_t colorInt = 0;
            for (int i = start + 1; i < end; i++) {
                char ch = str[i];
                if (ch == ']') {
                    if (i < start + 2 || i > start + 9)
                    {
                        //throw std::invalid_argument("Hex color cannot have " + (i - start - 1) + " digits.");
                        printf("Hex color cannot have %d digits\n", (i - start - 1)); // todo: figure out exceptions
                    }
                    if (i <= start + 7) { // RRGGBB
                        hexColor = Color(colorInt);
                        hexColor.a = 1.0f;
                    } else { // RRGGBBAA
                        hexColor = Color(colorInt);
                    }
                    markup->beginChunk(hexColor, nomarkupStart);
                    return i - start;
                }
                if (ch >= '0' && ch <= '9')
                    colorInt = (uint32_t) (colorInt * 16 + (ch - '0'));
                else if (ch >= 'a' && ch <= 'f')
                    colorInt = (uint32_t) (colorInt * 16 + (ch - ('a' - 10)));
                else if (ch >= 'A' && ch <= 'F')
                    colorInt = (uint32_t) (colorInt * 16 + (ch - ('A' - 10)));
                else
                {
                    //throw new Exception("Unexpected character in hex color: " + ch);
                    printf("Unexpected character in hex color: %s\n", ch); // todo: figure out exceptions
                }
            }
        } else
        {
            printf("WAR: missing named color\n");
        }
    }

    throw std::invalid_argument("Unclosed color tag"); // todo: figure out exceptions
}
