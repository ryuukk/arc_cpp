#pragma once

#include <glad/glad.h>

namespace arc
{
    enum TextureFilter
    {
        Nearest, // GL20.GL_NEAREST
        Linear, // GL20.GL_LINEAR
        MipMap, // GL20.GL_LINEAR_MIPMAP_LINEAR
        MipMapNearestNearest, // GL20.GL_NEAREST_MIPMAP_NEAREST
        MipMapLinearNearest, // GL20.GL_LINEAR_MIPMAP_NEAREST
        MipMapNearestLinear, // GL20.GL_NEAREST_MIPMAP_LINEAR
        MipMapLinearLinear, // GL20.GL_LINEAR_MIPMAP_LINEAR
    };

    enum TextureWrap
    {
        MirroredRepeat, // GL20.GL_MIRRORED_REPEAT
        ClampToEdge, // GL20.GL_CLAMP_TO_EDGE
        Repeat, // GL20.GL_REPEAT
    };

    class GLTexture
    {
    public:
        GLTexture(GLenum glTarget);
        virtual ~GLTexture()
        {
            deletee();
        }

        virtual int getWidth() = 0;
        virtual int getHeight() = 0;
        virtual bool isManaged() = 0;
        virtual void reload() = 0;

        void bind();
        void bind(int unit);

        TextureFilter getMinFilter();
        TextureFilter getMagFilter();

        TextureWrap getUWrap();
        TextureWrap getVWrap();

        GLenum getGlTarget();

        void unsafeSetWrap(TextureWrap u, TextureWrap v, bool force = false);
        void setWrap(TextureWrap u, TextureWrap v);

        void unsafeSetFilter(TextureFilter minFilter, TextureFilter magFilter, bool force = false);
        void setFilter(TextureFilter minFilter, TextureFilter magFilter);

        void deletee();

    private:
        GLenum glTarget;
        GLuint glHandle;
        TextureFilter minFilter = TextureFilter::Nearest;
        TextureFilter magFilter = TextureFilter::Nearest;
        TextureWrap uWrap = TextureWrap::ClampToEdge;
        TextureWrap vWrap = TextureWrap::ClampToEdge;
    };

    inline bool isMipMap(arc::TextureFilter filter)
    {
        return filter != arc::TextureFilter::Nearest && filter != arc::TextureFilter::Linear;
    }

    inline int getGLEnumFromTextureFilter(arc::TextureFilter filter)
    {
        switch (filter)
        {
            case arc::TextureFilter::Nearest:
                return GL_NEAREST;
            case arc::TextureFilter::Linear:
                return GL_LINEAR;
            case arc::TextureFilter::MipMap:
                return GL_LINEAR_MIPMAP_LINEAR;
            case arc::TextureFilter::MipMapNearestNearest:
                return GL_NEAREST_MIPMAP_NEAREST;
            case arc::TextureFilter::MipMapLinearNearest:
                return GL_LINEAR_MIPMAP_NEAREST;
            case arc::TextureFilter::MipMapNearestLinear:
                return GL_NEAREST_MIPMAP_LINEAR;
            case arc::TextureFilter::MipMapLinearLinear:
                return GL_LINEAR_MIPMAP_LINEAR;
        }
    }

    inline int getGLEnumFromTextureWrap(arc::TextureWrap wrap)
    {
        switch (wrap)
        {
            case arc::TextureWrap::MirroredRepeat:
                return GL_MIRRORED_REPEAT;
            case arc::TextureWrap::ClampToEdge:
                return GL_CLAMP_TO_EDGE;
            case arc::TextureWrap::Repeat:
                return GL_CLAMP_TO_EDGE;
        }
    }
}


