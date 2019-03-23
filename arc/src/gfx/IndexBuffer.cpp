#include "IndexBuffer.h"

arc::IndexBuffer::IndexBuffer(bool isStatic, int maxIndices) {
    _empty = maxIndices == 0;
    if (_empty) maxIndices = 1;
    _buffer.resize(maxIndices);

    _isDirect = true;
    glGenBuffers(1, &_bufferHandle);
    _usage = isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
}

void arc::IndexBuffer::setIndices(const std::vector<short>& indices, int offset, int count) {

    _isDirty = true;

    _buffer.resize(count);
    for (int i = 0; i < count; i++)
        _buffer[i] = indices[offset + i];

    if (_isBound) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _buffer.size() * 2, &_buffer[0], _usage);
        _isDirty = false;
    }
}

void arc::IndexBuffer::bind() {

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferHandle);
    if (_isDirty) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _buffer.size() * 2, &_buffer[0], _usage);
        _isDirty = false;
    }
    _isBound = true;
}

void arc::IndexBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    _isBound = false;
}

void arc::IndexBuffer::invalidate() {
    glGenBuffers(1, &_bufferHandle);
    _isDirty = true;
}

int arc::IndexBuffer::getNumIndices() {
    return _empty ? 0 : _buffer.size();
}

int arc::IndexBuffer::getNumMaxIndices() {
    return _empty ? 0 : _buffer.size();
}
