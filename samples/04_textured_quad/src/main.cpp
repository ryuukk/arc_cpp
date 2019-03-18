#include <fstream>
#include <string.h>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

#include <gfx/ShaderProgram.h>
#include <gfx/Mesh.h>
#include <gfx/Texture2D.h>

class MyGame : public arc::IApp
{
    std::string vs = R"(
#version 330

in vec4 a_position;
in vec2 a_texCoord0;

out vec2 v_texCoords;

void main() {
    v_texCoords = a_texCoord0;
    gl_Position = a_position;
}
)";

    std::string fs = R"(
#version 330

in vec2 v_texCoords;

uniform sampler2D u_texture;

out vec4 f_color;

void main() {
        vec3 color = texture2D(u_texture, v_texCoords).rgb;
        f_color = vec4(color, 1.0);
}
)";

    arc::ShaderProgram* _shader;
    arc::Mesh* _mesh;
    arc::Texture2D* _texture;

    void create() override {

        auto ap = arc::VertexAttribute(arc::VertexUsage::Position, 3, "a_position");
        auto at = arc::VertexAttribute(arc::VertexUsage::TextureCoordinates, 2, "a_texCoord0");
        _mesh = new arc::Mesh(true, 6, 0, new arc::VertexAttributes({ap, at}));

        std::vector<float> vertices = {
                -1.0f, 1.0f, 0.0f,     0.0f, 0.0f,
                1.0f, 1.0f, 0.0f,      1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,     1.0f, 1.0f,

                -1.0f, 1.0f, 0.0f,     0.0f, 0.0f,
                1.0f, -1.0f, 0.0f,      1.0f, 1.0f,
                -1.0f,  -1.0f, 0.0f,     0.0f, 1.0f,
        };
        _mesh->setVertices(vertices);

        _texture = arc::Texture2D::loadFromFile("data/bg_stars.png");

        _shader = new arc::ShaderProgram(vs, fs);

        printf("Shader Log: %s\n", _shader->log.c_str());
    }

    void update(float dt) override {

    }

    void render(float dt) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        _shader->begin();

        _texture->bind();
        _shader->setUniformi("u_texture", 0);

        _mesh->render(_shader, GL_TRIANGLES);

        _shader->end();
    }

    void resize(int width, int height) override {

    }

    void dispose() override {
        delete _mesh;
        delete _shader;
    }

};

int main(int argc, char **argv) {
    auto config = arc::Configuration();
    config.windowTitle = "Sample 04 - Textured Quad";
    auto myGame = new MyGame();
    auto engine = new arc::Engine(myGame, config);
    engine->run();

    return 0;
}