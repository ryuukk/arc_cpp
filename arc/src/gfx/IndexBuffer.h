#pragma once

#include <glad/glad.h>
#include <vector>

namespace arc
{
    class IndexBuffer
    {
    public:
        IndexBuffer(bool isStatic, int maxIndices);

        ~IndexBuffer() {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &_bufferHandle);
        }

        void setIndices(const std::vector<short>& indices, int offset, int count);

        void bind();

        void unbind();

        void invalidate();

        int getNumIndices();

        int getNumMaxIndices();

    private:
        std::vector<short> _buffer;
        GLuint _bufferHandle;
        bool _isDirect = false;
        bool _isDirty = true;
        bool _isBound = false;
        int _usage = 0;
        bool _empty = false;
    };
}