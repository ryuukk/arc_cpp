#include "RenderableBatch.h"

void arc::RenderableBatch::begin(arc::Camera* cam) {
    camera = cam;
    context.begin();
}

void arc::RenderableBatch::end() {
    flush();
    context.end();
}

void arc::RenderableBatch::flush() {
    // todo: sort

    IShader* currentShader = nullptr;
    for (int i = 0; i < renderables.size(); i++) {
        auto& renderable = renderables[i];
        if (currentShader != renderable->shader) {
            if (currentShader != nullptr) currentShader->end();
            currentShader = renderable->shader;
            currentShader->begin(camera, &context);
        }

        currentShader->render(renderable);
    }
    if (currentShader != nullptr)
    {
        currentShader->end();
    }


    // FIXME: pool causes a mem leak !!!
    // for the moment i'll manually handle a queue
    // i need to find a better way to handle this shit
    //pool.freeAll(renderables);


    //arc::Core::logger->info("Delete fucks");

    pool.delete_all();
    renderables.clear();

}

void arc::RenderableBatch::render(arc::IRenderableProvider* provider, arc::Environement* environement) {
    auto offset = renderables.size();

    provider->getRenderables(pool, renderables);

    for (int i = offset; i < renderables.size(); i++) {
        auto& renderable = renderables[i];
        renderable->environement = environement;
        renderable->shader = shaderProvider->getShader(renderable);
    }
}
