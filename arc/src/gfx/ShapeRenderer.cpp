#include "ShapeRenderer.h"
#include "../Core.h"

arc::ShapeRenderer::ShapeRenderer(int maxVertices) {
    _renderer = new ImmediateModeRenderer(maxVertices, false, true, 0);
    _projectionMatrix = Mat4::createOrthographic(0,0, Core::graphics->getWidth(), Core::graphics->getHeight(), 0, 1);
    _matrixDirty = true;
}

arc::ShapeRenderer::~ShapeRenderer() {
    delete _renderer;
}

void arc::ShapeRenderer::setColor(const arc::Color& color) {
    _color = color;
}

void arc::ShapeRenderer::updateMatrices() {
    _matrixDirty = true;
}

void arc::ShapeRenderer::setProjectionMatrix(const arc::Mat4& matrix) {
    _projectionMatrix = matrix;
    _matrixDirty = true;
}

const arc::Mat4& arc::ShapeRenderer::getProjectionMatrix() {
    return _projectionMatrix;
}

void arc::ShapeRenderer::setTransformMatrix(const arc::Mat4& matrix) {
    _transformMatrix = matrix;
    _matrixDirty = true;
}

const arc::Mat4& arc::ShapeRenderer::getTransformMatrix() {
    return _transformMatrix;
}

void arc::ShapeRenderer::identity() {
    _transformMatrix.idt();
    _matrixDirty = true;
}

void arc::ShapeRenderer::translate(const arc::Vec3& pos) {
    _transformMatrix = _transformMatrix * Mat4::createTranslation(pos);
    _matrixDirty = true;
}

void arc::ShapeRenderer::rotate(const arc::Vec3& axis, float degrees) {
    _transformMatrix.rotate(axis, degrees);
    _matrixDirty = true;
}

void arc::ShapeRenderer::scale(const arc::Vec3& scale) {
    _transformMatrix.scale(scale);
    _matrixDirty = true;
}

void arc::ShapeRenderer::begin(arc::ShapeType type) {
    _shapeType = type;
    if(_matrixDirty)
    {
        _combinedMatrix = _projectionMatrix * _transformMatrix;
        _matrixDirty = false;
    }
    _renderer->begin(_combinedMatrix, (int)_shapeType); // todo: redo this
}

void arc::ShapeRenderer::point(const arc::Vec3& pos) {

}

void arc::ShapeRenderer::line(const arc::Vec3& v0, const arc::Vec3& v1) {

}

void arc::ShapeRenderer::line(const arc::Vec3& v0, const arc::Vec3& v1, const arc::Color& color) {

}

void arc::ShapeRenderer::rect(const arc::Vec2& pos, const arc::Vec2& size) {

}

void arc::ShapeRenderer::rect(const arc::Vec2& pos, const arc::Vec2& size, const arc::Color& color) {

}

void arc::ShapeRenderer::rect(const arc::Vec2& pos, const arc::Vec2& origin, const arc::Vec2& size) {

}

void arc::ShapeRenderer::rect(const arc::Vec2& pos, const arc::Vec2& origin, const arc::Vec2& size,
                              const arc::Color& color) {

}

void arc::ShapeRenderer::rectLine(const arc::Vec2& a, const arc::Vec2& b, float width) {

}

