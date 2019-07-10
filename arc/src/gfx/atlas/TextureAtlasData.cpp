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

std::vector<int> readToken(const std::string& line)
{
    auto s = arc::string::split(line, ",");
    std::vector<int> ret;
    ret.resize(s.size());
    for (int i = 0; i < s.size(); ++i) {
        ret[i] = std::stoi(s[i]);
    }
    return ret;
}

int arc::TextureAtlasData::readTuple(std::ifstream& stream) {

    std::string buffer;
    std::getline(stream, buffer);
    buffer = buffer.substr(buffer.find(':') + 1);

    auto vec = readToken(buffer);
    for (int i = 0; i < vec.size(); ++i) {
        _tuple[i] = vec[i];
    }
    return vec.size();
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

        //arc::log::infof("Page file: {0}", page.file);

        // size
        readTuple(stream);
        page.width = _tuple[0];
        page.height = _tuple[1];


        //arc::log::infof("Page size: {0}:{1}", page.width, page.height);

        // format
        std::getline(stream, buffer);
        auto format = buffer.substr(8);

        // filter
        std::getline(stream, buffer);
        auto filter = buffer.substr(8);

        // repeat
        std::getline(stream, buffer);
        auto repeat = buffer.substr(8);
        pages.emplace_back(page);

        while (true)
        {
            if(!std::getline(stream, buffer)) break;
            if(buffer.empty()) break;

            Region region{};
            region.pageIndex = 0; // todo: support multiple pages
            region.name = buffer;

            // rotate
            if(!std::getline(stream, buffer)) break;
            region.rotate = buffer.substr(10) == "true";

            // xy
            readTuple(stream);
            region.left = _tuple[0];
            region.top = _tuple[1];

            // size
            readTuple(stream);
            region.width = _tuple[0];
            region.height = _tuple[1];


            if(readTuple(stream) == 4)
            {
                region.hasSplits = true;
                region.splits.resize(4);
                region.splits[0] = _tuple[0];
                region.splits[1] = _tuple[1];
                region.splits[2] = _tuple[2];
                region.splits[3] = _tuple[3];
                if(readTuple(stream) == 4)
                {
                    region.hasPads = true;
                    region.pads.resize(4);
                    region.pads[0] = _tuple[0];
                    region.pads[1] = _tuple[1];
                    region.pads[2] = _tuple[2];
                    region.pads[3] = _tuple[3];
                    readTuple(stream);
                }
            }

            // orig
            region.originalWidth = _tuple[0];
            region.originalHeight = _tuple[1];

            // offset
            readTuple(stream);
            region.offsetX = _tuple[0];
            region.offsetY = _tuple[1];


            // index
            readTuple(stream);
            region.index = _tuple[0];

            if(flip) region.flip = true;
            regions.emplace_back(region);

            // todo: figure out why printf can't print super long lines when there is std::string
            //printf("Region: %s xy: %d:%d size: %d:%d orig: %d:%d offset: %d:%d index: %d\n",
            //       region.name.c_str(),
            //       region.top,
            //       region.left,
            //       region.width,
            //       region.height,
            //       region.originalWidth,
            //       region.originalHeight,
            //       region.offsetX,
            //       region.offsetY,
            //       region.index
            //       );

            //printf("Region: %s\n",
            //        region.name.c_str()
            //        );
        }
    }
}
