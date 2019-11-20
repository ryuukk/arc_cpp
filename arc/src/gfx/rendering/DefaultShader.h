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
        DefaultShader(Renderable* renderable, Config  config, ShaderProgram* program);

        void init() override;

        int compareTo(IShader* other) override;

        bool canRender(Renderable* renderable) override;

        void bindGlobal(Camera* camera, RenderContext* context) override;
        void bind(Renderable* renderable) override;

    private:
        void bindMaterial(Attributes* attributes);
        void bindLights(Environement* environement);

    public:
        // Global uniforms
        int u_projTrans;
        int u_viewTrans;
        int u_projViewTrans;
        int u_cameraPosition;
        int u_cameraDirection;
        int u_cameraUp;
        int u_cameraNearFar;

        int u_time;


        // Object uniforms
        int u_worldTrans;
        int u_viewWorldTrans;
        int u_projViewWorldTrans;
        int u_normalMatrix;
        int u_bones;

        // Material uniforms
        int u_shininess;
        int u_opacity;
        int u_diffuseColor;
        int u_diffuseTexture;
        int u_diffuseUVTransform;
        int u_specularColor;
        int u_specularTexture;
        int u_specularUVTransform;
        int u_emissiveColor;
        int u_emissiveTexture;
        int u_emissiveUVTransform;
        int u_reflectionColor;
        int u_reflectionTexture;
        int u_reflectionUVTransform;
        int u_normalTexture;
        int u_normalUVTransform;
        int u_ambientTexture;
        int u_ambientUVTransform;
        int u_alphaTest;

        // Light uniforms
        int u_ambientCubemap;
        int u_environmentCubemap;
        int u_dirLights0color;
        int u_dirLights0direction;
        int u_dirLights1color;
        int u_pointLights0color;
        int u_pointLights0position;
        int u_pointLights0intensity;
        int u_pointLights1color;
        int u_spotLights0color;
        int u_spotLights0position;
        int u_spotLights0intensity;
        int u_spotLights0direction;
        int u_spotLights0cutoffAngle;
        int u_spotLights0exponent;
        int u_spotLights1color;
        int u_fogColor;
        int u_shadowMapProjViewTrans;
        int u_shadowTexture;
        int u_shadowPCFOffset;

    private:
        Renderable* _renderable = nullptr;
        Config _config;
        uint64_t _attributesMask{};
        uint64_t _vertexMask{};
        uint64_t _optionalAttributes{};

        uint32_t _defaultCullFace = GL_BACK;
        uint32_t _defaultDepthFunc = GL_LEQUAL;

        bool _lighting{};
        bool _environmentCubemap{};
        bool _shadowMap{};

        float _time{};

        inline static uint64_t combineAttributeMasks(Renderable* renderable)
        {
            auto mask = 0UL;
            if(renderable->environement != nullptr) mask |= renderable->environement->getMask();
            if(renderable->material != nullptr) mask |= renderable->material->getMask();
            return mask;
        }
    };

}

