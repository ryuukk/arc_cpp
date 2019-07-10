#include "Texture2D.h"
#include "../utils/IO.h"
#include "../utils/Format.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../utils/stb_image.h"

arc::Texture2D* arc::Texture2D::loadFromFile(const std::string& filename) {

    if(!arc::file::exists(filename))
        throw std::runtime_error(arc::Format("Texture not found: {0}", filename));
    int x,y,n;
    unsigned char *data = stbi_load(filename.c_str(), &x, &y, &n, 0);

    // todo: check if file exists, and figure out what to do in case of error..

    auto* texture = new Texture2D();
    texture->setData(data, x, y);

    return texture;
}
