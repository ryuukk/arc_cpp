#include <string.h>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

#include <utils/IO.h>
#include <gfx/fonts/BitmapFont.h>
#include <gfx/ShapeRenderer.h>
#include <gfx/Camera.h>
#include <utils/Format.h>
#include <utils/CameraController.h>

//#define TWO_D

class MyGame : public arc::IApp
{
#ifdef TWO_D
    arc::OrthographicCamera* _camera;
#else
    arc::PerspectiveCamera* _camera;
    arc::CameraController* _controller;
#endif


    arc::ShapeRenderer* _shapes;

    void create() override {

#ifdef TWO_D
        _camera = new arc::OrthographicCamera();
        _camera->setToOrtho(arc::Core::graphics->getWidth(), arc::Core::graphics->getHeight(), false);
#else
        _camera = new arc::PerspectiveCamera(67, arc::Core::graphics->getWidth(), arc::Core::graphics->getHeight());
        _camera->nearr = 0.1f,
        _camera->farr = 1000;
        _camera->position = {100, 100, 100};
        _camera->update();
        _controller = new arc::CameraController(_camera);
        arc::Core::input->setInputProcessor(_controller);
#endif
        _shapes = new arc::ShapeRenderer();
    }

    int fpsAcc = 0;
    int c = 0;
    float timer = 0.0f;

    void update(float dt) override {

        auto fps = arc::Core::graphics->fps();
        fpsAcc += fps;
        timer += dt;
        c++;

        if (timer > 1.0f) {
            int f = fpsAcc / c;
            printf("FPS: %d  AVG: %d\n", fps, f);

            c = 0;
            fpsAcc = 0;
            timer = 0;
        }

#ifndef TWO_D
        _controller->update(dt);
#endif
    }

    void render(float dt) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glEnable(GL_DEPTH_TEST);
        _shapes->setProjectionMatrix(_camera->combined);
        _shapes->begin(arc::ShapeType::Line);
        _shapes->box({0, 0, 0}, {200, 200, 200});
        _shapes->end();
    }

    void resize(int width, int height) override {

    }

    void dispose() override {
    }

};

int main(int argc, char** argv) {
    auto config = arc::Configuration();
    config.windowTitle = "Sample 12 - Shapes";
    config.vsync = true;
    auto myGame = new MyGame();
    auto engine = new arc::Engine(myGame, config);
    engine->run();

    return 0;
}