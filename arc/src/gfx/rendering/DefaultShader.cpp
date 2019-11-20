#include "DefaultShader.h"

#include <utility>
#include "../../Core.h"

arc::DefaultShader::DefaultShader(arc::Renderable* renderable, arc::DefaultShader::Config  config, ShaderProgram* program) : _renderable(renderable), _config(std::move(config))
{
    auto combinedMask = arc::DefaultShader::combineAttributeMasks(renderable);
    this->program = program;
    _lighting = renderable->environement != nullptr;

    _attributesMask = renderable->material->getMask() | _optionalAttributes;
    _vertexMask = renderable->meshPart.mesh->getVertexAttributes()->getMaskWithSizePacked();
}

void arc::DefaultShader::init() {
    u_projTrans = program->fetchUniformLocation("u_proj", false);
    u_viewTrans = program->fetchUniformLocation("u_viewTrans", false);
    u_projViewTrans = program->fetchUniformLocation("u_projViewTrans", false);
    u_cameraPosition = program->fetchUniformLocation("u_cameraPosition", false);
    u_cameraDirection = program->fetchUniformLocation("u_cameraDirection", false);
    u_cameraUp = program->fetchUniformLocation("u_cameraUp", false);
    u_cameraNearFar = program->fetchUniformLocation("u_cameraNearFar", false);

    u_time = program->fetchUniformLocation("u_time", false);


    u_worldTrans = program->fetchUniformLocation("u_worldTrans", false);
    u_viewWorldTrans = program->fetchUniformLocation("u_viewWorldTrans", false);
    u_projViewWorldTrans = program->fetchUniformLocation("u_projViewWorldTrans", false);
    u_normalMatrix = program->fetchUniformLocation("u_normalMatrix", false);
    u_bones = program->fetchUniformLocation("u_bones", false);

    u_shininess = program->fetchUniformLocation("u_shininess", false);
    u_opacity = program->fetchUniformLocation("u_opacity", false);
    u_diffuseColor = program->fetchUniformLocation("u_diffuseColor", false);
    u_diffuseTexture = program->fetchUniformLocation("u_diffuseTexture", false);
    u_diffuseUVTransform = program->fetchUniformLocation("u_diffuseUVTransform", false);
    u_specularColor = program->fetchUniformLocation("u_specularColor", false);
    u_specularTexture = program->fetchUniformLocation("u_specularTexture", false);
    u_specularUVTransform = program->fetchUniformLocation("u_specularUVTransform", false);
    u_emissiveColor = program->fetchUniformLocation("u_emissiveColor", false);
    u_emissiveTexture = program->fetchUniformLocation("u_emissiveTexture", false);
    u_emissiveUVTransform = program->fetchUniformLocation("u_emissiveUVTransform", false);
    u_reflectionColor = program->fetchUniformLocation("u_reflectionColor", false);
    u_reflectionTexture = program->fetchUniformLocation("u_reflectionTexture", false);
    u_reflectionUVTransform = program->fetchUniformLocation("u_reflectionUVTransform", false);
    u_normalTexture = program->fetchUniformLocation("u_normalTexture", false);
    u_normalUVTransform = program->fetchUniformLocation("u_normalUVTransform", false);
    u_ambientTexture = program->fetchUniformLocation("u_ambientTexture", false);
    u_ambientUVTransform = program->fetchUniformLocation("u_ambientUVTransform", false);
    u_alphaTest = program->fetchUniformLocation("u_alphaTest", false);


    u_ambientCubemap = program->fetchUniformLocation("u_ambientCubemap", false);
    u_environmentCubemap = program->fetchUniformLocation("u_environmentCubemap", false);
    u_dirLights0color = program->fetchUniformLocation("u_dirLights0color", false);
    u_dirLights0direction = program->fetchUniformLocation("u_dirLights0direction", false);
    u_dirLights1color = program->fetchUniformLocation("u_dirLights1color", false);
    u_pointLights0color = program->fetchUniformLocation("u_pointLights0color", false);
    u_pointLights0position = program->fetchUniformLocation("u_pointLights0position", false);
    u_pointLights0intensity = program->fetchUniformLocation("u_pointLights0intensity", false);
    u_pointLights1color = program->fetchUniformLocation("u_pointLights1color", false);
    u_spotLights0color = program->fetchUniformLocation("u_spotLights0color", false);
    u_spotLights0position = program->fetchUniformLocation("u_spotLights0position", false);
    u_spotLights0intensity = program->fetchUniformLocation("u_spotLights0intensity", false);
    u_spotLights0direction = program->fetchUniformLocation("u_spotLights0direction", false);
    u_spotLights0cutoffAngle = program->fetchUniformLocation("u_spotLights0cutoffAngle", false);
    u_spotLights0exponent = program->fetchUniformLocation("u_spotLights0exponent", false);
    u_spotLights1color = program->fetchUniformLocation("u_spotLights1color", false);
    u_fogColor = program->fetchUniformLocation("u_fogColor", false);
    u_shadowMapProjViewTrans = program->fetchUniformLocation("u_shadowMapProjViewTrans", false);
    u_shadowTexture = program->fetchUniformLocation("u_shadowTexture", false);
    u_shadowPCFOffset = program->fetchUniformLocation("u_shadowPCFOffset", false);
}

