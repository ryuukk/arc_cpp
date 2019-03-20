#include "BaseShader.h"

void arc::BaseShader::init(arc::ShaderProgram* program, arc::Renderable* renderable)
{
    // todo: check if compild
    this->program = program;
}

void arc::BaseShader::begin(Camera* camera, arc::RenderContext* context) {
    this->camera = camera;
    this->context = context;
    program->begin();
    currentMesh = nullptr;
    // set global uniforms
}

void arc::BaseShader::render(arc::Renderable* renderable) {

    // combine material and environment

    render(renderable, renderable->material);
}

void arc::BaseShader::end() {
    if(currentMesh != nullptr)
    {
        currentMesh->unbind(program, nullptr);
        currentMesh = nullptr;
    }

    program->end();
}

void arc::BaseShader::render(arc::Renderable* renderable, arc::Attributes* attributes)
{
    // setters

    if(currentMesh != renderable->meshPart->mesh)
    {
        if(currentMesh != nullptr)
            currentMesh->unbind(program, nullptr);

        currentMesh = renderable->meshPart->mesh;
        currentMesh->bind(program, nullptr);
    }

    renderable->meshPart->render(program, false);
}