void arc::ShapeRenderer::box(const arc::Vec3& pos, const arc::Vec3& size) {
    float depth = -size.z;
    float x = pos.x;
    float y = pos.y;
    float z = pos.z;
    float width = size.x;
    float height = size.y;
    float colorBits = _color.toFloatBits();

    if (_shapeType == ShapeType::Line) {
        check(ShapeType::Line, ShapeType::Filled, 24);

        _renderer->color(colorBits);
        _renderer->vertex(x, y, z);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y, z);

        _renderer->color(colorBits);
        _renderer->vertex(x + width, y, z);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y, z + depth);

        _renderer->color(colorBits);
        _renderer->vertex(x + width, y, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x, y, z + depth);

        _renderer->color(colorBits);
        _renderer->vertex(x, y, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x, y, z);

        _renderer->color(colorBits);
        _renderer->vertex(x, y, z);
        _renderer->color(colorBits);
        _renderer->vertex(x, y + height, z);

        _renderer->color(colorBits);
        _renderer->vertex(x, y + height, z);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y + height, z);

        _renderer->color(colorBits);
        _renderer->vertex(x + width, y + height, z);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y + height, z + depth);

        _renderer->color(colorBits);
        _renderer->vertex(x + width, y + height, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x, y + height, z + depth);

        _renderer->color(colorBits);
        _renderer->vertex(x, y + height, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x, y + height, z);

        _renderer->color(colorBits);
        _renderer->vertex(x + width, y, z);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y + height, z);

        _renderer->color(colorBits);
        _renderer->vertex(x + width, y, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y + height, z + depth);

        _renderer->color(colorBits);
        _renderer->vertex(x, y, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x, y + height, z + depth);
    } else {
        check(ShapeType::Line, ShapeType::Filled, 36);

        // Front
        _renderer->color(colorBits);
        _renderer->vertex(x, y, z);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y, z);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y + height, z);

        _renderer->color(colorBits);
        _renderer->vertex(x, y, z);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y + height, z);
        _renderer->color(colorBits);
        _renderer->vertex(x, y + height, z);

        // Back
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x, y, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y + height, z + depth);

        _renderer->color(colorBits);
        _renderer->vertex(x + width, y + height, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x, y, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x, y + height, z + depth);

        // Left
        _renderer->color(colorBits);
        _renderer->vertex(x, y, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x, y, z);
        _renderer->color(colorBits);
        _renderer->vertex(x, y + height, z);

        _renderer->color(colorBits);
        _renderer->vertex(x, y, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x, y + height, z);
        _renderer->color(colorBits);
        _renderer->vertex(x, y + height, z + depth);

        // Right
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y, z);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y + height, z + depth);

        _renderer->color(colorBits);
        _renderer->vertex(x + width, y, z);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y + height, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y + height, z);

        // Top
        _renderer->color(colorBits);
        _renderer->vertex(x, y + height, z);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y + height, z);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y + height, z + depth);

        _renderer->color(colorBits);
        _renderer->vertex(x, y + height, z);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y + height, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x, y + height, z + depth);

        // Bottom
        _renderer->color(colorBits);
        _renderer->vertex(x, y, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y, z);

        _renderer->color(colorBits);
        _renderer->vertex(x, y, z + depth);
        _renderer->color(colorBits);
        _renderer->vertex(x + width, y, z);
        _renderer->color(colorBits);
        _renderer->vertex(x, y, z);
    }
}

void arc::ShapeRenderer::x(const arc::Vec2& pos, float size) {

}

void arc::ShapeRenderer::circle(const arc::Vec2& pos, float radius) {

}

void arc::ShapeRenderer::circle(const arc::Vec2& pos, float radius, int segments) {

}

void arc::ShapeRenderer::check(arc::ShapeType preferred, arc::ShapeType other, int newVertices) {
    if(_shapeType == ShapeType::None) throw std::runtime_error("begin must be called"); // todo: change to assert
    if(_shapeType != preferred && _shapeType != other)
    {
        if(_autoShapeType)
        {
            if(other == ShapeType::None) throw std::runtime_error("must call begin()");
            else throw std::runtime_error("must call begin() or begin(ShapeType)");
        }
        end();
        begin(preferred);
    } else if(_matrixDirty)
    {
        auto type = _shapeType;
        end();
        begin(type);
    } else if(_renderer->getMaxVertices() - _renderer->getNumVertices() < newVertices)
    {
        // not enough space
        auto type = _shapeType;
        end();
        begin(type);
    }
}

void arc::ShapeRenderer::end() {
    _renderer->end();
    _shapeType = ShapeType::None;
}

void arc::ShapeRenderer::flush() {
    auto type = _shapeType;
    if(type == ShapeType::None) return;
    end();
    begin(type);
}

bool arc::ShapeRenderer::isDrawing() {
    return _shapeType != ShapeType::None;
}
