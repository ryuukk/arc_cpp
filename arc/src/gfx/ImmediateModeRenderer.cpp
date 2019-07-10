#include <assert.h>
#include "ImmediateModeRenderer.h"
#include "../utils/Format.h"

arc::ImmediateModeRenderer::ImmediateModeRenderer(int maxVertices, bool hasNormals, bool hasColors, int numTexCoords):
    maxVertices(maxVertices), numTexCoords(numTexCoords)
{
    ownsShader = true;

    std::string vs = "";
    std::string fs = "";

    // vertex shader
    {
        vs.append("attribute vec4 a_position;\n");
        if(hasNormals)
            vs.append("attribute vec3 a_normal;\n");
        if(hasColors)
            vs.append("attribute vec4 a_color;\n");

        for (int l = 0; l < numTexCoords; ++l) {
            vs.append("attribute vec2 a_texCoord").append(std::to_string(l)).append(";\n");
        }

        vs.append("uniform mat4 u_projModelView;\n");
        if(hasColors)
            vs.append("varying vec4 v_col;\n");

        for (int l = 0; l < numTexCoords; ++l) {
            vs.append("varying vec2 v_tex").append(std::to_string(l)).append(";\n");
        }

        vs.append("void main() {\n");
        vs.append("gl_Position = u_projModelView * a_position;\n");
        if(hasColors)
            vs.append("v_col = a_color;\n");
        for (int l = 0; l < numTexCoords; ++l) {
            vs.append("v_tex").append(std::to_string(l)).append("= a_texCoord").append(std::to_string(l)).append(";\n");
        }

        vs.append("gl_PointSize = 1.0;\n");
        vs.append("}\n");
    }

    // fragment shader
    {
        fs.append("#ifdef GL_ES\n");
        fs.append("precision mediump float;\n");
        fs.append("#endif\n");

        if(hasColors)
            fs.append("varying vec4 v_col;\n");

        for (int i = 0; i < numTexCoords; i++) {
            fs.append(arc::Format("varying vec2 v_tex{0};\n", i));
            fs.append(arc::Format("uniform sampler2D u_sampler{0};\n", i));
        }
        fs.append("void main() {\n");
        fs.append("gl_FragColor = ");
        if(hasColors)
            fs.append("v_col");
        else
            fs.append("vec4(1, 1, 1, 1)");

        if(numTexCoords > 0)
            fs.append(" * ");

        for (int i = 0; i < numTexCoords; i++) {
            if(i == numTexCoords - 1)
                fs.append(arc::Format(" texture2D(u_sampler{0},  v_tex{1})", i, i)); // todo: format: test if i can use multiple times same index
            else
                fs.append(arc::Format(" texture2D(u_sampler{0},  v_tex{1}) *", i, i));
        }

        fs.append(";\n}");
    }

    // shader
    {
        shader = new arc::ShaderProgram(vs, fs);
        assert(shader->isCompiled());
    }

    // mesh
    {
        std::vector<arc::VertexAttribute> attribs = { arc::VertexAttribute(VertexUsage::Position, 3, "a_position") };
        if(hasNormals)
            attribs.emplace_back(arc::VertexAttribute(VertexUsage::Normal, 3, "a_normal"));
        if(hasColors)
            attribs.emplace_back(arc::VertexAttribute(VertexUsage::ColorPacked, 4, "a_color"));
        for (int i = 0; i < numTexCoords; ++i) {
            attribs.emplace_back(arc::VertexAttribute(VertexUsage::TextureCoordinates, 2, arc::Format("a_texCoord{0}", i)));
        }
        mesh = new arc::Mesh(false, maxVertices, 0, new arc::VertexAttributes(attribs));

        vertices.resize(maxVertices * (mesh->getVertexAttributes()->vertexSize / 4));
        vertexSize = mesh->getVertexAttributes()->vertexSize / 4;

        for (int j = 0; j < mesh->getVertexAttributes()->size(); ++j) {
            auto& attrib = mesh->getVertexAttributes()->get(j);
            if(attrib.usage == VertexUsage::Normal)
                normalOffset = attrib.offset / 4;
            if(attrib.usage == VertexUsage::ColorPacked)
                colorOffset = attrib.offset / 4;
            if(attrib.usage == VertexUsage::TextureCoordinates)
                texCoordOffset = attrib.offset / 4;
        }
    }

    shaderUniformNames.resize(numTexCoords);
    for (int k = 0; k < numTexCoords; ++k) {
        shaderUniformNames[k] = arc::Format("u_sampler{0}", k);
    }
}

arc::ImmediateModeRenderer::~ImmediateModeRenderer() {
    if(ownsShader)
        delete shader;
    delete mesh;
}

void arc::ImmediateModeRenderer::begin(const arc::Mat4& projModelView, int primitiveType) {
    this->projModelView = projModelView;
    this->primitiveType = primitiveType;
}

void arc::ImmediateModeRenderer::flush() {
    if(numVertices == 0)
    {
        return;
    }

    shader->begin();
    shader->setUniformMat4("u_projModelView", projModelView);
    for (int i = 0; i < numTexCoords; i++)
        shader->setUniformi(shaderUniformNames[i], i);
    mesh->setVertices(vertices, 0, vertexIdx);
    mesh->render(shader, primitiveType);
    shader->end();

    numSetTexCoords = 0;
    vertexIdx = 0;
    numVertices = 0;
}

void arc::ImmediateModeRenderer::color(arc::Color& color) {
    vertices[vertexIdx + colorOffset] = color.toFloatBits();
}

void arc::ImmediateModeRenderer::color(float colorBits) {
    vertices[vertexIdx + colorOffset] = colorBits;
}

void arc::ImmediateModeRenderer::texCoord(const arc::Vec2& uv) {
    int idx = vertexIdx + texCoordOffset;
    vertices[idx + numSetTexCoords] = uv.x;
    vertices[idx + numSetTexCoords + 1] = uv.y;
    numSetTexCoords += 2;
}

void arc::ImmediateModeRenderer::normal(const arc::Vec3& normal) {
    int idx = vertexIdx + normalOffset;
    vertices[idx] = normal.x;
    vertices[idx + 1] = normal.y;
    vertices[idx + 2] = normal.z;
}

void arc::ImmediateModeRenderer::vertex(const arc::Vec3& vertex) {
    int idx = vertexIdx;
    vertices[idx] = vertex.x;
    vertices[idx + 1] = vertex.y;
    vertices[idx + 2] = vertex.z;

    numSetTexCoords = 0;
    vertexIdx += vertexSize;
    numVertices++;
}
void arc::ImmediateModeRenderer::vertex(float x, float y, float z) {
    int idx = vertexIdx;
    vertices[idx] = x;
    vertices[idx + 1] = y;
    vertices[idx + 2] = z;

    numSetTexCoords = 0;
    vertexIdx += vertexSize;
    numVertices++;
}

void arc::ImmediateModeRenderer::end() {
    flush();
}

int arc::ImmediateModeRenderer::getNumVertices() {
    return numVertices;
}

int arc::ImmediateModeRenderer::getMaxVertices() {
    return maxVertices;
}
