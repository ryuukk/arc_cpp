#pragma once


#include "Mesh.h"
#include "Texture2D.h"
#include "../Color.h"
#include "../math/Vec2.h"
#include "TextureRegion.h"

namespace arc
{
    class SpriteBatch
    {
    public:
        SpriteBatch(uint32_t size = 1000, ShaderProgram* defaultShader = nullptr);
        ~SpriteBatch()
        {
            delete _mesh;
            delete _shader;
        }

        void setProjectionMatrix(const Mat4& projection);
        void begin();
        void end();
        void flush();

        void draw(Texture2D* texture, Vec2 position, Vec2 size);
        void draw(Texture2D* texture, std::vector<float>& v, uint32_t offset, uint32_t count);

        void draw(TextureRegion* region, float x, float y, float width, float height);

        void setColor(const Color& color);

        bool isBlendingEnabled();

        const Mat4& getTransformMatrix();
        void setTransformMatrix(const Mat4& transform);

        Texture2D* getWhitePixel();

        uint32_t renderCalls = 0;
        uint32_t totalRenderCalls = 0;
        uint32_t maxSpritesInBatch = 0;
    private:
        void setupMatrices();
        void switchTexture(Texture2D* texture);

        Texture2D* _whitePixel = nullptr;
        Mesh* _mesh;
        std::vector<float> _vertices;
        uint32_t _idx{};
        Texture2D* _lastTexture = nullptr;
        float_t _invTexWidth{};
        float_t _invTexHeight{};

        bool _drawing{};

        Mat4 _transformMatrix = Mat4::identity();
        Mat4 _projectionMatrix = Mat4::identity();

        bool _blendingDisabled{};

        uint32_t _blendSrcFunc = GL_SRC_ALPHA;
        uint32_t _blendDstFunc = GL_ONE_MINUS_SRC_ALPHA;
        uint32_t _blendSrcFuncAlpha = GL_SRC_ALPHA;
        uint32_t _blendDstFuncAlpha = GL_ONE_MINUS_SRC_ALPHA;

        ShaderProgram* _shader = nullptr;
        ShaderProgram* _customShader = nullptr;
        bool _ownsShader{};

        Color _color = Color::WHITE;

        std::string _vs = R"(
#version 330

in vec4 a_position;
in vec4 a_color;
in vec2 a_texCoord0;

uniform mat4 u_proj;
uniform mat4 u_trans;

out vec4 v_color;
out vec2 v_texCoords;

void main() {
    v_color = a_color;
    v_color.a = v_color.a * (255.0/254.0);
    v_texCoords = a_texCoord0;
    gl_Position = u_proj * u_trans * a_position;
}
)";
        std::string _fs = R"(
#version 330

in vec4 v_color;
in vec2 v_texCoords;

uniform sampler2D u_texture;

out vec4 f_color;

void main() {
        f_color = v_color * texture(u_texture, v_texCoords);
}
)";
    };
}


