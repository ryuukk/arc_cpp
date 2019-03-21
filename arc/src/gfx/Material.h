#pragma once

#include <string>
#include "material/Attributes.h"

namespace arc
{
    class Material: public Attributes
    {
    public:
        std::string id;

        Material* copy()
        {
          auto* ret = new Material;
          ret->id = id;
          for(auto& attr : attributes)
          {
              ret->set(attr->copy());
          }
          return ret;
        };
    };
}
