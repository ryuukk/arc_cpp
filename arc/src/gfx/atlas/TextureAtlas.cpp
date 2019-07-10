#include "TextureAtlas.h"
#include <unordered_map>
#include "TextureAtlasData.h"
#include "../../utils/IO.h"

arc::TextureAtlas::TextureAtlas(const std::string& file)
{
    load(file, arc::directory::dirName(file), false);
}

arc::TextureAtlas::~TextureAtlas() {
    for(auto* texture : textures)
        delete texture;
    for(auto* region : regions)
        delete region;
}

void arc::TextureAtlas::load(const std::string& file, const std::string& imagesDir, bool flip)
{
    TextureAtlasData data;
    data.load(file, imagesDir, flip);

    for (const auto& page : data.pages) {
        Texture2D* texture = nullptr;
        if(page.texture == nullptr)
        {
            // todo: apply filters
            auto dir = arc::directory::pathAppend(imagesDir, page.file);
            texture = Texture2D::loadFromFile(dir);
        }
        else
        {
            texture = page.texture;
            // todo: apply filters
        }

        textures.emplace_back(texture);
    }

    for (const auto& region : data.regions) {
        int width = region.width;
        int height = region.height;

        //auto page = data.pages[region.pageIndex];
        auto texture = textures[region.pageIndex];
        //auto texRegion = new TextureRegion(page.texture);
        //texRegion->setRegion(region.left, region.top, region.rotate ? height:width, region.rotate ? width:height);
        auto atlasRegion = new AtlasRegion(texture, region.left, region.top, region.rotate ? height:width, region.rotate ? width:height);
        atlasRegion->index = region.index;
        atlasRegion->name = region.name;
        atlasRegion->offsetX = region.offsetX;
        atlasRegion->offsetY = region.offsetY;
        atlasRegion->originalHeight = region.originalHeight;
        atlasRegion->originalWidth = region.originalWidth;
        atlasRegion->rotate = region.rotate;
        atlasRegion->degrees = region.degrees;
        atlasRegion->splits = region.splits;
        atlasRegion->pads = region.pads;
        if(region.flip) atlasRegion->flip(false, true);
        regions.emplace_back(atlasRegion);
    }
}

arc::AtlasRegion* arc::TextureAtlas::findRegion(const std::string& name) {
    for(auto* region : regions)
    {
        if(region->name == name)
            return region;
    }
    return nullptr;
}

arc::NinePatch* arc::TextureAtlas::createPatch(const std::string& name) {
    for(auto* region : regions)
    {
        if(region->name == name)
        {
            auto& splits = region->splits;
            auto& pads = region->pads;
            if(splits.empty()) throw std::runtime_error("region excpected to be ninepatch but no splits found");
            auto patch = new NinePatch(static_cast<TextureRegion*>(region), splits[0], splits[1], splits[2], splits[3]);
            if(!pads.empty())
                patch->setPadding(pads[0], pads[1], pads[2], pads[3]);
            return patch;
        }
    }
    return nullptr;
}

arc::AtlasRegion::AtlasRegion(arc::Texture2D* texture, int x, int y, int width, int height) {

    this->texture = texture;
    setRegion(x, y, width, height);
}

void arc::AtlasRegion::flip(bool x, bool y) {
    TextureRegion::flip(x, y);
    if (x) offsetX = originalWidth - offsetX - packedWidth;
    if (y) offsetY = originalHeight - offsetY - packedHeight;
}
