#pragma once


namespace arc
{
    class Logger
    {
    public:
        template<typename... Args>
        void info(const char *txt, const Args &... args)
        {
            //fmt::printf("[INFO] %s\n", txt,  args...);
        }
        template<typename... Args>
        void error(const char *txt, const Args &... args)
        {
            //fmt::printf("[ERROR] %s\n", txt,  args...);
        }
    };
}