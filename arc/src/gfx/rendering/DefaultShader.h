#pragma once


#include "BaseShader.h"

namespace arc
{
    class DefaultShader : public BaseShader
    {
    public:
        struct Config
        {
            std::string vertexShader;
            std::string fragmentShader;
            int numDirectionalLights = 2;
            int numPointLights = 5;
            int numSpotLights = 0;
            int numBones = 20;
            bool ignoreUnimplemented = true;
            int defaultCullFace = -1;
            int defaultDepthFunc = -1;
        };

    public:
        DefaultShader(Renderable* renderable, const Config& config, ShaderProgram* program);

        void init() override;

        int compareTo(IShader* other) override;

        bool canRender(Renderable* renderable) override;

        void begin(Camera* camera, RenderContext* context) override;

        void render(Renderable* renderable) override;

        void end() override;

        void render(Renderable* renderable, Attributes* attributes) override;

    private:
        Renderable* _renderable = nullptr;
        Config _config;
        uint64_t _attributesMask{};
        uint64_t _vertexMask{};
        uint64_t _optionalAttributes{};

        uint32_t _defaultCullFace = GL_BACK;
        uint32_t _defaultDepthFunc = GL_LEQUAL;

        bool _lighting{};
    };

}

