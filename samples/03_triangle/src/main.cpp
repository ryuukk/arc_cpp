#include <fstream>
#include <string.h>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

#include <ShaderProgram.h>

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

    arc::ShaderProgram *shader;

    void create() override {
        shader = new arc::ShaderProgram(vs, fs);
    }

    void update(float dt) override {

    }

    void render(float dt) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    }

    void resize(int width, int height) override {

    }

    void dispose() override {
        delete shader;
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