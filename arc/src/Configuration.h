#pragma once

#include <string>

namespace arc {
    class Configuration {
    public:
        int glMinVersion = 3;
        int glMajVersion = 3;

        int windowWidth = 1280;
        int windowHeight = 720;

        int windowX = -1;
        int windowY = -1;

        std::string windowTitle = "";

        bool vsync = true;

        bool logToFile = false;
        std::string logPath = "log.txt";
    };
}