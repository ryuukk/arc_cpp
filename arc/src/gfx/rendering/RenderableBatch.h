#pragma once

#include "../Camera.h"
#include "Renderable.h"
#include "IShaderProvider.h"
#include "../ModelInstance.h"
#include "../../utils/object_pool.hpp"
#include "../../Core.h"

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


        IShaderProvider* shaderProvider = nullptr;

        Camera* camera = nullptr;

        void begin(Camera* cam);

        void end();

        void flush();
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

        void render(IRenderableProvider* provider, Environement* environement = nullptr);

    private:
        std::vector<Renderable*> renderables;
        RenderContext context{};
        DynamicObjectPool<Renderable> pool{64};

    };
}