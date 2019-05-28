#include "ShaderProgram.h"
#include "../Core.h"
#include "../utils/Format.h"
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
    }
}

void arc::ShaderProgram::fetchUniforms() {

    int numUniforms;
    glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &numUniforms);

    _uniformNames.resize(numUniforms);
    for (int i = 0; i < numUniforms; i++) {
        GLchar *buffer = new char[64];
        GLenum type;
        int size;
        int length;
        glGetActiveUniform(_program, i, 64, &length, &size, &type, buffer);

        std::string name((char *) &buffer[0], length);
        int location = glGetUniformLocation(_program, buffer);

        _uniforms[name] = location;
        _uniformTypes[name] = type;
        _uniformSizes[name] = size;
        _uniformNames[i] = name;

        delete[] buffer;
    }
}

int arc::ShaderProgram::fetchAttributeLocation(std::string &name) {
    // -2 == not yet cached
    // -1 == cached but not found
    auto search = _attributes.find(name);
    int location = -2;
    if (search != _attributes.end()) {
        location = glGetAttribLocation(_program, name.c_str());
        _attributes[name] = location;
    }
    return location;
}

void arc::ShaderProgram::checkManaged() {
    if (_invalidated)
    {
        Core::logger->infof("Recompile shader: {0}", _program);

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
    glVertexAttribPointer(location, size, type, normalize ? GL_TRUE : GL_FALSE, stride, (void*) offset);
}

void arc::ShaderProgram::begin() {
    checkManaged();

    glUseProgram(_program);
}

void arc::ShaderProgram::end() {
    glUseProgram(0);
}


int arc::ShaderProgram::fetchUniformLocation(const std::string &name, bool pedantic)
{
    int location = -2;

    auto it = _uniforms.find(name);
    if (it == _uniforms.end())
        location = -2;
    else
        location = _uniforms[name];

    if(location == -2)
    {
        location =  glGetUniformLocation(_program, name.c_str());
        if(location == -1 && pedantic)
            throw std::runtime_error("no uniform with name %s in shader"); // todo: figure out exceptions in c++

        _uniforms[name] = location;
    }

    return location;
}

void arc::ShaderProgram::setUniformi(const std::string& name, int value)
{
    checkManaged();
    int location = fetchUniformLocation(name, true); // todo: change once static pedantic bool added
    glUniform1i(location, value);
}

void arc::ShaderProgram::setUniformf(const std::string& name, float value)
{
    checkManaged();
    int location = fetchUniformLocation(name, true); // todo: change once static pedantic bool added
    glUniform1f(location, value);
}

void arc::ShaderProgram::setUniform4f(const std::string& name, float a, float b, float c, float d)
{
    checkManaged();
    int location = fetchUniformLocation(name, true); // todo: change once static pedantic bool added
    glUniform4f(location, a, b, c, d);
}

void arc::ShaderProgram::setUniformMat4(const std::string& name, arc::Mat4& value, bool transpose)
{
    checkManaged();
    int location = fetchUniformLocation(name, true); // todo: change once static pedantic bool added

    glUniformMatrix4fv(location, 1, transpose, &value.m00);
}

void arc::ShaderProgram::setUniformMat4Array(const std::string& name, int count, std::vector<arc::Mat4>& value, bool transpose){
    checkManaged();
    int location = fetchUniformLocation(name, true); // todo: change once static pedantic bool added

    glUniformMatrix4fv(location, count, transpose, (float*) &value[0].m00);
}



void arc::ShaderProgram::setUniformi(int location, int value)
{
    checkManaged();
    glUniform1i(location, value);
}

void arc::ShaderProgram::setUniformf(int location, float value)
{
    checkManaged();
    glUniform1f(location, value);
}

void arc::ShaderProgram::setUniform4f(int location, float a, float b, float c, float d)
{
    checkManaged();
    glUniform4f(location, a, b, c, d);
}

void arc::ShaderProgram::setUniformMat4(int location, arc::Mat4& value, bool transpose)
{
    checkManaged();
    glUniformMatrix4fv(location, 1, transpose, &value.m00);
}

void arc::ShaderProgram::setUniformMat4Array(int location, int count, std::vector<arc::Mat4>& value, bool transpose){
    checkManaged();
    glUniformMatrix4fv(location, count, transpose, (float*) &value[0].m00);
}


