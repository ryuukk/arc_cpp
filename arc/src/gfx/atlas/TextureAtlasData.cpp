#include <fstream>
#include <tuple>
#include "../../Core.h"
#include "TextureAtlasData.h"
#include "../../utils/StringUtils.h"

std::tuple<int, int> readT2(const std::string& line)
{
    auto content = arc::string::split(line, ",");
    auto a = std::stoi(content[0]);
    auto b = std::stoi(content[1]);
    return {a, b};
}

std::tuple<int, int, int, int> readT4(const std::string& line)
{
    auto content = arc::string::split(line, ",");
    auto a = std::stoi(content[0]);
    auto b = std::stoi(content[1]);
    auto c = std::stoi(content[2]);
    auto d = std::stoi(content[3]);
    return {a, b, c, d};
}

void arc::TextureAtlasData::load(const std::string& packFile, const std::string& imagesDir, bool flip)
{
    std::ifstream stream (packFile, std::ifstream::binary);
    if(stream) {
        Page page;
        std::string buffer;

        // file
        std::getline(stream, buffer);
        page.file = buffer;

        arc::Core::logger->infof("Page file: {0}", page.file);

        // size
        std::getline(stream, buffer);
        auto size = readT2(buffer.substr(6));
        page.width = std::get<0>(size);
        page.height = std::get<1>(size);


        arc::Core::logger->infof("Page size: {0}:{1}", page.width, page.height);



        // format
        std::getline(stream, buffer);
        auto format = buffer.substr(8);

        // filter
        std::getline(stream, buffer);
        auto filter = buffer.substr(8);

        // repeat
        std::getline(stream, buffer);
        auto repeat = buffer.substr(8);

        while (true)
        {
            if(!std::getline(stream, buffer)) break;

            Region region;
            region.name = buffer;


        }
    }
}
