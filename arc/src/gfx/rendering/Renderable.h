#pragma once

#include "IShader.h"
#include "../../math/Mat4.h"
#include "../MeshPart.h"
#include "../Material.h"

namespace arc
{
    class IShader;
    class Renderable
    {
    public:
        Mat4 worldTransform = Mat4::identity();

        MeshPart* meshPart = nullptr;
        Material* material = nullptr;
        std::vector<Mat4>* bones = nullptr;
        IShader* shader = nullptr;
    };
}