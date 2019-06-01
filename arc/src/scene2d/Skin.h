#pragma once

#include <string>
#include "../gfx/atlas/TextureAtlas.h"

namespace arc
{
    class Skin
    {
    public:
        Skin();
        ~Skin();
        void load(const std::string& file);
        void loadDefault();

    public:
        std::string id;
    private:
        TextureAtlas* _atlas = nullptr;
        std::unordered_map<int, std::unordered_map<std::string, void*>> _resources;
    };
}

