#include "ShaderProgram.h"
#include <assert.h>
#include <glad/glad.h>

arc::ShaderProgram::ShaderProgram(std::string &vs, std::string &fs) {
    assert(vs.length() > 0);
    assert(fs.length() > 0);

    _vertexShaderSource = vs;
    _fragmentShaderSource = fs;

    compileShaders(_vertexShaderSource, _fragmentShaderSource);

    if (_isCompiled) {
        fetchAttributes();
        fetchUniforms();
    }
}

void arc::ShaderProgram::compileShaders(std::string &vertexShader, std::string &fragmentShader) {
    _vertexShaderHandle = loadShader(ShaderType::Vertex, vertexShader);
    _fragmentShaderHandle = loadShader(ShaderType::Fragment, fragmentShader);

    if (_vertexShaderHandle == -1 || _fragmentShaderHandle == -1) {
        _isCompiled = false;
        return;
    }

    _program = linkProgram(createProgram());
    if (_program == -1) {
        _isCompiled = false;
        return;
    }

    _isCompiled = true;
}

int arc::ShaderProgram::loadShader(arc::ShaderType shaderType, std::string &source) {
    int shader = glCreateShader(shaderType == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    if (shader == 0)
        return -1;

    int compiled;
    auto ssp = source.c_str();
    int ssl = source.length();

    glShaderSource(shader, 1, &ssp, &ssl);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (compiled == 0) {
        GLint logLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

        auto *msgBuffer = new char[logLen];
        glGetShaderInfoLog(shader, logLen, NULL, &msgBuffer[0]);

        log.append(shaderType == ShaderType::Vertex ? "Vertex Shader\n" : "Fragment Shader\n");
        log.append(std::string(msgBuffer));

        delete[] msgBuffer;

        return -1;
    }
    return shader;
}

int arc::ShaderProgram::createProgram() {
    auto program = glCreateProgram();
    return program != 0 ? program : -1;
}

int arc::ShaderProgram::linkProgram(int program) {
    if (program == -1)
        return -1;

    glAttachShader(program, _vertexShaderHandle);
    glAttachShader(program, _fragmentShaderHandle);
    glLinkProgram(program);

    int linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == 0) {
        GLint logLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

        GLchar *msgBuffer = new char[logLen];
        glGetProgramInfoLog(program, logLen, NULL, &msgBuffer[0]);

        log.append(msgBuffer);

        delete[] msgBuffer;

        return -1;
    }

    return program;
}

void arc::ShaderProgram::fetchAttributes() {
    int numAttributes;
    glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTES, &numAttributes);

    _attributeNames.resize(numAttributes);
    for (int i = 0; i < numAttributes; i++) {
        GLchar *buffer = new char[96];
        GLenum type;
        int size;
        int length;
        glGetActiveAttrib(_program, i, 96, &length, &size, &type, buffer);

        std::string name((char *) &buffer[0], length);
        int location = glGetAttribLocation(_program, buffer);

        _attributes[name] = location;
        _attributeTypes[name] = type;
        _attributeSizes[name] = size;
        _attributeNames[i] = name;

        delete[] buffer;
        {
            printf("ATTRIBUTE: %s loc: %d type: %d size: %d\n", name.c_str(), location, type, size);
        }
    }
}

void arc::ShaderProgram::fetchUniforms() {

    int numUniforms;
    glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &numUniforms);

    _uniformNames.resize(numUniforms);
    for (int i = 0; i < numUniforms; i++) {
        GLchar *buffer = new char[96];
        GLenum type;
        int size;
        int length;
        glGetActiveUniform(_program, i, 96, &length, &size, &type, buffer);

        std::string name((char *) &buffer[0], length);
        int location = glGetUniformLocation(_program, buffer);

        _uniforms[name] = location;
        _uniformTypes[name] = type;
        _uniformSizes[name] = size;
        _uniformNames[i] = name;

        delete[] buffer;
        {
            printf("UNIFORM: %s loc: %d type: %d size: %d\n", name.c_str(), location, type, size);
        }
    }
}

int arc::ShaderProgram::fetchAttributeLocation(std::string &name) {
    // -2 == not yet cached
    // -1 == cached but not found
    auto search = _attributes.find(name);
    if (search != _attributes.end()) {
        int location = glGetAttribLocation(_program, name.c_str());
        _attributes[name] = location;

    } else {
        return -2;
    }
}

void arc::ShaderProgram::checkManaged() {
    if (_invalidated)
    {
        printf("Recompile shader\n");

        compileShaders(_vertexShaderSource, _fragmentShaderSource);
        _invalidated = false;
    }
}

void arc::ShaderProgram::disableVertexAttribute(int location)
{
    checkManaged();
    glDisableVertexAttribArray(location);
}
void arc::ShaderProgram::enableVertexAttribute(int location)
{
    checkManaged();
    glEnableVertexAttribArray(location);
}

int arc::ShaderProgram::getAttributeLocation(std::string& name)
{
    if (_attributes.find(name) == _attributes.end()) {
        return -1;
    } else {
        return _attributes[name];
    }
}

void arc::ShaderProgram::setVertexAttribute(int location, int size, int type, bool normalize, int stride, int offset) {
    checkManaged();
    glVertexAttribPointer(location, size, type, normalize ? GL_TRUE : GL_FALSE, stride, &offset);
}

void arc::ShaderProgram::begin() {
    checkManaged();

    glUseProgram(_program);
}

void arc::ShaderProgram::end() {
    glUseProgram(0);
}


