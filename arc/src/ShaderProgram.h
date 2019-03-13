#pragma once

#include <string>
#include <unordered_map>
#include <vector>

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

    private:
        std::string _log;
        bool _isCompiled;

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

        bool _invalidated;
        int _refCount;

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


