#include "BaseShader.h"

void arc::BaseShader::begin(Camera* camera, arc::RenderContext* context) {
    this->camera = camera;
    this->context = context;
    program->begin();
    currentMesh = nullptr;

    bindGlobal(camera, context);
}

void arc::BaseShader::render(arc::Renderable* renderable) {

    if(currentMesh != renderable->meshPart.mesh)
    {
        if(currentMesh != nullptr)
            currentMesh->unbind(program, nullptr);

        currentMesh = renderable->meshPart.mesh;
        currentMesh->bind(program, nullptr);
    }

    bind(renderable);

    renderable->meshPart.render(program, false);
}

void arc::BaseShader::end() {
    if(currentMesh != nullptr)
    {
        currentMesh->unbind(program, nullptr);
        currentMesh = nullptr;
    }

    program->end();
}
