#pragma once

#include "Model.h"

namespace arc
{
    class ModelInstance
    {
    public:
        ModelInstance(Model& model);

        Model& model;
    };
}


