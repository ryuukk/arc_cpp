#include "TextureAtlas.h"
#include "TextureAtlasData.h"
#include "../../utils/DirUtils.h"

arc::TextureAtlas::TextureAtlas(const std::string& file)
{
    load(file, arc::directory::dirName(file), false);
}

void arc::TextureAtlas::load(const std::string& file, const std::string& imagesDir, bool flip)
{
    TextureAtlasData data;
    data.load(file, imagesDir, flip);
}
