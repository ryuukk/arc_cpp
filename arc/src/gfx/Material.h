#pragma once

#include <string>
#include "material/Attributes.h"

namespace arc
{
    class Material: public Attributes
    {
    public:
        std::string id;
    };
}
