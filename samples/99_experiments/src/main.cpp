#include <fstream>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

#define STB_IMAGE_IMPLEMENTATION
#include <utils/stb_image.h>

class MyGame : public arc::IApp
{
    void create() override {
        int x,y,n;
        unsigned char *data = stbi_load("data/tree_diffuse.png", &x, &y, &n, 0);

        printf("Image: %d:%d %d\n", x, y, n);
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