#pragma once

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include "Configuration.h"
#include "Core.h"

namespace arc {
    enum HdpiMode
    {
        Logical,
        Pixels
    };

    class IApp;

    class Graphics
    {
    public:
        Graphics(IApp *app, Configuration &configuration) : _app(app), _config(configuration) {
        }

        bool createContext();

        void update();

        void updateBackbufferInfo();

        bool isInitialized() {
            return _initialized;
        }

        bool isIconified() {
            return _iconified;
        }

        bool shouldClose() {

            return glfwWindowShouldClose(_window) == 1;
        }

        float deltaTime() {
            return _deltaTime;
        }

        int fps() {
            return _fps;
        }

        float getWidth() {
            if (_hdpiMode == arc::HdpiMode::Pixels) {
                return _backBufferWidth;
            } else {
                return _logicalWidth;
            }
        }

        float getHeight() {
            if (_hdpiMode == arc::HdpiMode::Pixels) {
                return _backBufferHeight;
            } else {
                return _logicalHeight;
            }
        }

        GLFWwindow *windowHandle() {
            return _window;
        }

        IApp *getApp() {
            return _app;
        }

    private:
        GLFWwindow *_window;
        int _width = 1280;
        int _height = 720;
        int _backBufferWidth;
        int _backBufferHeight;
        int _logicalWidth;
        int _logicalHeight;

        HdpiMode _hdpiMode = HdpiMode::Logical;

        bool _iconified = false;
        double _lastFrameTime = -1;
        float _deltaTime = 0;
        long _frameId = 0;
        double _frameCounterStart = 0;
        int _frames = 0;
        int _fps = 0;

        IApp *_app;
        Configuration _config;

        bool _initialized = false;


        void track();
    };
}