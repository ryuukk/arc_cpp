#include <fstream>
#include <cstring>
#include <regex>
#include "BitmapFontData.h"
#include "BitmapFont.h"
#include "../../utils/StringUtils.h"
#include "../../utils/IO.h"

arc::BitmapFontData::BitmapFontData(const std::string& fontFile, bool flip)
{
    glyphs.resize(arc::font::PAGES);
    this->fontFile = fontFile;
    this->flipped = flip;
    load(fontFile, flip);
}

arc::Glyph* arc::BitmapFontData::getFirstGlyph()
{
    for (auto& page : glyphs)
    {
        if (page.empty()) continue;
        for(auto& glyph : page)
        {
            if (glyph == nullptr || glyph->height == 0 || glyph->width == 0) continue;
            return glyph;
        }
    }

    throw std::invalid_argument("No glyphs found."); // todo: figure out exceptions
}

void arc::BitmapFontData::setGlyphRegion(arc::Glyph* glyph, arc::TextureRegion* region)
{
    auto* texture = region->texture;
    float invTexWidth = 1.0f / texture->getWidth();
    float invTexHeight = 1.0f / texture->getHeight();

    float offsetX = 0, offsetY = 0;
    float u = region->u;
    float v = region->v;
    float regionWidth = region->regionWidth;
    float regionHeight = region->regionHeight;

    float x = glyph->srcX;
    float x2 = glyph->srcX + glyph->width;
    float y = glyph->srcY;
    float y2 = glyph->srcY + glyph->height;

    // Shift glyph for left and top edge stripped whitespace. Clip glyph for right and bottom edge stripped whitespace.
    // Note if the font region has padding, whitespace stripping must not be used.
    if (offsetX > 0)
    {
        x -= offsetX;
        if (x < 0)
        {
            glyph->width += (int) x;
            glyph->xoffset -= (int) x;
            x = 0;
        }

        x2 -= offsetX;
        if (x2 > regionWidth)
        {
            glyph->width -= (int) x2 - (int) regionWidth;
            x2 = regionWidth;
        }
    }

    if (offsetY > 0)
    {
        y -= offsetY;
        if (y < 0)
        {
            glyph->height += (int) y;
            if (glyph->height < 0) glyph->height = 0;
            y = 0;
        }

        y2 -= offsetY;
        if (y2 > regionHeight)
        {
            float amount = y2 - regionHeight;
            glyph->height -= (int) amount;
            glyph->yoffset += (int) amount;
            y2 = regionHeight;
        }
    }

    glyph->u = u + x * invTexWidth;
    glyph->u2 = u + x2 * invTexWidth;
    if (flipped)
    {
        glyph->v = v + y * invTexHeight;
        glyph->v2 = v + y2 * invTexHeight;
    }
    else
    {
        glyph->v2 = v + y * invTexHeight;
        glyph->v = v + y2 * invTexHeight;
    }
}

arc::Glyph* arc::BitmapFontData::getGlyph(uint8_t ch)
{
    auto& page = glyphs[ch / arc::font::PAGE_SIZE];
    if (!page.empty())

        return page[ch & arc::font::PAGE_SIZE - 1];
    return nullptr;
}

void arc::BitmapFontData::setGlyph(uint8_t ch, arc::Glyph* glyph)
{
    auto& page = glyphs[ch / arc::font::PAGE_SIZE];
    if (page.empty())
    {
        page.resize(arc::font::PAGE_SIZE);
    }
    page[ch & arc::font::PAGE_SIZE - 1] = glyph;
}

