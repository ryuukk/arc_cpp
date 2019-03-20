#pragma once

#include <vector>
#include <string>

namespace arc
{
    class VertexAttribute
    {
    public:
        static VertexAttribute position();

        static VertexAttribute normal();

        static VertexAttribute colorPacked();

        static VertexAttribute colorUnpacked();

        static VertexAttribute tangent();

        static VertexAttribute binormal();

        static VertexAttribute texCoords(int unit);

        static VertexAttribute boneWeight(int unit);

    public:
        VertexAttribute() {};
        VertexAttribute(int usage, int numComponents, int type, bool normalized, std::string aliass, int unit = 0);
        VertexAttribute(int usage, int numComponents, std::string aliass, int unit = 0);
        int usage = 0;
        int numComponents = 0;
        bool normalized = false;
        int type = 0;
        int offset = 0;
        std::string aliass;
        int unit = 0;

        int getKey();
        int getSizeInBytes();
    private:
        int _usageIndex;
    };

    class VertexAttributes
    {
    public:
        VertexAttributes(){};
        VertexAttributes(const std::vector<VertexAttribute> attributes);
        int size();
        VertexAttribute& get(int index);
        uint64_t getMask();
        uint64_t getMaskWithSizePacked();
        int vertexSize{};
    private:
        int calculateOffsets();

        std::vector<VertexAttribute> _attributes;
        uint64_t _mask = 0;
    };
}
