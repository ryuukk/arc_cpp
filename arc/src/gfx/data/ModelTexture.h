#pragma once

#include <string>
#include "../../math/Vec2.h"

namespace arc
{
    class ModelTexture
    {
    public:
        static const int USAGE_UNKNOWN = 0;
        static const int USAGE_NONE = 1;
        static const int USAGE_DIFFUSE = 2;
        static const int USAGE_EMISSIVE = 3;
        static const int USAGE_AMBIENT = 4;
        static const int USAGE_SPECULAR = 5;
        static const int USAGE_SHININESS = 6;
        static const int USAGE_NORMAL = 7;
        static const int USAGE_BUMP = 8;
        static const int USAGE_TRANSPARENCY = 9;
        static const int USAGE_REFLECTION = 10;

        std::string id;
        std::string fileName;
        Vec2 uvTranslation;
        Vec2 uvScaling;
        int usage;
    };
}
