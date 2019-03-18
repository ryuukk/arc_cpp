#pragma once

#include "VertexAttributes.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace arc
{
    class VertexBuffer;

    class Mesh
    {
    public:
        Mesh(bool isStatic, int maxVertices, int maxIndices, VertexAttributes* attributes);

        ~Mesh() {
            delete _vertices;
            delete _indices;
            delete _attributes;
        }

        void bind(ShaderProgram* shader, std::vector<int>* locations);

        void unbind(ShaderProgram* shader, std::vector<int>* locations);

        void setVertices(std::vector<float>& vertices);

        void setVertices(std::vector<float>& vertices, int offset, int count);

        void setIndices(std::vector<short>& indices);

        void render(ShaderProgram* shader, int primitiveType);

        void render(ShaderProgram* shader, int primitiveType, int offset, int count, bool autoBind);

        bool autoBind = true;
    private:
        VertexBuffer* _vertices;
        IndexBuffer* _indices;
        VertexAttributes* _attributes;
    };
}