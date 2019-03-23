#pragma once

#include "../Camera.h"
#include "../../Pool.h"
#include "Renderable.h"
#include "IShaderProvider.h"
#include "../ModelInstance.h"

namespace arc
{
    class RenderablePool : public Pool<Renderable>
    {
    public:
        Renderable* obtain() override
        {
            auto* ret = Pool::obtain();
            ret->environement = nullptr;
            ret->material = nullptr;
            ret->meshPart.set("", nullptr, 0, 0, 0);
            ret->shader = nullptr;
            ret->bones = nullptr;
            ret->worldTransform = Mat4::identity();
            return ret;
        }

    protected:
        Renderable* newObject() override
        {
            return new Renderable;
        }

    };

    class RenderableBatch
    {
    public:
        RenderableBatch(IShaderProvider* provider) : shaderProvider(provider)
        {

        }

        ~RenderableBatch()
        {
            delete shaderProvider;
        }

        std::vector<Renderable*> renderables;
        RenderContext context{};
        RenderablePool pool{};
        IShaderProvider* shaderProvider = nullptr;

        Camera* camera = nullptr;

        void begin(Camera* cam)
        {
            camera = cam;
            context.begin();
        }

        void end()
        {
            flush();
            context.end();
        }

        void flush()
        {
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
            if (currentShader != nullptr) currentShader->end();

            pool.freeAll(renderables);
            renderables.clear();
        }

        void render(ModelInstance* model, Environement* environement = nullptr)
        {
            auto offset = renderables.size();

            for(auto& node : model->nodes)
            {
                for(auto& part : node->parts)
                {
                    auto* renderable = pool.obtain();
                    renderable->environement = environement;
                    renderable->material = part->material;
                    renderable->bones = &part->bones;
                    renderable->meshPart.set(part->meshPart);

                    if(!part->bones.empty())
                        renderable->worldTransform = model->transform * node->globalTransform;
                    else
                        renderable->worldTransform = model->transform;

                    renderables.emplace_back(renderable);
                }
            }

            for (int i = offset; i < renderables.size(); i++) {
                auto& renderable = renderables[i];
                renderable->shader = shaderProvider->getShader(renderable);
            }
        }

    private:
    };
}