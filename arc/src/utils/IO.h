#pragma once

#include <string>
#include <fstream>

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
                return (tmp + p2);
            } else
                return (p1 + p2);
        }

    }


    namespace file
    {
        inline std::vector<unsigned char> loadFile(const std::string& path) {
            std::ifstream infile(path, std::ios_base::binary);

            return {std::istreambuf_iterator<char>(infile),
                    std::istreambuf_iterator<char>()};
        }

        inline std::string readFile(const std::string& path) {
            std::ifstream file(path);
            std::string str;
            std::string file_contents;
            while (std::getline(file, str)) {
                file_contents += str;
                file_contents.push_back('\n');
            }
            return file_contents;
        }

        inline bool exists(const std::string& name) {
            std::ifstream file(name.c_str());
            return file.good();
        }

        inline std::string parent(const std::string& path) {
            // todo: figure this out
            return "";
        }
    }
}