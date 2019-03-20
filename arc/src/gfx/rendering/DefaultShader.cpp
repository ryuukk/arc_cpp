#include "DefaultShader.h"

arc::DefaultShader::DefaultShader(arc::Renderable* renderable, const arc::DefaultShader::Config& config) : _renderable(renderable), _config(config) {

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
    // if (renderable->environment != nullptr) mask |= renderable->environment.getMask();

    return (_attributesMask == (renderableMask | _optionalAttributes))
           && (_vertexMask == renderable->meshPart->mesh->getVertexAttributes()->getMaskWithSizePacked()) /*&& (renderable.environment != null) == lighting*/
            ;
}

void arc::DefaultShader::begin(arc::Camera* camera, arc::RenderContext* context) {
    arc::BaseShader::begin(camera, context);

    // todo: set lights
}

void arc::DefaultShader::render(arc::Renderable* renderable) {

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
