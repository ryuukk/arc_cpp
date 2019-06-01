#include "Skin.h"
#include "../utils/IO.h"
#include "../utils/json11.hpp"

arc::Skin::Skin() {

}

arc::Skin::~Skin() {
    delete _atlas;
    for(auto& a : _resources)
    {
        for(auto& b : a.second)
        {
            // todo: is it how to iterate over all values of a map ????
        }
    }
}

void arc::Skin::load(const std::string& file)
{
    // todo: handle errors

    if(arc::file::exists(file))
    {
        std::string file_contents = arc::file::readFile(file);
        std::string error;
        auto json = json11::Json::parse(file_contents, error, json11::JsonParse::COMMENTS);
    }
}

void arc::Skin::loadDefault()
{

}
