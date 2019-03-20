#pragma once

#include "GLTexture.h"

namespace arc
{
    // todo: figure out how to do: where T : GLTexture
    template <class T>
    class TextureDescriptor
    {
    public:
        T* texture = nullptr;
        TextureFilter minFilter;
        TextureFilter magFilter;
        TextureWrap uWrap;
        TextureWrap vWrap;
    };

}

