#pragma once

#include <GLFW/glfw3.h>
#include "Configuration.h"
#include "Core.h"

namespace arc {
    enum HdpiMode {
        Logical,
        Pixels
    };

    class Graphics {
    public:
        Graphics(arc::IApp app, arc::Configuration configuration)
        {
            _app = app;
            _config = configuration;
        }

        bool createContext();
        void update();
    private:
        GLFWwindow *_window;
        int _width = 1280;
        int _height = 720;
        int _backBufferWidth;
        int _backBufferHeight;
        int _logicalWidth;
        int _logicalHeight;

        HdpiMode _hdpiMode = arc::HdpiMode::Logical;

        bool _iconified = false;
        long _lastFrameTime = -1;
        float _deltaTime = 0;
        long _frameId = 0;
        long _frameCounterStart = 0;
        int _frames = 0;
        int _fps = 0;

        arc::IApp _app;
        arc::Configuration _config;

        bool _initialized;

        void updateBackbufferInfo();
        void track();
    };
}