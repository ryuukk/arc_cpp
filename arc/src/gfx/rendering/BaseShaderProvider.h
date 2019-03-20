#pragma once

#include "IShaderProvider.h"
#include "IShader.h"

namespace arc
{
    class BaseShaderProvider : public IShaderProvider
    {
    public:
        ~BaseShaderProvider()
        {
            for(auto& shader : shaders)
                delete shader;
        }

        IShader* getShader(Renderable* renderable) override
        {
            auto* suggestedShader = renderable->shader;
            if (suggestedShader != nullptr && suggestedShader->canRender(renderable)) return suggestedShader;
            for (auto& shader : shaders) {
                if (shader->canRender(renderable)) return shader;
            }
            auto* shader = createShader(renderable);
            shader->init();
            shaders.emplace_back(shader);
            return shader;

        }

    protected:
        virtual IShader* createShader(Renderable* renderable) = 0;

        std::vector<IShader*> shaders;
    };
}