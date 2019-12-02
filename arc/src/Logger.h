#pragma once

#include <cstdio>
#include <string>
#include "utils/Format.h"


namespace arc
{
    namespace log
    {
        inline static void infof(const std::string& txt)
        {
            printf("[INFO] %s\n", txt.c_str());
        }
        inline static void warnf(const std::string& txt)
        {
            printf("[WARN] %s\n", txt.c_str());
        }
        inline static void errorf(const std::string& txt)
        {
            printf("[ERROR] %s\n", txt.c_str());
        }

        template <typename... Args>
        inline static void infof(const std::string& txt, Args&&... args)
        {
            printf("[INFO] %s\n", arc::Format(txt, args...).c_str());
        }

        template <typename... Args>
        inline static void warnf(const std::string& txt, Args&&... args)
        {
            printf("[WARN] %s\n", arc::Format(txt, args...).c_str());
        }

        template <typename... Args>
        inline static void errorf(const std::string& txt, Args&&... args)
        {
            printf("[ERROR] %s\n", arc::Format(txt, args...).c_str());
        }
    }
    
    class Logger
    {
    public:

        void info(const std::string& txt)
        {
            printf("[INFO] %s\n", txt.c_str());
        }
        void error(const std::string& txt)
        {
            printf("[ERROR] %s\n", txt.c_str());
        }

        template <typename... Args>
        void infof(const std::string& txt,  Args&&... args)
        {
            printf("[INFO] %s\n", arc::Format(txt, args...).c_str());
        }

        template <typename... Args>
        void errorf(const std::string& txt, Args&&... args)
        {
            printf("[ERROR] %s\n", arc::Format(txt, args...).c_str());
        }
    };
}