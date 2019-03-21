#include <fstream>
#include <string.h>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

#include <gfx/rendering/DefaultShaderProvider.h>
#include <gfx/Camera.h>
#include <gfx/Model.h>
#include <gfx/ModelInstance.h>
#include <gfx/data/ModelData.h>
#include <utils/FileUtils.h>

class MyGame : public arc::IApp
{
    arc::PerspectiveCamera* _cam;
    arc::Model* _model;
    arc::ModelInstance* _instance;
    arc::Mat4 _transform;
    float _a = 5.0f;

    void create() override {

        _cam = new arc::PerspectiveCamera(67, arc::Core::graphics->getWidth(), arc::Core::graphics->getHeight());
        _cam->position = arc::Vec3(0, 10, 5) * 0.5f;
        _cam->lookAt(0, 0, 0);
        _cam->update();

        auto modelData = arc::ModelData::load("data/tree_small_0.g3dj");
        _model = new arc::Model(modelData);
        _instance = new arc::ModelInstance(*_model);

        std::string vs = arc::readFile("data/default.vert");
        std::string fs = arc::readFile("data/default.frag");

        auto* provider = new arc::DefaultShaderProvider(vs, fs);
    }

    void update(float dt) override {

        _a += dt;
        _transform.set({0, 0, 0}, arc::Quat::fromAxis({0, 1, 0}, _a));
        _cam->update();
    }

    void render(float dt) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glEnable(GL_DEPTH_TEST);

    }

    void resize(int width, int height) override {

    }

    void dispose() override {
        delete _cam;
        delete _model;
        delete _instance;
    }

};

int main(int argc, char** argv) {
    auto config = arc::Configuration();
    config.windowTitle = "Sample 08 - Render Batch";
    auto myGame = new MyGame();
    auto engine = new arc::Engine(myGame, config);
    engine->run();

    return 0;
}