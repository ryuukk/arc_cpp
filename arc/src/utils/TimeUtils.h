#pragma once

#include <chrono>
#include <cstdint>

namespace arc
{
    namespace time
    {
        static inline uint64_t milliseconds()
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>
                    (std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        }

        static inline uint64_t microseconds()
        {
            return std::chrono::duration_cast<std::chrono::microseconds>
                    (std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        }

        static inline uint64_t nanoseconds()
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>
                    (std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        }
    }
}