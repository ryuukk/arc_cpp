#pragma once

#include "IShader.h"
#include "../../math/Mat4.h"
#include "../MeshPart.h"
#include "../Material.h"
#include "../Environement.h"
#include "../../utils/object_pool.hpp"

namespace arc
{
    class IShader;
    class Renderable
    {
    public:
        Mat4 worldTransform = Mat4::identity();
        MeshPart meshPart{};

        Material* material = nullptr;
        Environement* environement = nullptr;
        std::vector<Mat4>* bones = nullptr;
        IShader* shader = nullptr;
    };

    class IRenderableProvider
    {
    public:
        virtual void getRenderables(DynamicObjectPool<Renderable>& pool, std::vector<Renderable*>& renderables) = 0;
    };
}