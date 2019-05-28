#pragma once

#include <string>
#include <vector>

namespace arc
{
    class TextureAtlas
    {
    public:
        TextureAtlas(const std::string& file);

        void load(const std::string& file, const std::string& imagesDir, bool flip);
    };
}


