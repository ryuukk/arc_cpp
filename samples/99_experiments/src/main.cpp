#include <fstream>
#include <string.h>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

#include <gfx/Model.h>
#include <gfx/ModelInstance.h>
#include <gfx/data/ModelData.h>
#include <utils/FileUtils.h>


class MyGame : public arc::IApp
{
    arc::Model* _modelA;
    arc::Model* _modelB;


    void create() override {


        auto dataA = arc::ModelData::load("data/character_male_0.g3dj");
        _modelA = new arc::Model(dataA);
        _modelB = new arc::Model(dataA);
        delete _modelA;
        _modelA = new arc::Model(dataA);
        delete _modelA;
        
        delete _modelB;
    }

    void update(float dt) override {
    }

    void render(float dt) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glEnable(GL_DEPTH_TEST);

    }

    void resize(int width, int height) override {

    }

    void dispose() override {

    }

};

int main(int argc, char** argv) {
    auto config = arc::Configuration();
    config.windowTitle = "Sample 99 - Experiments";
    auto myGame = new MyGame();
    auto engine = new arc::Engine(myGame, config);
    engine->run();

    return 0;
}