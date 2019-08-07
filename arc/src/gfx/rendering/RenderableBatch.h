#pragma once

#include "../Camera.h"
#include "Renderable.h"
#include "IShaderProvider.h"
#include "../ModelInstance.h"
#include "../../utils/object_pool.hpp"

namespace arc
{
    class RenderableBatch
    {
    public:
        RenderableBatch(IShaderProvider* provider) : shaderProvider(provider)
        {
            renderables.reserve(16*16);
        }

        ~RenderableBatch()
        {
            // pool.clear();
            delete shaderProvider;
        }

        std::vector<Renderable*> renderables;
        RenderContext context{};
        DynamicObjectPool<Renderable> pool{64};

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


            // FIXME: pool causes a mem leak !!!
            // for the moment i'll manually handle a queue
            // i need to find a better way to handle this shit
           //pool.freeAll(renderables);

            for (int i = 0; i < renderables.size(); ++i) {
                //queue.push(renderables[i]);
                pool.delete_object(renderables[i]);
            }
           renderables.clear();
        }
/*
        void render(ModelInstance* model, Environement* environement = nullptr)
        {
            auto offset = renderables.size();

            for(auto& node : model->nodes)
            {
                for(auto& part : node->parts)
                {
                    Renderable* renderable = pool.new_object();
                    // clean it here, remove when pool is fixed
                    renderable->environement = nullptr;
                    renderable->material = nullptr;
                    renderable->meshPart.set("", nullptr, 0, 0, 0);
                    renderable->shader = nullptr;
                    renderable->bones = nullptr;
                    renderable->worldTransform = Mat4::identity();
                    // --

                    renderable->material = part->material;
                    renderable->bones = &part->bones;
                    renderable->meshPart.set(part->meshPart);

                    if(part->bones.empty())
                        renderable->worldTransform = model->transform * node->globalTransform;
                    else
                        renderable->worldTransform = model->transform;

                    renderables.emplace_back(renderable);
                }
            }

            for (int i = offset; i < renderables.size(); i++) {
                auto& renderable = renderables[i];
                renderable->environement = environement;
                renderable->shader = shaderProvider->getShader(renderable);
            }
        }
*/

        void render(IRenderableProvider* provider, Environement* environement = nullptr)
        {
            auto offset = renderables.size();

            provider->getRenderables(pool, renderables);

            for (int i = offset; i < renderables.size(); i++) {
                auto& renderable = renderables[i];
                renderable->environement = environement;
                renderable->shader = shaderProvider->getShader(renderable);
            }
        }

    private:
    };
}