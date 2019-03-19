#include <fstream>
#include <string.h>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

#include <gfx/ShaderProgram.h>
#include <gfx/Mesh.h>

class MyGame : public arc::IApp
{
    std::string vs = R"(
#version 330

in vec3 a_position;


void main()
{
    gl_Position = vec4(a_position, 1.0);
}
)";

    std::string fs = R"(
#version 330

out vec4 f_color;

void main()
{
	f_color = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

    arc::ShaderProgram *_shader;
    arc::Mesh *_mesh;

    void create() override {

        auto attribute = arc::VertexAttribute(arc::VertexUsage::Position, 3, "a_position");
		_mesh = new arc::Mesh(true, 3, 3, new arc::VertexAttributes({ attribute }));

        std::vector<float> vertices = {
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                0.0f,  1.0f, 0.0f,
        };

        std::vector<short> indices = {
                0, 1, 2,
        };

        _mesh->setVertices(vertices);
        _mesh->setIndices(indices);


        _shader = new arc::ShaderProgram(vs, fs);

        printf("Shader Log: %s\n", _shader->log.c_str());
    }

    void update(float dt) override {

    }

    void render(float dt) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        _shader->begin();

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
    config.windowTitle = "Sample 03 - Triangle";
    auto myGame = new MyGame();
    auto engine = new arc::Engine(myGame, config);
    engine->run();

    return 0;
}