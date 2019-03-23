#pragma once

#include <glad/glad.h>
#include "ShaderProgram.h"
#include "VertexAttributes.h"

namespace arc
{
    enum VertexUsage
    {
        Position = 1,
        ColorUnpacked = 2,
        ColorPacked = 4,
        Normal = 8,
        TextureCoordinates = 16,
        Generic = 32,
        BoneWeight = 64,
        Tangent = 128,
        BiNormal = 256,
    };

    class ShaderProgram;
    class VertexBuffer
    {
    public:
        VertexBuffer(bool isStatic, int numVertices, VertexAttributes* attributes);

        ~VertexBuffer() {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &_bufferHandle);
            glDeleteVertexArrays(1, &_vaoHandle);
        }

        int getNumVertices();

        int getNumMaxVertices();

        void setVertices(const std::vector<float>& vertices, int offset, int count);

        void bind(ShaderProgram* shader, std::vector<int>* locations);

        void unbind(ShaderProgram* shader, std::vector<int>* locations);

        void invalidate();

    private:
        void bufferChanged();

        void bindAttributes(ShaderProgram* shader, std::vector<int>* locations);

        void unbindAttributes(ShaderProgram* shader);

        void bindData();

        void createVAO();

    private:
        VertexAttributes* _attributes = nullptr;
        GLuint _bufferHandle;
        bool _isStatic = false;
        int _usage = 0;
        bool _isDirty = false;
        bool _isBound = false;
        GLuint _vaoHandle;

        std::vector<float> _vertices;
        std::vector<int> _cachedLocations;
    };

}