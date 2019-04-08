#include "DefaultShader.h"

arc::DefaultShader::DefaultShader(arc::Renderable* renderable, const arc::DefaultShader::Config& config, ShaderProgram* program) : _renderable(renderable), _config(config)
{
    this->program = program;
    _attributesMask = renderable->material->getMask() | _optionalAttributes;
    _vertexMask = renderable->meshPart.mesh->getVertexAttributes()->getMaskWithSizePacked();
}

void arc::DefaultShader::init() {

    // todo: i don't like this
    auto* program = this->program;
    this->program = nullptr;

    arc::BaseShader::init(program, _renderable);

    _renderable = nullptr;
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

void arc::DefaultShader::begin(arc::Camera* camera, arc::RenderContext* context) {
    arc::BaseShader::begin(camera, context);


    program->setUniformMat4("u_projViewTrans", camera->combined);

}

void arc::DefaultShader::render(arc::Renderable* renderable) {

    program->setUniformMat4("u_worldTrans", renderable->worldTransform);

    if(_config.numBones > 0 && !renderable->bones->empty())
        program->setUniformMat4Array("u_bones", _config.numBones, *renderable->bones);


    // MATERIAL
    auto cullFace = _config.defaultCullFace == -1 ? _defaultCullFace : _config.defaultCullFace;
    auto depthFunc = _config.defaultDepthFunc == -1 ? _defaultDepthFunc : _config.defaultDepthFunc;
    auto depthRangeNear = 0.0f;
    auto depthRangeFar = 1.0f;
    auto depthMask = true;

    if (renderable->material->has(DiffuseTextureAttribute::stype))
    {
        // todo: use binder from context
        auto* ta = renderable->material->get<DiffuseTextureAttribute>(DiffuseTextureAttribute::stype);
        ta->descriptor.texture->bind();
        //context.textureBinder.bind(ta.textureDescriptor);
        program->setUniformi("u_diffuseTexture", 0);
        program->setUniform4f("u_diffuseUVTransform", 0, 0, 1, 1);
    }

    context->setCullFace(cullFace);
    context->setDepthTest(depthFunc, depthRangeNear, depthRangeFar);
    context->setDepthMask(depthMask);
    // --

    arc::BaseShader::render(renderable);
}

void arc::DefaultShader::end() {
    arc::BaseShader::end();
}

void arc::DefaultShader::render(arc::Renderable* renderable, arc::Attributes* attributes) {
    if(!attributes->has(arc::BlendingAttribute::stype))
        context->setBlending(false, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    arc::BaseShader::render(renderable, attributes);
}
