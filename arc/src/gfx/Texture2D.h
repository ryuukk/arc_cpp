#pragma once

#include <string>
#include <vector>
#include "GLTexture.h"

namespace arc
{
    class Texture2D : public GLTexture
    {
    public:
        Texture2D() : GLTexture(GL_TEXTURE_2D){}
        ~Texture2D()
        {
            delete _data;
        }

        int getWidth() override {
            return _width;
        }

        int getHeight() override {
            return _height;
        }

        bool isManaged() override {
            return false;
        }

        void reload() override {

        };

        void setData(unsigned char* data, int w, int h)
        {
            _data = data;
            _width = w;
            _height = h;

            bind();

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(getGlTarget(), 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            unsafeSetWrap(getUWrap(), getVWrap(), true);
            unsafeSetFilter(getMinFilter(), getMagFilter(), true);

            glBindTexture(getGlTarget(), 0);
        }

    private:
        int _width;
        int _height;
        unsigned char* _data;

    public:
        static Texture2D* loadFromFile(const std::string& filename);
    };
}


