#include "TextureAtlas.h"
#include <unordered_map>
#include "TextureAtlasData.h"
#include "../../utils/DirUtils.h"

arc::TextureAtlas::TextureAtlas(const std::string& file)
{
    load(file, arc::directory::dirName(file), false);
}

arc::TextureAtlas::~TextureAtlas() {
    for(auto* texture : textures)
        delete texture;
}

void arc::TextureAtlas::load(const std::string& file, const std::string& imagesDir, bool flip)
{
    TextureAtlasData data;
    data.load(file, imagesDir, flip);

    for (int i = 0; i < data.pages.size(); ++i) {
        auto page = data.pages[i];

        Texture2D* texture = nullptr;
        if(page.texture == nullptr)
        {
            // todo: apply filters
            texture = Texture2D::loadFromFile(page.file);
        }
        else
        {
            texture = page.texture;
            // todo: apply filters
        }

        textures.emplace_back(texture);
    }

    for (int j = 0; j < data.regions.size(); ++j) {
        auto region = data.regions[j];
        int width = region.width;
        int height = region.height;

        //auto page = data.pages[region.pageIndex];
        auto texture = textures[region.pageIndex];
        //auto texRegion = new TextureRegion(page.texture);
        //texRegion->setRegion(region.left, region.top, region.rotate ? height:width, region.rotate ? width:height);
        auto atlasRegion = AtlasRegion(texture, region.left, region.top, region.rotate ? height:width, region.rotate ? width:height);
        if(region.flip) atlasRegion.flip(false, true);
        regions.emplace_back(atlasRegion);
    }
}

arc::AtlasRegion::AtlasRegion(arc::Texture2D* texture, int x, int y, int width, int height) {

    this->texture = texture;
    setRegion(x, y, width, height);
}
