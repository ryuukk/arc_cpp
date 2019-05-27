#include <fstream>
#include <string.h>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

class MyController : public arc::InputProcessor
{
public:
    bool keyDown(arc::Keys keycode) override {
        printf("KEY DOWN: \n");
        return false;
    }

    bool keyUp(arc::Keys keycode) override {
        printf("KEY UP: \n");
        return false;
    }

    bool keyTyped(char character) override {
        printf("KEY TYPED: %c\n", character);
        return false;
    }

    bool touchDown(int screenX, int screenY, int pointer, arc::Buttons button) override {
        printf("TOUCH DOWN %d -> %d:%d\n", pointer, screenX, screenY);
        return false;
    }

    bool touchUp(int screenX, int screenY, int pointer, arc::Buttons button) override {
        printf("TOUCH UP: %d -> %d:%d\n", pointer, screenX, screenY);
        return false;
    }

    bool touchDragged(int screenX, int screenY, int pointer) override {
        printf("TOUCH DRAGGED: %d -> %d:%d\n", pointer, screenX, screenY);
        return false;
    }

    bool mouseMoved(int screenX, int screenY) override {
        printf("MOUSE MOVED: %d:%d\n", screenX, screenY);
        return false;
    }

    bool scrolled(int amount) override {
        printf("SCROLLED: %d\n", amount);
        return false;
    }
};

class MyGame : public arc::IApp
{
    MyController* _controller = nullptr;

    void create() override {
        _controller = new MyController();
        arc::Core::input->setInputProcessor(_controller);
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
    config.windowTitle = "Sample 02 - Input";
    auto myGame = new MyGame();
    auto engine = new arc::Engine(myGame, config);
    engine->run();

    return 0;
}