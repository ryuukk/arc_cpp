#pragma once


#include "Graphics.h"
#include "Logger.h"

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
        Logger* _logger = nullptr;

        Configuration _config;
        bool _running = true;
    };

    void Engine::run()
    {

        //if(_config.logToFile)
        //{
        //    _logger = spdlog::basic_logger_mt("main_log", _config.logPath).get();
        //}
        //else
        {
            _logger = new Logger();
        }

        //_logger->set_pattern("[%H:%M:%S %z] (%l) %@ %v");

        _logger->info("Starting engine..");


        _graphics = new Graphics(_app, _config);
        _input = new Input();

        Core::graphics = _graphics;
        Core::input = _input;
        Core::app = _app;
        Core::logger = _logger;

        _graphics->createContext();

        _input->windowHandleChanged(_graphics->windowHandle());

        _logger->info("Engine started !");

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