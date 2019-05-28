#pragma once

#include <stdio.h>
#include <string>


namespace arc
{
    class Logger
    {
    public:
        void info(const std::string& txt)
        {
            //fmt::printf("[INFO] %s\n", txt,  args...);

            printf("[INFO] %s\n", txt.c_str());
        }
        void error(const std::string& txt)
        {
            printf("[ERROR] %s\n", txt.c_str());
        }
    };
}