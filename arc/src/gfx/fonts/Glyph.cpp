#include "Glyph.h"
#include "BitmapFont.h"

int arc::Glyph::getKerning(uint8_t ch) {
    if (!kerning.empty()) {
        auto& page = kerning[ch >> arc::font::LOG2_PAGE_SIZE];
        if (!page.empty())
            return page[ch & arc::font::PAGE_SIZE - 1];
    }
    return 0;
}

void arc::Glyph::setKerning(uint8_t ch, int value) {
    // todo: verify this code
    if (kerning.empty()) kerning.reserve(arc::font::PAGES);
    auto& page = kerning[ch >> arc::font::LOG2_PAGE_SIZE];

    if (page.empty()) {
        page.resize(arc::font::PAGE_SIZE);
    }
    page[ch & arc::font::PAGE_SIZE - 1] = value;


}

