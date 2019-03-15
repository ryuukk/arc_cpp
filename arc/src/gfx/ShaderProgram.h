#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "../math/Mat4.h"

namespace arc {
    enum ShaderType
    {
        Vertex,
        Fragment
    };

    class ShaderProgram
    {
    public:
        ShaderProgram(std::string &vs, std::string &fs);
        void disableVertexAttribute(int location);
        void enableVertexAttribute(int location);
        int getAttributeLocation(std::string& name);
        void setVertexAttribute(int location, int size, int type, bool normalize, int stride, int offset);
        void begin();
        void end();

        int fetchUniformLocation(const std::string& name, bool pedantic);

        void setUniformMat4(const std::string& name, Mat4& value, bool transpose = false);

        std::string log;
    private:
        bool _isCompiled = false;

        std::unordered_map<std::string, int> _uniforms;
        std::unordered_map<std::string, int> _uniformTypes;
        std::unordered_map<std::string, int> _uniformSizes;
        std::vector<std::string> _uniformNames;

        std::unordered_map<std::string, int> _attributes;
        std::unordered_map<std::string, int> _attributeTypes;
        std::unordered_map<std::string, int> _attributeSizes;
        std::vector<std::string> _attributeNames;

        int _program;
        int _vertexShaderHandle;
        int _fragmentShaderHandle;
        std::string _vertexShaderSource;
        std::string _fragmentShaderSource;

        bool _invalidated = false;
        int _refCount = 0;

        void compileShaders(std::string &vertexShader, std::string &fragmentShader);

        int loadShader(ShaderType shaderType, std::string &source);

        int createProgram();

        int linkProgram(int program);

        void fetchAttributes();

        void fetchUniforms();

        int fetchAttributeLocation(std::string &name);

        void checkManaged();
    };
}


