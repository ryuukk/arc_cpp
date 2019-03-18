#include "GLTexture.h"

arc::GLTexture::GLTexture(GLenum glTarget) {
    glGenTextures(1, &glHandle);
    this->glTarget = glTarget;
}

void arc::GLTexture::bind() {
    glBindTexture(glTarget, glHandle);
}

void arc::GLTexture::bind(int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(glTarget, glHandle);
}

arc::TextureFilter arc::GLTexture::getMinFilter() {
    return minFilter;
}

arc::TextureFilter arc::GLTexture::getMagFilter() {
    return magFilter;
}

arc::TextureWrap arc::GLTexture::getUWrap() {
    return uWrap;
}

arc::TextureWrap arc::GLTexture::getVWrap() {
    return vWrap;
}

GLenum arc::GLTexture::getGlTarget() {
    return glTarget;
}


void arc::GLTexture::unsafeSetWrap(arc::TextureWrap u, arc::TextureWrap v, bool force) {
    if ((force || uWrap != u))
    {
        glTexParameteri(glTarget, GL_TEXTURE_WRAP_S, getGLEnumFromTextureWrap(u));
        uWrap = u;
    }

    if ((force || vWrap != v))
    {
        glTexParameteri(glTarget, GL_TEXTURE_WRAP_T, getGLEnumFromTextureWrap(v));
        vWrap = v;
    }
}

void arc::GLTexture::setWrap(arc::TextureWrap u, arc::TextureWrap v) {

    this->uWrap = u;
    this->vWrap = v;
    bind();
    glTexParameteri(glTarget, GL_TEXTURE_WRAP_S, getGLEnumFromTextureWrap(u));
    glTexParameteri(glTarget, GL_TEXTURE_WRAP_T, getGLEnumFromTextureWrap(v));
}

void arc::GLTexture::unsafeSetFilter(arc::TextureFilter minFilter, arc::TextureFilter magFilter, bool force) {

    if ((force || this->minFilter != minFilter))
    {
        glTexParameteri(glTarget, GL_TEXTURE_MIN_FILTER, getGLEnumFromTextureFilter(minFilter));
        this->minFilter = minFilter;
    }

    if ((force || this->magFilter != magFilter))
    {
        glTexParameteri(glTarget, GL_TEXTURE_MAG_FILTER, getGLEnumFromTextureFilter(magFilter));
        this->magFilter = magFilter;
    }
}

void arc::GLTexture::setFilter(arc::TextureFilter minFilter, arc::TextureFilter magFilter) {
    this->minFilter = minFilter;
    this->magFilter = magFilter;
    bind();
    glTexParameteri(glTarget, GL_TEXTURE_MIN_FILTER, getGLEnumFromTextureFilter(minFilter));
    glTexParameteri(glTarget, GL_TEXTURE_MAG_FILTER, getGLEnumFromTextureFilter(magFilter));
}

void arc::GLTexture::deletee() {
    if (glHandle != 0)
    {
        glDeleteTextures(1, &glHandle);
        glHandle = 0;
    }
}