void arc::BitmapFontData::load(const std::string& fontFile, bool flip)
{
    std::ifstream stream (fontFile, std::ifstream::binary);
    if(stream)
    {
        std::string buffer;
        std::getline(stream, buffer);

        auto infoTokens = arc::string::split(buffer, " ");

        // info
        fontFace = infoTokens[1].substr(6);
        fontFace = fontFace.substr(0, fontFace.size() - 1);

        fontSize = std::stoi(infoTokens[2].substr(5));;
        bold = std::stoi(infoTokens[3].substr(5)) == 1;
        italic = std::stoi(infoTokens[4].substr(7)) == 1;

        // printf("Font: %s\n%s\nFontSize:%d\nBold:%d\nItalic:%d\n", fontFile.c_str(), buffer.c_str(), fontSize, bold, italic);
        // --

        buffer = buffer.substr(buffer.find("padding=")+8);

        auto padding = arc::string::split(buffer.substr(0, buffer.find(' ')), ",");
        if(padding.size() != 4) printf("ERROR: Padding should have 4 values\n");

        padTop = std::stoi(padding[0]);
        padRight = std::stoi(padding[1]);
        padBottom = std::stoi(padding[2]);
        padLeft = std::stoi(padding[3]);
        float padY = padTop + padBottom;

        std::getline(stream, buffer);

        auto common = arc::string::split(buffer, " ");
        if(common.size() < 3) printf("ERROR: invalid common header\n");

        if(common[1].find("lineHeight") == -1) printf("ERROR: missing lineHeight\n");
        lineHeight = std::stoi(common[1].substr(11));

        if(common[2].find("base") == -1) printf("ERROR: missing base\n");
        float baseLine = std::stoi(common[2].substr(5));

        int pageCount = 1;
        if(common.size() >= 6 && common[5].find("pages") != -1)
            pageCount = Mathf::max(1, std::stoi(common[5].substr(6)));

        // printf("Line Height: %f Base: %f PageCount: %d \n", lineHeight, baseLine, pageCount);

        for (int p = 0; p < pageCount; p++)
        {
            std::getline(stream, buffer);

            {
                std::regex re(".*id=(\\d+)");
                std::smatch match;
                std::string id;
                if (std::regex_search(buffer, match, re) && match.size() > 1) {
                    id = match.str(1);
                } else {
                    id = "";
                }
                int pageId = std::stoi(id);
            }
            {
                std::regex re(".*file=\"?([^\"]+)\"?");
                std::smatch match;
                std::string fileName;
                if (std::regex_search(buffer, match, re) && match.size() > 1) {
                    fileName = match.str(1);
                } else {
                    fileName = "";
                }
                std::string dirName = arc::directory::dirName(fontFile);
                std::string texPath = arc::directory::pathAppend(dirName, fileName);
                printf("INFO: Has Page %d -> %s Path: %s\n", p, fileName.c_str(), texPath.c_str());
                imagePaths.emplace_back(texPath);
            }
        }

        descent = 0;

        while(true)
        {
            if(!std::getline(stream, buffer))
                break;

            if(buffer.find("kernings") != -1) break;
            if(buffer.find("metrics") != -1) break;
            if(buffer.find("char ") == -1) continue;

            auto* glyph = new Glyph();
            auto tokens = arc::string::split(buffer, " ");

            auto id = std::stoi(tokens[1].substr(3));
            auto x = std::stoi(tokens[2].substr(2));
            auto y = std::stoi(tokens[3].substr(2));
            auto width = std::stoi(tokens[4].substr(6));
            auto height = std::stoi(tokens[5].substr(7));
            auto xoffset = std::stoi(tokens[6].substr(8));
            auto yoffset = std::stoi(tokens[7].substr(8));
            auto xadvance = std::stoi(tokens[8].substr(9));
            auto page = std::stoi(tokens[9].substr(5));
            auto chnl = std::stoi(tokens[10].substr(5));

            if (id <= 0)
                missingGlyph = glyph;
            else if (id <= 3000)
                setGlyph(id, glyph);
            else
                continue;

            glyph->id = id;
            glyph->srcX = x;
            glyph->srcY = y;
            glyph->width = width;
            glyph->height = height;
            glyph->xoffset = xoffset;
            if(flip)
                glyph->yoffset = yoffset;
            else
                glyph->yoffset = -(glyph->height + yoffset);
            glyph->xadvance= xadvance;
            glyph->page = page;
            if (glyph->width > 0 && glyph->height > 0) descent = Mathf::min(baseLine + glyph->yoffset, descent);
        }

        descent += padBottom;

        while(true)
        {
            if(!std::getline(stream, buffer))
                break;
            if(buffer.find("kerning") == -1) break;

            auto tokens = arc::string::split(buffer, " ");

            int first = std::stoi(tokens[1].substr(6));
            int second = std::stoi(tokens[1].substr(7));

            if (first < 0 || first > 3000 || second < 0 || second > 3000) continue;

            auto glyph = getGlyph(first);
            int amount = std::stoi(tokens[3].substr(7));
            if(glyph)
            {
                glyph->setKerning(second, amount);
            }
        }

        auto hasMetricsOverride = false;
        auto overrideAscent = 0.0f;
        auto overrideDescent = 0.0f;
        auto overrideDown = 0.0f;
        auto overrideCapHeight = 0.0f;
        auto overrideLineHeight = 0.0f;
        auto overrideSpaceXAdvance = 0.0f;
        auto overrideXHeight = 0.0f;

        if(buffer.find("metrics") != -1)
        {
            hasMetricsOverride = true;
            printf("WARN: forgot to parse metrics data\n");
        }

        auto spaceGlyph = getGlyph(' ');
        if(!spaceGlyph)
        {
            spaceGlyph = new Glyph();
            spaceGlyph->id = ' ';
            auto xadvanceGlyph = getGlyph('l');
            if(!xadvanceGlyph) xadvanceGlyph = getFirstGlyph();
            spaceGlyph->xadvance = xadvanceGlyph->xadvance;
            setGlyph(' ', spaceGlyph);
        }

        if(spaceGlyph->width == 0)
        {
            spaceGlyph->width = (int) (padLeft + spaceGlyph->xadvance + padRight);
            spaceGlyph->xoffset = (int) -padLeft;
        }
        spaceXadvance = spaceGlyph->xadvance;

        Glyph* xGlyph = nullptr;
        for(auto xChar : xChars)
        {
            xGlyph = getGlyph(xChar);
            if(xGlyph) break;
        }

        Glyph* capGlyph = nullptr;
        for (auto capChar : capChars)
        {
            capGlyph = getGlyph(capChar);
            if (capGlyph) break;
        }

        if (capGlyph == nullptr)
        {
            for (auto& page : this->glyphs)
            {
                if (page.empty()) continue;
                for (auto* glyph : page)
                {
                    if (glyph == nullptr || glyph->height == 0 || glyph->width == 0) continue;
                    capHeight = Mathf::max(capHeight, (float) glyph->height);
                }
            }
        }
        else
            capHeight = capGlyph->height;

        capHeight -= padY;

        ascent = baseLine - capHeight;
        down = -lineHeight;
        if (flip)
        {
            ascent = -ascent;
            down = -down;
        }

        if (hasMetricsOverride)
        {
            this->ascent = overrideAscent;
            this->descent = overrideDescent;
            this->down = overrideDown;
            this->capHeight = overrideCapHeight;
            this->lineHeight = overrideLineHeight;
            this->spaceXadvance = overrideSpaceXAdvance;
            this->xHeight = overrideXHeight;
        }

    }

}

