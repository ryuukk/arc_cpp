#pragma once
#include <thread>
#include <utils/TimeUtils.h>
#include <utils/StopWatch.h>

namespace arc
{
    class Headless
    {
    public:
        bool quit;
        int tickRate = 1000 / 5;

    public:
        virtual ~Headless() = default;
        void run()
        {
            auto t = 0;
            auto sw = arc::Stopwatch();

            auto currentTime = sw.elapsed();
            auto accumulator = 0.0;

            init();

            while(!quit)
            {
                auto newTime = sw.elapsed();
                auto frameTime = newTime - currentTime;
                currentTime = newTime;

                accumulator += frameTime;

                while (accumulator >= tickRate)
                {
                    onTick(t, tickRate);
                    accumulator -= tickRate;
                    t += tickRate;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            dispose();
        }

        virtual void init() = 0;
        virtual void dispose() = 0;
        virtual void onTick(int time, int dt) = 0;
    };
}