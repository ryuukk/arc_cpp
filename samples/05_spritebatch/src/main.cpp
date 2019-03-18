#include <fstream>
#include <string.h>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

#include <gfx/Texture2D.h>
#include <gfx/SpriteBatch.h>
#include <gfx/Camera.h>

class MyGame : public arc::IApp
{
    arc::SpriteBatch* _batch;
    arc::Texture2D* _texture;
    arc::OrthographicCamera* _camera;

    void create() override {

        _batch = new arc::SpriteBatch();
        _texture = arc::Texture2D::loadFromFile("data/bg_stars.png");
        _camera = new arc::OrthographicCamera();
        _camera->setToOrtho(arc::Core::graphics->getWidth(), arc::Core::graphics->getHeight());
        _camera->update();
    }

    void update(float dt) override {
        _camera->update();
    }

    void render(float dt) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);


        _batch->setProjectionMatrix(_camera->combined);
        _batch->begin();

        float w = arc::Core::graphics->getWidth();
        float h = arc::Core::graphics->getHeight();

        _batch->draw(_texture, {0, 0}, {256, 256});
        _batch->draw(_texture, {w - 256, 0}, {256, 256});
        _batch->draw(_texture, {0, h - 256}, {256, 256});
        _batch->draw(_texture, {w - 256, h - 256}, {256, 256});

        _batch->end();

    }

    void resize(int width, int height) override {
        _camera->viewportWidth = width;
        _camera->viewportHeight = height;
    }

    void dispose() override {
        delete _batch;
        delete _texture;
        delete _camera;
    }

};

int main(int argc, char **argv) {
    auto config = arc::Configuration();
    config.windowTitle = "Sample 05 - SpriteBatch";
    auto myGame = new MyGame();
    auto engine = new arc::Engine(myGame, config);
    engine->run();

    return 0;
}