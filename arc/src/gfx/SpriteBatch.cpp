#include <cassert>
#include "../Core.h"
#include "SpriteBatch.h"

arc::SpriteBatch::SpriteBatch(uint32_t size, arc::ShaderProgram* defaultShader) {
    assert(size < 8191 && "spritebatch too big");

    auto ap = VertexAttribute(VertexUsage::Position, 2, "a_position");
    auto ac = VertexAttribute(VertexUsage::ColorPacked, 4, "a_color");
    auto at = VertexAttribute(VertexUsage::TextureCoordinates, 2, "a_texCoord0");

    _mesh = new Mesh(false, size * 4, size * 6, new VertexAttributes({ap, ac, at}));

    _projectionMatrix = Mat4::createOrthographicOffCenter(0.0, 0.0, Core::graphics->getWidth(), Core::graphics->getHeight());

    _vertices.resize(size * 20);

    for (int i = 0; i < _vertices.size(); i++)
        _vertices[i] = 0.0f;

    int len = size * 6;
    std::vector<short> indices;
    indices.resize(len);
    short j = 0;
    for (int i = 0; i < len; i += 6, j += 4) {
        indices[i] = j;
        indices[i + 1] = (short) (j + 1);
        indices[i + 2] = (short) (j + 2);
        indices[i + 3] = (short) (j + 2);
        indices[i + 4] = (short) (j + 3);
        indices[i + 5] = (short) j;
    }
    _mesh->setIndices(indices);

    if (defaultShader == nullptr) {
        _shader = new ShaderProgram(_vs, _fs);
        assert(_shader->isCompiled() && _shader->log.c_str());
        _ownsShader = true;
    } else
        _shader = defaultShader;

    // create white pixel
    {

        unsigned char data[4] = {255,255,255,255};
        _whitePixel = new Texture2D();
        _whitePixel->setData(data, 1, 1);
    }
}

void arc::SpriteBatch::setProjectionMatrix(const arc::Mat4& projection) {
    assert(!_drawing && "must call end");
    _projectionMatrix = projection;
}

void arc::SpriteBatch::begin() {
    assert(!_drawing && "must call end");
    renderCalls = 0;

    glDepthMask(GL_FALSE);
    if (_customShader != nullptr)
        _customShader->begin();
    else
        _shader->begin();

    setupMatrices();
    _drawing = true;
}

void arc::SpriteBatch::end() {
    assert(_drawing && "must call begin");
    if (_idx > 0)
        flush();
    _lastTexture = nullptr;
    _drawing = false;

    glDepthMask(GL_TRUE);
    if (isBlendingEnabled())
        glDisable(GL_BLEND);

    if (_customShader != nullptr)
        _customShader->end();
    else
        _shader->end();
}

void arc::SpriteBatch::flush() {

    if (_idx == 0)
        return;

    renderCalls++;
    totalRenderCalls++;

    int spritesInBatch = _idx / 20;
    if (spritesInBatch > maxSpritesInBatch)
        maxSpritesInBatch = spritesInBatch;
    int count = spritesInBatch * 6;

    _lastTexture->bind();
    _mesh->setVertices(_vertices, 0, _idx);

    if (_blendingDisabled) {
        glDisable(GL_BLEND);
    } else {
        glEnable(GL_BLEND);
        if (_blendSrcFunc != -1)
            glBlendFuncSeparate(_blendSrcFunc, _blendDstFunc, _blendSrcFuncAlpha, _blendDstFuncAlpha);
    }

    _mesh->render(_customShader != nullptr ? _customShader : _shader, GL_TRIANGLES, 0, count, true);

    _idx = 0;
}

void arc::SpriteBatch::draw(arc::Texture2D* texture, arc::Vec2 position, arc::Vec2 size) {

    assert(_drawing && "must call begin");

    if (texture != _lastTexture)
        switchTexture(texture);
    else if (_idx == _vertices.size()) //
        flush();

    float fx2 = position.x + size.x;
    float fy2 = position.y + size.y;
    float u = 0;
    float v = 1;
    float u2 = 1;
    float v2 = 0;

    float color = _color.toFloatBits();

    int idx = _idx;
    _vertices[idx] = position.x;
    _vertices[idx + 1] = position.y;
    _vertices[idx + 2] = color;
    _vertices[idx + 3] = u;
    _vertices[idx + 4] = v;

    _vertices[idx + 5] = position.x;
    _vertices[idx + 6] = fy2;
    _vertices[idx + 7] = color;
    _vertices[idx + 8] = u;
    _vertices[idx + 9] = v2;

    _vertices[idx + 10] = fx2;
    _vertices[idx + 11] = fy2;
    _vertices[idx + 12] = color;
    _vertices[idx + 13] = u2;
    _vertices[idx + 14] = v2;

    _vertices[idx + 15] = fx2;
    _vertices[idx + 16] = position.y;
    _vertices[idx + 17] = color;
    _vertices[idx + 18] = u2;
    _vertices[idx + 19] = v;

    _idx = idx + 20;
}

