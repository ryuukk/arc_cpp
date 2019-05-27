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
#include <utils/CameraController.h>

class Entity
{
public:
    ~Entity() {
        delete controller;
        delete instance;
    }

    int id{};
    arc::ModelInstance* instance = nullptr;
    arc::AnimationController* controller = nullptr;

    arc::Vec3 position = {0, 0, 0};
    arc::Vec3 scale = {1, 1, 1};
    arc::Mat4 _transform = arc::Mat4::identity();
    float _a = 5.0f;


    void update(const float dt) {
        _a += dt;
        _transform.set(position, arc::Quat::fromAxis({0, 1, 0}, _a), scale);
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
    arc::CameraController* _camController;
    arc::PerspectiveCamera* _cam;
    arc::Model* _modelA;
    arc::Model* _modelB;
    arc::Model* _modelC;
    arc::RenderableBatch* _batch;
    std::vector<Entity*> _entities{};

    void create() override {

        _cam = new arc::PerspectiveCamera(67, arc::Core::graphics->getWidth(), arc::Core::graphics->getHeight());
        _cam->position = arc::Vec3(0, 0, 10);
        _cam->update();

        _camController = new arc::CameraController(_cam);
        arc::Core::input->setInputProcessor(_camController);

        auto modelDataA = arc::ModelData::load("data/models/knight.g3dj");
        auto modelDataB = arc::ModelData::load("data/models/character_male_0.g3dj");
        auto modelDataC = arc::ModelData::load("data/models/tree_small_0.g3dj");
        _modelA = new arc::Model(modelDataA);
        _modelB = new arc::Model(modelDataB);
        _modelC = new arc::Model(modelDataC);
        std::string vs = arc::file::readFile("data/shaders/default.vert");
        std::string fs = arc::file::readFile("data/shaders/default.frag");

        _batch = new arc::RenderableBatch(new arc::DefaultShaderProvider(vs, fs));

        auto one = false;
        auto enableAnim = true;
        auto size = 5;
        auto pad = 3;
        if(one)
        {
            auto* entity = new Entity;
            entity->position = {0, 0, 0};

            entity->instance = new arc::ModelInstance(*_modelA);

            if(!entity->instance->animations.empty())
            {
                entity->controller = new arc::AnimationController(*entity->instance);
                entity->controller->animate("run_1h");
            }

            _entities.emplace_back(entity);
        }
        else
        {
            printf("Loading entities..\n");
            int id = 0;
            for (int i = -size; i < size; i++)
            {
                for (int j = -size; j < size; j++)
                {
                    auto a = id % 3;

                    if(a == 0)
                    {
                        auto* entity = new Entity;
                        entity->id = ++id;
                        entity->position = {(float) i * 2, 0, (float) j * 2};
                        entity->scale = {0.2f, 0.2f, 0.2f};

                        entity->instance = new arc::ModelInstance(*_modelA);

                        if(!entity->instance->animations.empty())
                        {
                            entity->controller = new arc::AnimationController(*entity->instance);
                            entity->controller->animate("Attack");
                        }

                        _entities.emplace_back(entity);
                    }
                    else if (a == 1)
                    {
                        auto* entity = new Entity;
                        entity->id = ++id;
                        entity->position = {(float) i * 2, 0, (float) j * 2};

                        entity->instance = new arc::ModelInstance(*_modelB);

                        if(!entity->instance->animations.empty())
                        {
                            entity->controller = new arc::AnimationController(*entity->instance);
                            entity->controller->animate("run_1h");
                        }

                        _entities.emplace_back(entity);
                    }
                    else
                    {
                        auto* entity = new Entity;
                        entity->id = ++id;
                        entity->position = {(float) i * 2, 0, (float) j * 2};
                        entity->instance = new arc::ModelInstance(*_modelC);

                        _entities.emplace_back(entity);
                    }
                }
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
        _camController->update(dt);
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
        delete _modelA;
        delete _modelB;
        delete _modelC;
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