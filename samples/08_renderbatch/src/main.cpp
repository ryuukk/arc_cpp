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
#include <gfx/rendering/RenderableBatch.h>
#include <gfx/animation/AnimationController.h>


class Entity
{
public:
    ~Entity() {
        delete controller;
        delete instance;
    }

    arc::ModelInstance* instance = nullptr;
    arc::AnimationController* controller = nullptr;

    arc::Vec3 position = {0, 0, 0};
    arc::Mat4 _transform = arc::Mat4::identity();
    float _a = 5.0f;


    void update(const float dt) {
        _a += dt;
        _transform.set(position, arc::Quat::fromAxis({0, 1, 0}, _a));
        instance->transform = _transform;

        if(controller)
            controller->update(dt);
    }

    void render(arc::RenderableBatch* batch, arc::Environement* environement = nullptr) {
        batch->render(instance, environement);
    }
};

class MyGame : public arc::IApp
{
    arc::PerspectiveCamera* _cam;
    arc::Model* _model;
    arc::RenderableBatch* _batch;
    std::vector<Entity*> _entities{};


    void create() override {

        _cam = new arc::PerspectiveCamera(67, arc::Core::graphics->getWidth(), arc::Core::graphics->getHeight());
        //_cam->position = arc::Vec3(0, 10, 5) * 6.0f;
        //_cam->lookAt(0, 0, 0);
        _cam->update();

        auto modelData = arc::ModelData::load("data/character_male_0.g3dj");
        //auto modelData = arc::ModelData::load("data/tree_small_0.g3dj");
        _model = new arc::Model(modelData);
        std::string vs = arc::file::readFile("data/default.vert");
        std::string fs = arc::file::readFile("data/default.frag");

        _batch = new arc::RenderableBatch(new arc::DefaultShaderProvider(vs, fs));

        auto s = 16;
        for (int i = -s; i < s; ++i) {
            for (int j = -s; j < s; ++j) {

                auto* entity = new Entity;
                entity->position = {(float) i * 2, 0, (float) j * 2};

                entity->instance = new arc::ModelInstance(*_model);

                if(!entity->instance->animations.empty())
                {
                    entity->controller = new arc::AnimationController(*entity->instance);
                    entity->controller->animate("run_1h");
                }

                _entities.emplace_back(entity);
            }
        }

        printf("Loaded %d entities\n", _entities.size());

    }

    int fpsAcc = 0;
    int c = 0;
    float timer = 0.0f;
    void update(float dt) override {

        auto fps = arc::Core::graphics->fps();
        fpsAcc+=fps;
        timer += dt;
        c++;

        if (timer > 1.0f)
        {
            int f = fpsAcc / c;
            printf("FPS: %d  AVG: %d\n", fps, f);

            c = 0;
            fpsAcc = 0;
            timer = 0;
        }
        _cam->update();

        for (auto& entity : _entities)
            entity->update(dt);
    }

    void render(float dt) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glEnable(GL_DEPTH_TEST);

        _batch->begin(_cam);

        for (auto& entity : _entities)
            entity->render(_batch, nullptr);

        _batch->end();
    }

    void resize(int width, int height) override {

    }

    void dispose() override {
        delete _cam;
        delete _model;
        delete _batch;

        for (auto& entity : _entities)
            delete entity;
    }

};

int main(int argc, char** argv) {
    auto config = arc::Configuration();
    config.windowTitle = "Sample 08 - Render Batch";
    config.vsync = true;
    auto myGame = new MyGame();
    auto engine = new arc::Engine(myGame, config);
    engine->run();

    return 0;
}