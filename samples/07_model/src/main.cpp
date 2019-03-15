#include <fstream>
#include <string.h>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

#include <gfx/ShaderProgram.h>
#include <gfx/Mesh.h>
#include <gfx/Camera.h>
#include <gfx/Model.h>
#include <gfx/ModelInstance.h>
#include <gfx/data/ModelData.h>

class MyGame : public arc::IApp
{
    std::string vs = R"(
#version 330

in vec3 a_position;
in vec3 a_normal;

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_world;

out vec3 v_normal;

void main()
{
    gl_Position = u_proj * u_view * u_world * vec4(a_position, 1.0);

    v_normal = a_normal;
}
)";

    std::string fs = R"(
#version 330

in vec3 v_normal;

out vec4 f_color;

void main()
{
	f_color = vec4(1.0, 1.0, 1.0, 1.0) * vec4(v_normal, 1.0);
}
)";

    arc::ShaderProgram* _shader;
    arc::PerspectiveCamera* _cam;
    arc::Model* _model;
    arc::ModelInstance* _instance;
    arc::Mat4 _transform;
    float _a = 0.0f;

    void create() override {

        _shader = new arc::ShaderProgram(vs, fs);

        _cam = new arc::PerspectiveCamera(67, arc::Core::graphics->getWidth(), arc::Core::graphics->getHeight());
        _cam->position = arc::Vec3(0, 10, 5) * 0.5f;
        _cam->lookAt(0, 0, 0);
        _cam->update();

        printf("Shader Log: %s\n", _shader->log.c_str());


        auto modelData = arc::ModelData::load("data/character_male_0.g3dj");
        _model = new arc::Model(modelData);
    }

    void update(float dt) override {

        _a += 0.016f;
        _transform.set({0, 0, 0}, arc::Quat::fromAxis({0.5, 1, 0.5}, _a));
        _cam->update();
    }

    void render(float dt) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glEnable(GL_DEPTH_TEST);

        _shader->begin();
        _shader->setUniformMat4("u_proj", _cam->projection);
        _shader->setUniformMat4("u_view", _cam->view);
        _shader->setUniformMat4("u_world", _transform);


        _shader->end();
    }

    void resize(int width, int height) override {

    }

    void dispose() override {
        delete _shader;
    }

};

int main(int argc, char** argv) {
    auto config = arc::Configuration();
    config.windowTitle = "Sample 06 - Cube";
    auto myGame = new MyGame();
    auto engine = new arc::Engine(myGame, config);
    engine->run();

    return 0;
}