#pragma once


#include  <string>
#include  <vector>
#include "../math/Mat4.h"
#include "../Color.h"
#include "../math/Vec2.h"
#include "Mesh.h"

namespace arc
{
    class ImmediateModeRenderer
    {
    public:
        ImmediateModeRenderer(int maxVertices, bool hasNormals, bool hasColors, int numTexCoords);
        ~ImmediateModeRenderer();
        void begin(const Mat4& projModelView, int primitiveType);
        void flush();
        void color(Color& color);
        void color(float colorBits);
        void texCoord(const Vec2& uv);
        void normal(const Vec3& normal);
        void vertex(const Vec3& vertex);
        void vertex(float x, float y, float z);
        void end();
        int getNumVertices();
        int getMaxVertices();

    private:
        int primitiveType{};
        int vertexIdx{};
        int numSetTexCoords{};
        int maxVertices{};
        int numVertices{};

        Mesh* mesh = nullptr;
        ShaderProgram* shader = nullptr;
        bool ownsShader{};
        int numTexCoords{};
        int vertexSize{};
        int normalOffset{};
        int colorOffset{};
        int texCoordOffset{};
        Mat4 projModelView = Mat4::identity();
        std::vector<float> vertices{};
        std::vector<std::string> shaderUniformNames{};
    };

}

