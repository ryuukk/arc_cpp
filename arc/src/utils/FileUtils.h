#pragma once

#include <string>
#include <fstream>

namespace arc
{
    namespace file
    {
        inline std::string readFile(const std::string& path)
        {
            std::ifstream file(path);
            std::string str;
            std::string file_contents;
            while (std::getline(file, str)) {
                file_contents += str;
                file_contents.push_back('\n');
            }
            return file_contents;
        }

        inline bool exists (const std::string& name) {
            std::ifstream file(name.c_str());
            return file.good();
        }

        inline std::string parent(const std::string& path)
        {
            // todo: figure this out
            return "";
        }
    }
}