#pragma once

#include <string>
#include "material/Attributes.h"

namespace arc
{
    class Material: public Attributes
    {
    public:
        std::string id;

        Material() = default;
        explicit Material(Attribute* attribute);


        Material* copy();
    };
}
