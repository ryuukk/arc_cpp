#pragma once

#include <string>
#include <vector>

#include "../Texture2D.h"
#include "../PixFormat.h"

namespace arc
{
    class Page
    {
    public:
        std::string file;
        Texture2D* texture = nullptr;
        float width, height;
        bool useMipMaps;
        PixFormat format;
        TextureFilter minFilter;
        TextureFilter magFilter;
        TextureWrap uWrap;
        TextureWrap vWrap;

    };

    class Region
    {
    public:
        int pageIndex{};
        int index{};
        std::string name;
        float offsetX{};
        float offsetY{};
        int originalWidth{};
        int originalHeight{};
        bool rotate{};
        int degrees{};
        int left{};
        int top{};
        int width{};
        int height{};
        bool flip{};

        // optionals
        bool hasSplits{};
        bool hasPads{};
        std::vector<int> splits;
        std::vector<int> pads;
    };

    class TextureAtlasData
    {
    public:
        std::vector<Page> pages;
        std::vector<Region> regions;

        void load(const std::string& packFile, const std::string& imagesDir, bool flip);

    private:
        int readTuple(std::ifstream& stream);
        int _tuple[4];
    };
}

