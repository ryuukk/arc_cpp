#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "../TextureDescriptor.h"
#include "../Texture2D.h"

namespace arc
{
    struct Attribute
    {
        Attribute(uint64_t type): type(type)
        {}

        uint64_t type;

    private:
        bool registered = false;

    public:
        static std::vector<std::string> types;

        static uint64_t getAttributeType(const std::string& alias)
        {
            for (int i = 0; i < types.size(); ++i) {
                if(types[i] == alias)
                    return 1UL << i;
            }
            return 0UL;
        }

        static std::string getAttributeAlias(const uint64_t type)
        {
            int idx = -1;
            while (type != 0 && ++idx < 63 && (((type >> idx) & 1) == 0))
            {
            }
            return (idx >= 0 && idx < types.size()) ? types[idx] : "";
        }

        static uint64_t registerr(const std::string& alias)
        {
            uint64_t result = getAttributeType(alias);
            if (result > 0)
                return result;
            types.emplace_back(alias);
            return 1UL << (types.size() - 1);
        }
    };

    struct DiffuseTextureAttribute : public Attribute
    {
    public:
        static std::string alias;
        static uint64_t stype;

    public:
        DiffuseTextureAttribute(Texture2D* texture) : Attribute(stype)
        {
            descriptor.texture = texture;
        }

        TextureDescriptor<Texture2D> descriptor;
        float offsetU = 0;
        float offsetV = 0;
        float scaleU = 1;
        float scaleV = 1;
        int uvIndex = 0;

    };

    struct CullfaceAttribute : public Attribute
    {
    public:
        static std::string alias;
        static uint64_t stype;

    public:
        CullfaceAttribute(int value) : Attribute(stype)
        {
            this->value = value;
        }

        int value = 0;

    };

    struct DepthTestAttribute : public Attribute
    {
    public:
        static std::string alias;
        static uint64_t stype;

    public:
        DepthTestAttribute() : Attribute(stype)
        {
        }

        int depthFunc{};
        float depthRangeNear{};
        float depthRangeFar{};
        bool depthMask{};

    };

    struct BlendingAttribute : public Attribute
    {
    public:
        static std::string alias;
        static uint64_t stype;

    public:
        BlendingAttribute() : Attribute(stype)
        {
        }

        bool blended{};
        int sourceFunction{};
        int destFunction{};
        float opacity = 1.0f;
    };
}