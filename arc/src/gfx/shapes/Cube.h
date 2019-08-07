#pragma once


#include "../rendering/Renderable.h"

namespace arc
{
    class Cube : public IRenderableProvider
    {
    public:
        explicit Cube(Color color);

        void getRenderables(DynamicObjectPool<Renderable>& pool, std::vector<Renderable*>& renderables) override;
    private:
        Mesh _mesh;
        Material _material;
    };
}


