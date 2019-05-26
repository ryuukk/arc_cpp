#pragma once

#include <string>

namespace arc
{
    namespace directory
    {
        inline static std::string dirName(const std::string& path) {
            size_t found = path.find_last_of("/\\");
            if (found == -1) return "";
            return (path.substr(0, found));
        }

        inline static std::string pathAppend(const std::string& p1, const std::string& p2) {
            char sep = '/';
            std::string tmp = p1;

#ifdef _WIN32
            sep = '\\';
#endif
            if (p1[p1.length()] != sep) { // Need to add a
                tmp += sep;                // path separator
                return(tmp + p2);
            }
            else
                return(p1 + p2);
        }

    }
	/*
    const std::string currentPath()
    {
        return std::filesystem::current_path().string();
    }
	*/
}