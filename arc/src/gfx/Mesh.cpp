#include "Mesh.h"

arc::Mesh::Mesh(bool isStatic, int maxVertices, int maxIndices, arc::VertexAttributes* attributes) {
    _attributes = attributes;
    _vertices = new VertexBuffer(isStatic, maxVertices, _attributes);
    _indices = new IndexBuffer(isStatic, maxIndices);
}

void arc::Mesh::bind(arc::ShaderProgram* shader, std::vector<int>* locations) {
    _vertices->bind(shader, locations);
    if (_indices->getNumIndices() > 0)
        _indices->bind();
}

void arc::Mesh::unbind(arc::ShaderProgram* shader, std::vector<int>* locations) {
    _vertices->unbind(shader, locations);
    if (_indices->getNumIndices() > 0)
        _indices->unbind();
}

void arc::Mesh::setVertices(const std::vector<float>& vertices) {
    _vertices->setVertices(vertices, 0, vertices.size());
}

void arc::Mesh::setVertices(const std::vector<float>& vertices, int offset, int count) {
    _vertices->setVertices(vertices, offset, count);
}

void arc::Mesh::setIndices(const std::vector<short>& indices) {
    _indices->setIndices(indices, 0, indices.size());
}

void arc::Mesh::render(arc::ShaderProgram* shader, int primitiveType) {
    render(shader, primitiveType, 0,
           _indices->getNumMaxIndices() > 0 ? _indices->getNumIndices() : _vertices->getNumVertices(), autoBind);
}

void arc::Mesh::render(arc::ShaderProgram* shader, int primitiveType, int offset, int count, bool autoBind) {

    if (count == 0)
        return;

    if (autoBind)
        bind(shader, nullptr);


    if (_indices->getNumIndices() > 0) {
        auto orr = offset * 2;
        glDrawElements(primitiveType, count, GL_UNSIGNED_SHORT, (int*) orr);
    } else {
        glDrawArrays(primitiveType, offset, count);
    }

    if (autoBind)
        unbind(shader, nullptr);
}

arc::VertexAttributes* arc::Mesh::getVertexAttributes() {
    return _attributes;
}
