#include <string.h>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

#include <utils/IO.h>
#include <gfx/fonts/BitmapFont.h>
#include <gfx/SpriteBatch.h>
#include <utils/HdpiUtils.h>
#include <scene2d/Stage.h>
#include <scene2d/Table.h>
#include <scene2d/PrimitiveDrawable.h>


class MyGame : public arc::IApp
{
    arc::BitmapFont* _font;
    arc::SpriteBatch* _spriteBatch;
    arc::Texture2D* _texture;
    arc::Stage* _stage;

    void create() override {

        _font = new arc::BitmapFont("data/fonts/m6x11_16_o.fnt", false, false);

        _spriteBatch = new arc::SpriteBatch();
        _texture = arc::Texture2D::loadFromFile("data/bg_stars.png");

        _stage = new arc::Stage();

        auto* a = new arc::Table();
        a->setBackground(new arc::PrimitiveDrawable(arc::Color::BLUE, 50,50));

        auto* root = new arc::Table();
        root->setFillParent(true);
        root->setBackground(new arc::PrimitiveDrawable(arc::Color::RED, 200,200));
        root->row();
        root->add(a);
        root->row();
        root->add(a);
        root->row();
        root->add(a);
        root->row();
        root->add(a);
        root->row();


        _stage->addActor(root);
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

        if(arc::Core::input->isKeyJustPressed(arc::Keys::SPACE))
        {
        }


        _stage->act(dt);
    }

    void render(float dt) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        _stage->render(dt);

        _font->getData().scaleX = 2;
        _font->getData().scaleY = 2;
        _font->enableColorMarkup = true;
        float y = arc::Core::graphics->getHeight();

        _spriteBatch->begin();

        //_spriteBatch->draw(_texture, {0,y - 720}, {1280,720});

        //auto bounds = _font->draw(_spriteBatch, "Hello", 0, y);
        //auto bounds = _font->draw(_spriteBatch, "[#FF0000]reeeeeeeeeeeeeeeeeeeeeeed[]white[#00FF00]gr[#0000FF]ee[]n[]", 0, y);
        //auto bounds = _font->draw(_spriteBatch, "[#FF0000]Hello[]", 50, 50);
        auto bounds = _font->draw(_spriteBatch, "[#FF5555]Hello[] [#55FF55]Colored[] [#5555FF]World[] Yay", 50, 50);


        //printf("Bounds: %f:%f:%f:%f\n", bounds.x, bounds.y, bounds.width, bounds.height);


        _spriteBatch->end();
    }

    void resize(int width, int height) override {

    }

    void dispose() override {
    }

};

int main(int argc, char** argv) {
    auto config = arc::Configuration();
    config.windowTitle = "Sample 13 - GUI";
    config.vsync = true;
    auto myGame = new MyGame();
    auto engine = new arc::Engine(myGame, config);
    engine->run();

    return 0;
}