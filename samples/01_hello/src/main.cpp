#include <fstream>
#include <string.h>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

class MyGame : public arc::IApp
{
    void create() override {
    }

    void update(float dt) override {

    }

    void render(float dt) override {

    }

    void resize(int width, int height) override {

    }

    void dispose() override {
    }

};

int main( int argc, char** argv )
{
    auto config = arc::Configuration();
    config.windowTitle = "Sample 01 - Hello";
    auto myGame = new MyGame();
    auto engine = new arc::Engine(myGame, config);
    engine->run();

    return 0;
}