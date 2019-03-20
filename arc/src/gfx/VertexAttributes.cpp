#include <cstdarg>
#include <stdexcept>
#include <glad/glad.h>
#include "VertexAttributes.h"
#include "VertexBuffer.h"


arc::VertexAttributes::VertexAttributes(const std::vector<VertexAttribute> attributes) {
    _attributes = attributes;

    vertexSize = calculateOffsets();
}

int arc::VertexAttributes::size()
{
    return _attributes.size();
}

arc::VertexAttribute& arc::VertexAttributes::get(int index)
{
    return _attributes[index];
}

int arc::VertexAttributes::calculateOffsets() {

    int count = 0;
    for (int i = 0; i < _attributes.size(); i++) {
        auto& attribute = _attributes[i];
        attribute.offset = count;
        count += attribute.getSizeInBytes();
    }

    return count;
}

uint64_t arc::VertexAttributes::getMask() {
    if (_mask == 0) {
        uint64_t result = 0;
        for (int i = 0; i < _attributes.size(); i++) {
            result |= _attributes[i].usage;
        }
        _mask = result;
    }
    return _mask;
}

uint64_t arc::VertexAttributes::getMaskWithSizePacked() {
    return getMask() | (_attributes.size() << 32);
}

int arc::VertexAttribute::getKey() {
    return (_usageIndex << 8) + (unit & 0xFF);
}

int arc::VertexAttribute::getSizeInBytes() {
    switch (type) {
        case GL_FLOAT:
            return 4 * numComponents;
        case GL_UNSIGNED_BYTE:
        case GL_BYTE:
            return numComponents;
        case GL_UNSIGNED_SHORT:
        case GL_SHORT:
            return 2 * numComponents;
        default:
            throw std::invalid_argument("Type not supported");
    }
}


int bitCount(int i)
{
    // Algo from : http://aggregate.ee.engr.uky.edu/MAGIC/#Population%20Count%20(ones%20Count)
    i -= ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    i = (((i >> 4) + i) & 0x0F0F0F0F);
    i += (i >> 8);
    i += (i >> 16);
    return (i & 0x0000003F);
}

int numberOfTrailingZeros(int i)
{
    return bitCount((i & -i) - 1);
}


arc::VertexAttribute::VertexAttribute(int usage, int numComponents, int type, bool normalized, std::string aliass, int unit) {
    this->usage = usage;
    this->numComponents = numComponents;
    this->type = type;
    this->normalized = normalized;
    this->aliass = aliass;
    this->unit = unit;
    this->offset = 0;
    this->_usageIndex = numberOfTrailingZeros(usage);
}

arc::VertexAttribute::VertexAttribute(int usage, int numComponents, std::string aliass, int unit)
{
    this->usage = usage;
    this->numComponents = numComponents;
    this->type = usage == arc::VertexUsage::ColorPacked ? GL_UNSIGNED_BYTE : GL_FLOAT;
    this->normalized = usage == arc::VertexUsage::ColorPacked;
    this->aliass = aliass;
    this->unit = unit;
    this->offset = 0;
    this->_usageIndex = numberOfTrailingZeros(usage);
}


arc::VertexAttribute arc::VertexAttribute::position() {
    return {VertexUsage::Position, 3, "a_position"};
}

arc::VertexAttribute arc::VertexAttribute::normal() {
    return {VertexUsage::Normal, 3, "a_normal"};
}

arc::VertexAttribute arc::VertexAttribute::colorPacked() {
    return {VertexUsage::ColorPacked, 4, "a_color"};
}

arc::VertexAttribute arc::VertexAttribute::colorUnpacked() {
    return {VertexUsage::ColorUnpacked, 4, "a_color"};
}

arc::VertexAttribute arc::VertexAttribute::tangent() {
    return {VertexUsage::Tangent, 3, "a_tangent"};
}

arc::VertexAttribute arc::VertexAttribute::binormal() {
    return {VertexUsage::BiNormal, 3, "a_binormal"};
}


arc::VertexAttribute arc::VertexAttribute::texCoords(int unit) {
    return {VertexUsage::TextureCoordinates, 2, std::string("a_texCoord").append(std::to_string(unit)), unit};
}


arc::VertexAttribute arc::VertexAttribute::boneWeight(int unit) {
    return {VertexUsage::BoneWeight, 2, std::string("a_boneWeight").append(std::to_string(unit)), unit};
}
