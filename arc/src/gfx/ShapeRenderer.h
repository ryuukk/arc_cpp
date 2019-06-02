#pragma once

#include <glad/glad.h>
#include "ImmediateModeRenderer.h"
#include "../Color.h"
#include "../math/Mat4.h"
#include "../math/Vec2.h"

namespace arc
{
    enum class ShapeType
    {
        None,
        Point = GL_POINTS,
        Line = GL_LINES,
        Filled = GL_TRIANGLES
    };

    class ShapeRenderer
    {
    public:
        ShapeRenderer(int maxVertices = 5000);
        ~ShapeRenderer();

        void setColor(const Color& color);
        void updateMatrices();
        void setProjectionMatrix(const Mat4& matrix);
        const Mat4& getProjectionMatrix();
        void setTransformMatrix(const Mat4& matrix);
        const Mat4& getTransformMatrix();
        void identity();
        void translate(const Vec3& pos);
        void rotate(const Vec3& axis, float degrees);
        void scale(const Vec3& scale);
        void begin(ShapeType type);
        void point(const Vec3& pos);
        void line(const Vec3& v0, const Vec3& v1);
        void line(const Vec3& v0, const Vec3& v1, const Color& color);
        void rect(const Vec2& pos, const Vec2& size);
        void rect(const Vec2& pos, const Vec2& size, const Color& color);
        void rect(const Vec2& pos, const Vec2& origin, const Vec2& size);
        void rect(const Vec2& pos, const Vec2& origin, const Vec2& size, const Color& color);
        void rectLine(const Vec2& a, const Vec2& b, float width);
        void box(const Vec3& pos, const Vec3& size);
        void x(const Vec2& pos, float size);
        void circle(const Vec2& pos, float radius);
        void circle(const Vec2& pos, float radius, int segments);

        void check(ShapeType preferred, ShapeType other, int newVertices);
        void end();
        void flush();

        bool isDrawing();
    private:
        ImmediateModeRenderer* _renderer = nullptr;
        bool _matrixDirty = false;
        Mat4 _projectionMatrix = Mat4::identity();
        Mat4 _transformMatrix = Mat4::identity();
        Mat4 _combinedMatrix = Mat4::identity();
        Color _color = Color::WHITE;
        ShapeType _shapeType = ShapeType::Point;
        bool _autoShapeType = false;
        float _defaultRectLineWidth = 0.75f;
    };

}

