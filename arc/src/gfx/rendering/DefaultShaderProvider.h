#pragma once

#include "BaseShaderProvider.h"
#include "DefaultShader.h"

namespace arc
{
    class DefaultShaderProvider : public BaseShaderProvider
    {
    public:
        DefaultShaderProvider(const std::string& vs, const std::string& fs)
        {
            config.vertexShader = vs;
            config.fragmentShader = fs;
        }

        DefaultShader::Config config;
    protected:
        IShader* createShader(Renderable* renderable) override {

            return new DefaultShader(renderable, config);
        }
    };
}