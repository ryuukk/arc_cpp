#pragma once

#include "IShader.h"
#include "../Camera.h"
#include "../Mesh.h"
#include "../material/Attributes.h"

namespace arc
{
    class BaseShader : public IShader
    {
    public:
        ~BaseShader()
        {}

        void init(ShaderProgram* program, Renderable* renderable);

        void begin(Camera* camera, RenderContext* context) override;

        void render(Renderable* renderable) override;

        void end() override;

        virtual void render(Renderable* renderable, Attributes* attributes);

        ShaderProgram* program = nullptr;
        RenderContext* context = nullptr;
        Camera* camera = nullptr;

    private:
        std::vector<std::string> uniforms;
        std::vector<int32_t> locations;
        std::vector<int32_t> globalUniforms;
        std::vector<int32_t> localUniforms;
        std::unordered_map<int32_t, int32_t> attributes;

        Mesh* currentMesh = nullptr;
    };
}

