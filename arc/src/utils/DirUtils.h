#pragma once

#include <string>

namespace arc
{
    std::string dirName(const std::string& path) {
        size_t found = path.find_last_of("/\\");
        if (found == -1) return "";
        return (path.substr(0, found));
    }
}