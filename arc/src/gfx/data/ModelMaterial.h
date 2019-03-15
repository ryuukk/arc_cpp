#pragma once

#include <vector>
#include <string>
#include "../../Color.h"
#include "ModelTexture.h"

namespace arc
{
    class ModelMaterial
    {
    public:
        enum MaterialType
        {
            Lambert,
            Phong
        };

        std::string id;

        MaterialType type;

        Color ambient;
        Color diffuse;
        Color specular;
        Color emissive;
        Color reflection;

        float shininess{};
        float opacity = 1.0f;

        std::vector<ModelTexture> textures;
    };
}


