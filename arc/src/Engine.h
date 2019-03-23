#pragma once


#include "Graphics.h"

namespace arc
{
    class Engine
    {
    public:
        Engine(IApp* app, Configuration& config): _app(app), _config(config)
        {
        }
        void run();

    private:
        Graphics* _graphics = nullptr;
        Input* _input = nullptr;
        IApp* _app = nullptr;
        Configuration _config;
        bool _running = true;
    };

    void Engine::run()
    {
        _graphics = new Graphics(_app, _config);
        _input = new Input();

        Core::graphics = _graphics;
        Core::input = _input;
        Core::app = _app;

        _graphics->createContext();

        while(_running)
        {
            // ru;nables


            if (!_graphics->isIconified())
                _input->update();


            _graphics->update();


            if (!_graphics->isIconified())
                _input->prepareNext();

            glfwPollEvents();

            _running = !_graphics->shouldClose();
        }

        _app->dispose();

        glfwTerminate();

        delete(_graphics);
        delete(_app);
    }
}