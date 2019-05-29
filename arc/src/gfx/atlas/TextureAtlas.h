#pragma once

#include <string>
#include <vector>
#include "../TextureRegion.h"
#include "../NinePatch.h"

namespace arc
{
    class AtlasRegion : public TextureRegion
    {
    public:
        AtlasRegion(Texture2D* texture, int x, int y, int width, int height);

        int index{};
        std::string name;
        float offsetX{};
        float offsetY{};
        int packedWidth{};
        int packedHeight{};
        int originalWidth{};
        int originalHeight{};
        bool rotate{};
        int degrees{};
        std::vector<int> splits;
        std::vector<int> pads;
    };

    class TextureAtlas
    {
    public:
        TextureAtlas(const std::string& file);
        ~TextureAtlas();

        void load(const std::string& file, const std::string& imagesDir, bool flip);

    public:
        std::vector<Texture2D*> textures;
        std::vector<AtlasRegion> regions;
        std::vector<NinePatch*> ninepatches;
    };
}


