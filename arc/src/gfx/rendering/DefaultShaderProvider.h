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

            std::string prefix = createPrefix(*renderable, config);

            std::string vs = "#version 330\n";
            std::string fs = "#version 330\n";


            vs.append(prefix).append("\n").append(config.vertexShader);
            fs.append(prefix).append("\n").append(config.fragmentShader);

            auto* program = new ShaderProgram(vs, fs);

            // todo: create prefix
            return new DefaultShader(renderable, config, program);
        }



    private:
        static std::string createPrefix(Renderable& renderable, const DefaultShader::Config& config)
        {

            auto& attributes = renderable.material;
            auto attributesMask = attributes->getMask();
            auto vertexMask = renderable.meshPart.mesh->getVertexAttributes()->getMask();

            std::string builder{};

            if (andd(vertexMask, VertexUsage::Position))
                builder.append("#define positionFlag\n");
            if (orr(vertexMask, VertexUsage::ColorUnpacked | VertexUsage::ColorPacked))
                builder.append("#define colorFlag\n");
            if (andd(vertexMask, VertexUsage::BiNormal))
                builder.append("#define binormalFlag\n");
            if (andd(vertexMask, VertexUsage::Tangent))
                builder.append("#define tangentFlag\n");
            if (andd(vertexMask, VertexUsage::Normal))
                builder.append("#define normalFlag\n");

            for (int i = 0; i < renderable.meshPart.mesh->getVertexAttributes()->size(); ++i) {
                auto& attr = renderable.meshPart.mesh->getVertexAttributes()->get(i);

                if (attr.usage == VertexUsage::BoneWeight)
                    builder.append("#define boneWeight").append(std::to_string(attr.unit).append("Flag\n"));
                else if (attr.usage == VertexUsage::TextureCoordinates)
                    builder.append("#define texCoord").append(std::to_string(attr.unit).append("Flag\n"));
            }

            if(andd(attributesMask, DiffuseTextureAttribute::stype))
            {
                builder.append("#define ").append(DiffuseTextureAttribute::alias).append("Flag\n");
                builder.append("#define ").append(DiffuseTextureAttribute::alias).append("Coord texCoord0\n");
            }

            if(config.numBones > 0 && !renderable.bones->empty())
                builder.append("#define numBones ").append(std::to_string(config.numBones)).append("\n");

            return builder;
        }

        static bool andd(uint64_t mask, uint64_t flag) {
            return (mask & flag) == flag;
        }

        static bool orr(uint64_t mask, uint64_t flag) {
            return (mask & flag) != 0;
        }
    };
}