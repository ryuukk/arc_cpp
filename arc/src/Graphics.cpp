
#include <chrono>
#include "Graphics.h"

bool arc::Graphics::createContext()
{
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _config.glMajVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _config.glMinVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // delay window opening to avoid positioning glitch and white window
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);


    _window = glfwCreateWindow(_config.windowWidth, _config.windowHeight, _config.windowTitle.c_str(), NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        return false;
    }
    printf("Created window with size: %d %d\n", _config.windowWidth, _config.windowHeight);

    if (_config.windowX == -1 && _config.windowY == -1)
    {
        auto primaryMonitor = glfwGetPrimaryMonitor();
        auto vidMode = glfwGetVideoMode(primaryMonitor);

        int windowWidth = 0;
        int windowHeight = 0;
        glfwGetWindowSize(_window, &windowWidth, &windowHeight);

        int windowX = vidMode->width / 2 - windowWidth / 2;
        int windowY = vidMode->height / 2 - windowHeight / 2;
        glfwSetWindowPos(_window, windowX, windowY);
    }



    glfwMakeContextCurrent(_window);
    glfwSwapInterval(_config.vsync ? 1 : 0);

    updateBackbufferInfo();

    // delay window opening to avoid positioning glitch and white window
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glfwSwapBuffers(_window);
    glfwShowWindow(_window);


    printf("Vendor:    %s", glGetString(GL_VENDOR));
    printf("Renderer:  %s", glGetString(GL_RENDERER));
    printf("Version:   %s", glGetString(GL_VERSION));
    printf("GLSL:      %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    printf("Created window with size: %d %d\n", _config.windowWidth, _config.windowHeight);

    return false;
}

void arc::Graphics::update() {

}

void arc::Graphics::updateBackbufferInfo() {
    glfwGetFramebufferSize(_window, &_backBufferWidth, &_backBufferHeight);
    glfwGetWindowSize(_window, &_logicalWidth, &_logicalHeight);
}

void arc::Graphics::track() {

    // auto curr = MonoTime.currTime;
    // auto time = curr.ticks;

    auto t0 = std::chrono::high_resolution_clock::now();
    auto time = t0.time_since_epoch().count();

    if (_lastFrameTime == -1)
        _lastFrameTime = time;

    _deltaTime = (time - _lastFrameTime) / 1000000000.0f;
    _lastFrameTime = time;

    if (time - _frameCounterStart >= 1000000000)
    {
        _fps = _frames;
        _frames = 0;
        _frameCounterStart = time;
    }

    _frames++;
    _frameId++;
}
