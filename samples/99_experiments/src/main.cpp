#include <fstream>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

#define STB_IMAGE_IMPLEMENTATION
#include <utils/stb_image.h>
#include <gfx/material/Attribute.h>

class MyGame : public arc::IApp
{
    void create() override {

        auto a = arc::DiffuseTextureAttribute::stype;
        auto b = arc::CullfaceAttribute::stype;
        auto c = arc::DepthTestAttribute::stype;

        printf("Type: %d\n", a);
        printf("Type: %d\n", b);
        printf("Type: %d\n", c);
    }

    void update(float dt) override {
    }

    void render(float dt) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

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