#pragma once

#include <cstdint>
#include <vector>

namespace arc
{
    class Glyph
    {
    public:
        int id{};
        int srcX{};
        int srcY{};
        int width{};
        int height{};
        float u{};
        float v{};
        float u2{};
        float v2{};
        int xoffset{};
        int yoffset{};
        int xadvance{};
        std::vector<std::vector<uint8_t>> kerning;
        bool fixedWidth{};
        int page = 0;

        int getKerning(uint8_t ch);

        void setKerning(uint8_t ch, int value);
    };
}