#include "VertexBuffer.h"

arc::VertexBuffer::VertexBuffer(bool isStatic, int numVertices, VertexAttributes* attributes)
{
    _attributes = attributes;
    _isStatic = isStatic;
    _vertices.resize(numVertices * (_attributes->vertexSize / 4));

    glGenBuffers(1, &_bufferHandle);

    _usage = _isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;

    createVAO();
}

int arc::VertexBuffer::getNumVertices() {
    return _vertices.size() / (_attributes->vertexSize / 4);
}

int arc::VertexBuffer::getNumMaxVertices() {
    return _vertices.size() / (_attributes->vertexSize / 4);
}

void arc::VertexBuffer::setVertices(std::vector<float> vertices, int offset, int count) {
    _isDirty = true;

    _vertices.resize(count);
    for(int i = 0; i < count; i++)
        _vertices[i] = vertices[offset + i];

    bufferChanged();
}

void arc::VertexBuffer::bind(ShaderProgram *shader, std::vector<int> *locations) {
    glBindVertexArray(_vaoHandle);

    bindAttributes(shader, locations);

    //if our data has changed upload it:
    bindData();

    _isBound = true;
}

void arc::VertexBuffer::unbind(ShaderProgram *shader, std::vector<int> *locations) {
    glBindVertexArray(0);
    _isBound = false;
}

void arc::VertexBuffer::invalidate() {
    glGenBuffers(1, &_bufferHandle);
    createVAO();
    _isDirty = true;
}

void arc::VertexBuffer::bufferChanged() {
    if (_isBound)
    {
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), &_vertices[0], _usage);
        _isDirty = false;
    }
}

void arc::VertexBuffer::bindAttributes(ShaderProgram *shader, std::vector<int> *locations) {

    bool stillValid = !_cachedLocations.empty();
    int numAttributes = _attributes->size();

    if (stillValid)
    {
        if (locations == nullptr)
        {
            for (int i = 0; stillValid && i < numAttributes; i++)
            {
                VertexAttribute attribute = _attributes->get(i);
                int location = shader->getAttributeLocation(attribute.aliass);
                stillValid = location == _cachedLocations[i];
            }
        }
#ifdef false
        else
        {
            std::vector<int>& vecRef = *locations;
            stillValid = locations->size() == _cachedLocations.size();
            for (int i = 0; stillValid && i < numAttributes; i++)
            {
                stillValid = vecRef[i] == _cachedLocations[i];
            }
        }
#endif
    }

    if (!stillValid)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _bufferHandle);
        unbindAttributes(shader);

        _cachedLocations.clear();

        for (int i = 0; i < numAttributes; i++)
        {
            VertexAttribute attribute = _attributes->get(i);
            if (locations == nullptr)
            {
                int l = shader->getAttributeLocation(attribute.aliass);


                printf("CACHE LOC: %d NAME: %s\n",l, attribute.aliass.c_str());
                _cachedLocations.emplace_back(l);
            }
#ifdef false
            else
            {
                std::vector<int>& vecRef = *locations;
                _cachedLocations[i] = vecRef[i];
            }
#endif

            int location = _cachedLocations[i];
            if (location < 0)
            {
                continue;
            }

            shader->enableVertexAttribute(location);
            shader->setVertexAttribute(location, attribute.numComponents, attribute.type, attribute.normalized, _attributes->vertexSize, attribute.offset);
        }
    }
}

void arc::VertexBuffer::unbindAttributes(ShaderProgram* shader) {

    if (_cachedLocations.empty())
    {
        return;
    }

    int numAttributes = _attributes->size();
    for (int i = 0; i < numAttributes; i++)
    {
        int location = _cachedLocations[i];
        printf("UNBIND LOC: %d\n", location);
        if (location < 0)
        {
            continue;
        }
        shader->disableVertexAttribute(location);
    }
}

void arc::VertexBuffer::bindData() {

    if (_isDirty)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _bufferHandle);
        glBufferData(GL_ARRAY_BUFFER, (_vertices.size() * sizeof(float)), &_vertices[0], _usage);
        _isDirty = false;
    }
}

void arc::VertexBuffer::createVAO() {
    glGenVertexArrays(1, &_vaoHandle);
}
