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
            renderables.reserve(16*16);
        }

        ~RenderableBatch()
        {
            // pool.clear();
            delete shaderProvider;
        }

        std::queue<Renderable*> queue;
        std::vector<Renderable*> renderables;
        RenderContext context{};


        //RenderablePool pool{};

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
                queue.push(renderables[i]);
            }
           renderables.clear();
        }

        void render(ModelInstance* model, Environement* environement = nullptr)
        {
            auto offset = renderables.size();

            for(auto& node : model->nodes)
            {
                for(auto& part : node->parts)
                {
                    Renderable* renderable = nullptr;

                    if(queue.empty())
                        renderable = new Renderable();
                    else
                    {
                        renderable = queue.front();
                        // clean it here, remove when pool is fixed
                        renderable->environement = nullptr;
                        renderable->material = nullptr;
                        renderable->meshPart.set("", nullptr, 0, 0, 0);
                        renderable->shader = nullptr;
                        renderable->bones = nullptr;
                        renderable->worldTransform = Mat4::identity();
                        // --
                        queue.pop();
                    }
                    renderable->environement = environement;
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
                renderable->shader = shaderProvider->getShader(renderable);
            }
        }

    private:
    };
}