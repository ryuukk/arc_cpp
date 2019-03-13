#pragma once

#include <vector>
#include <string>

namespace arc
{
    class VertexAttribute
    {
    public:
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
        VertexAttributes(VertexAttribute &attribute...);
        int size();
        VertexAttribute get(int index);

        int vertexSize;
    private:
        int calculateOffsets();

        std::vector<VertexAttribute> _attributes;
        unsigned long _mask = 0;
    };
}