void arc::SpriteBatch::draw(arc::Texture2D* texture, std::vector<float>& v, uint32_t offset, uint32_t count) {
    int verticesLength = _vertices.size();
    int remainingVertices = verticesLength;
    if (texture != _lastTexture)
        switchTexture(texture);
    else
    {
        remainingVertices -= _idx;
        if (remainingVertices == 0)
        {
            flush();
            remainingVertices = verticesLength;
        }
    }

    int copyCount = Mathf::min(remainingVertices, count);

    _vertices.reserve(_vertices.size() + copyCount);
    std::copy(v.begin() + offset, v.end(), _vertices.begin() + _idx);

    _idx += copyCount;
    count -= copyCount;
    while(count > 0)
    {
        offset += copyCount;
        flush();
        copyCount = Mathf::min(verticesLength, count);

        _vertices.reserve(_vertices.size() + copyCount);
        std::copy(v.begin() + offset, v.end(), _vertices.begin());

        _idx += copyCount;
        count -= copyCount;
    }
}

void arc::SpriteBatch::draw(arc::TextureRegion* region, float x, float y, float width, float height) {
    assert(_drawing && "must call begin");

    auto texture = region->texture;
    if (texture != _lastTexture) {
        switchTexture(texture);
    } else if (_idx == _vertices.size())
        flush();

    float fx2 = x + width;
    float fy2 = y + height;
    float u = region->u;
    float v = region->v2;
    float u2 = region->u2;
    float v2 = region->v;

    float color = _color.toFloatBits();
    int idx = _idx;
    _vertices[idx] = x;
    _vertices[idx + 1] = y;
    _vertices[idx + 2] = color;
    _vertices[idx + 3] = u;
    _vertices[idx + 4] = v;

    _vertices[idx + 5] = x;
    _vertices[idx + 6] = fy2;
    _vertices[idx + 7] = color;
    _vertices[idx + 8] = u;
    _vertices[idx + 9] = v2;

    _vertices[idx + 10] = fx2;
    _vertices[idx + 11] = fy2;
    _vertices[idx + 12] = color;
    _vertices[idx + 13] = u2;
    _vertices[idx + 14] = v2;

    _vertices[idx + 15] = fx2;
    _vertices[idx + 16] = y;
    _vertices[idx + 17] = color;
    _vertices[idx + 18] = u2;
    _vertices[idx + 19] = v;
    _idx = idx + 20;
}

bool arc::SpriteBatch::isBlendingEnabled() {
    return !_blendingDisabled;
}

void arc::SpriteBatch::setupMatrices() {
    if (_customShader != nullptr) {
        _customShader->setUniformMat4("u_proj", _projectionMatrix);
        _customShader->setUniformMat4("u_trans", _transformMatrix);
        _customShader->setUniformi("u_texture", 0);
    } else {
        _shader->setUniformMat4("u_proj", _projectionMatrix);
        _shader->setUniformMat4("u_trans", _transformMatrix);
        _shader->setUniformi("u_texture", 0);
    }
}

void arc::SpriteBatch::switchTexture(arc::Texture2D* texture) {
    flush();
    _lastTexture = texture;
    _invTexWidth = 1.0f / texture->getWidth();
    _invTexHeight = 1.0f / texture->getHeight();
}

const arc::Mat4& arc::SpriteBatch::getTransformMatrix() {
    return _transformMatrix;
}

void arc::SpriteBatch::setTransformMatrix(const arc::Mat4& transform) {
    if (_drawing) flush();
    _transformMatrix = transform;
    if (_drawing) setupMatrices();
}

void arc::SpriteBatch::setColor(const arc::Color& color) {
    _color = color;
}

arc::Texture2D* arc::SpriteBatch::getWhitePixel() {
    return _whitePixel;
}
