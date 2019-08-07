#include <chrono>
#include "Graphics.h"


// GLFWwindow*,int,int
void onFrameBufferResize(GLFWwindow *window, int width, int height)
{
    arc::Core::graphics->updateBackbufferInfo();
    if (!arc::Core::graphics->isInitialized())
    {
        return;
    }
    glViewport(0, 0, width, height);

    arc::Core::app->resize(width, height);

    glfwSwapBuffers(window);
}


bool arc::Graphics::createContext()
{
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _config.glMajVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _config.glMinVersion);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    // delay window opening to avoid positioning glitch and white window
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);


    _window = glfwCreateWindow(_config.windowWidth, _config.windowHeight, _config.windowTitle.c_str(), NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        return false;
    }

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Core::logger->error("Failed to initialize GLAD");
        return false;
    }

    // delay window opening to avoid positioning glitch and white window
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glfwSwapBuffers(_window);
    glfwShowWindow(_window);


    Core::logger->infof("Created window with size: {0}:{1}", _config.windowWidth, _config.windowHeight);

    Core::logger->infof("Vendor:    {0}", glGetString(GL_VENDOR));
    Core::logger->infof("Renderer:  {0}", glGetString(GL_RENDERER));
    Core::logger->infof("Version:   {0}", glGetString(GL_VERSION));
    Core::logger->infof("GLSL:      {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));


    glViewport(0, 0, _width, _height);
    glfwSetFramebufferSizeCallback(_window, &onFrameBufferResize);

    // cbs

    return true;
}

void arc::Graphics::update() {
    if(!_initialized)
    {
        _app->create();
        _app->resize(_backBufferWidth, _backBufferHeight);
		_initialized = true;
    }
    glfwMakeContextCurrent(_window);

    track();
    
    _app->render(_deltaTime);
    _app->update(_deltaTime);


    glfwSwapBuffers(_window);
}

void arc::Graphics::updateBackbufferInfo() {
    glfwGetFramebufferSize(_window, &_backBufferWidth, &_backBufferHeight);
    glfwGetWindowSize(_window, &_logicalWidth, &_logicalHeight);
}

void arc::Graphics::track() {

    auto time = glfwGetTime();

    if (_lastFrameTime == -1)
        _lastFrameTime = time;

    _deltaTime = (float) (time - _lastFrameTime);
    _lastFrameTime = time;

    if (time - _frameCounterStart >= 1)
    {
        _fps = _frames;
        _frames = 0;
        _frameCounterStart = time;
    }

    _frames++;
    _frameId++;
}
