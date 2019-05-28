#include <string.h>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

#include <utils/FileUtils.h>
#include <gfx/fonts/BitmapFont.h>
#include <gfx/SpriteBatch.h>
#include <gfx/Camera.h>
#include <utils/Format.h>

class MyGame : public arc::IApp
{
    arc::BitmapFont* _font;
    arc::SpriteBatch* _spriteBatch;
    arc::Texture2D* _texture;
    arc::OrthographicCamera* _camera;

    void create() override {

        _font = new arc::BitmapFont("data/fonts/m6x11_16_o.fnt", false, false);
        _font->getData().scaleX = 1;
        _font->getData().scaleY = 1;

        _spriteBatch = new arc::SpriteBatch();
        _texture = arc::Texture2D::loadFromFile("data/bg_stars.png");

        _camera = new arc::OrthographicCamera();
        _camera->setToOrtho(arc::Core::graphics->getWidth(), arc::Core::graphics->getHeight(), false);
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
    }

    void render(float dt) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        if (arc::Core::input->isKeyJustPressed(arc::Keys::SPACE)) {
            _font->enableColorMarkup = !_font->enableColorMarkup;
        }
        if (arc::Core::input->isKeyJustPressed(arc::Keys::UP)) {
            _font->getData().scaleX += 1;
            _font->getData().scaleY += 1;
        }
        if (arc::Core::input->isKeyJustPressed(arc::Keys::DOWN)) {
            _font->getData().scaleX -= 1;
            _font->getData().scaleY -= 1;
            if (_font->getData().scaleX <= 0)
            {
                _font->getData().scaleX = 1;
                _font->getData().scaleY = 1;
            }
        }

        _spriteBatch->setProjectionMatrix(_camera->combined);
        _spriteBatch->begin();

        float y = _camera->viewportHeight;
        float lineHeight = _font->getData().lineHeight * _font->getData().scaleY;
        _font->draw(_spriteBatch, arc::Format("FPS:  {0}", arc::Core::graphics->fps()), 0, y);
        _font->draw(_spriteBatch, "Press <SPACE> to toggle Color Markup", 0, y - lineHeight);
        _font->draw(_spriteBatch, "Press <UP> or <DOWN> to scale up or down the font", 0, y - lineHeight * 2);
        _font->draw(_spriteBatch, "[#FF5555]Hello[] [#55FF55]Colored[] [#5555FF]World[] Yay", 0, y - lineHeight * 3);


        _spriteBatch->end();
    }

    void resize(int width, int height) override {

    }

    void dispose() override {
    }

};

int main(int argc, char** argv) {
    auto config = arc::Configuration();
    config.windowTitle = "Sample 10 - BitmapFont";
    config.vsync = true;
    auto myGame = new MyGame();
    auto engine = new arc::Engine(myGame, config);
    engine->run();

    return 0;
}