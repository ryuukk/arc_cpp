#include "AssetManager.h"
#include "../gfx/Texture2D.h"

std::shared_ptr<arc::Texture2D> arc::TextureLoader::load(std::string path) {

    auto* tex = arc::Texture2D::loadFromFile(path);
    return std::shared_ptr<arc::Texture2D>(tex);
}
