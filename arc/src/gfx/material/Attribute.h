#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "../TextureDescriptor.h"
#include "../Texture2D.h"
#include "../../Color.h"

namespace arc
{
    struct Attribute
    {
        explicit Attribute(uint64_t type) : type(type) {}

        uint64_t type;

        virtual Attribute* copy() = 0;


    private:
        bool registered = false;

    public:
        static std::vector<std::string> types;

        static uint64_t getAttributeType(const std::string& alias) {
            for (int i = 0; i < types.size(); ++i) {
                if (types[i] == alias)
                    return 1UL << i;
            }
            return 0UL;
        }

        static std::string getAttributeAlias(const uint64_t type) {
            int idx = -1;
            while (type != 0 && ++idx < 63 && (((type >> idx) & 1) == 0)) {
            }
            return (idx >= 0 && idx < types.size()) ? types[idx] : "";
        }

        static uint64_t registerr(const std::string& alias) {
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
        explicit DiffuseTextureAttribute(Texture2D* texture) : Attribute(stype) {
            descriptor.texture = texture;
        }

        TextureDescriptor<Texture2D> descriptor;
        float offsetU = 0;
        float offsetV = 0;
        float scaleU = 1;
        float scaleV = 1;
        int uvIndex = 0;

        Attribute* copy() override {
            auto* ret = new DiffuseTextureAttribute(descriptor.texture);
            return ret;
        }

    };

    struct DiffuseColorAttribute : public Attribute
    {
    public:
        static std::string alias;
        static uint64_t stype;

    public:
        explicit DiffuseColorAttribute(Color color) : Attribute(stype) {
            this->color = color;
        }

        Color color;

        Attribute* copy() override {
            auto* ret = new DiffuseColorAttribute(color);
            return ret;
        }

    };

    struct CullfaceAttribute : public Attribute
    {
    public:
        static std::string alias;
        static uint64_t stype;

    public:
        explicit CullfaceAttribute(int value) : Attribute(stype) {
            this->value = value;
        }

        int value = 0;

        Attribute* copy() override {
            auto* ret = new CullfaceAttribute(value);
            return ret;
        }

    };

    struct DepthTestAttribute : public Attribute
    {
    public:
        static std::string alias;
        static uint64_t stype;

    public:
        explicit DepthTestAttribute() : Attribute(stype) {
        }

        int depthFunc{};
        float depthRangeNear{};
        float depthRangeFar{};
        bool depthMask{};

        Attribute* copy() override {
            auto* ret = new DepthTestAttribute();
            ret->depthFunc = depthFunc;
            ret->depthRangeNear = depthRangeNear;
            ret->depthRangeFar = depthRangeFar;
            ret->depthMask = depthMask;
            return ret;
        }

    };

    struct BlendingAttribute : public Attribute
    {
    public:
        static std::string alias;
        static uint64_t stype;

    public:
        explicit BlendingAttribute() : Attribute(stype) {
        }

        bool blended{};
        int sourceFunction{};
        int destFunction{};
        float opacity = 1.0f;

        Attribute* copy() override {
            auto* ret = new BlendingAttribute();
            ret->blended = blended;
            ret->sourceFunction = sourceFunction;
            ret->destFunction = destFunction;
            ret->opacity = opacity;
            return ret;
        }
    };
}