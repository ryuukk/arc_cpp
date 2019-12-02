#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "../gfx/Texture2D.h"

namespace arc
{
    using std::string;
    using std::vector;
    using std::unordered_map;

    template<typename T>
    class IAssetLoader
    {
    public:
        virtual std::shared_ptr<T> load(string path) = 0;
    };

    class TextureLoader : public IAssetLoader<arc::Texture2D>
    {
    public :
        std::shared_ptr<arc::Texture2D> load(string path) override;
    };


    struct LoaderInfo
    {
    public:
        void* loader;
        bool cache = false;
    };

    struct AssetInfo
    {
    public:
        void* asset;
        string key;
        bool watched;
    };

    class AssetManager
    {
    public:
        string root;

        AssetManager() = default;
        ~AssetManager() = default;
    };
}