int arc::DefaultShader::compareTo(arc::IShader* other) {
    if (other == nullptr) return -1;
    if (other == this) return 0;
    return 0;
}

bool arc::DefaultShader::canRender(arc::Renderable* renderable)
{
    uint64_t renderableMask = 0;
    if (renderable->material != nullptr) renderableMask |= renderable->material->getMask();
    if (renderable->environement != nullptr) renderableMask |= renderable->environement->getMask();

    return (_attributesMask == (renderableMask | _optionalAttributes)) &&
            (_vertexMask == renderable->meshPart.mesh->getVertexAttributes()->getMaskWithSizePacked()) &&
            (renderable->environement != nullptr) == _lighting
            ;
}

void arc::DefaultShader::bindGlobal(arc::Camera* camera, arc::RenderContext* context) {
    if(u_time > 0)
    {
        _time += arc::Core::graphics->deltaTime();
        program->setUniformf(u_time, _time);
    }
    program->setUniformMat4(u_projViewTrans, camera->combined);

    // light ambient cubemap
}

void arc::DefaultShader::bind(arc::Renderable* renderable) {

    program->setUniformMat4(u_worldTrans, renderable->worldTransform);

	if (_config.numBones > 0 && renderable->bones != nullptr && !renderable->bones->empty())
	{
		auto count = renderable->bones->size() > _config.numBones ? _config.numBones : renderable->bones->size();

		program->setUniformMat4Array(u_bones, count, *renderable->bones);
	}

    if (!renderable->material->has(arc::BlendingAttribute::stype))
        context->setBlending(false, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    bindMaterial(renderable->material);

    if(_lighting)
        bindLights(renderable->environement);
}

void arc::DefaultShader::bindMaterial(arc::Attributes* attributes) {

    auto defaultCullFace = GL_BACK;
    auto defaultDepthFunc = GL_LEQUAL;

    auto cullFace = defaultCullFace;
    auto depthFunc = defaultDepthFunc;
    auto depthRangeNear = 0.0f;
    auto depthRangeFar = 1.0f;
    auto depthMask = true;

    if(attributes->has(arc::DiffuseTextureAttribute::stype))
    {
        auto* ta = attributes->get<arc::DiffuseTextureAttribute>(arc::DiffuseTextureAttribute::stype);
        ta->descriptor.texture->bind(0);
        program->setUniformi(u_diffuseTexture, 0);
        program->setUniform4f(u_diffuseUVTransform, ta->offsetU, ta->offsetV, ta->scaleU, ta->scaleV);
    }
    context->setCullFace(cullFace);
    context->setDepthTest(depthFunc, depthRangeNear, depthRangeFar);
    context->setDepthMask(depthMask);
}

void arc::DefaultShader::bindLights(arc::Environement* environement) {

}
