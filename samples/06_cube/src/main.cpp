#include <fstream>
#include <string.h>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

#include <gfx/ShaderProgram.h>
#include <gfx/Mesh.h>
#include <gfx/Camera.h>
#include <utils/CameraController.h>


class Entity
{
public:
    ~Entity() {
    }

    arc::Mesh* mesh = nullptr;
    arc::Vec3 position = {0, 0, 0};
    arc::Mat4 _transform = arc::Mat4::identity();
    float _a = 5.0f;


    void update(const float dt) {
        _a += dt;
        _transform.set(position, arc::Quat::fromAxis({0, 1, 0}, _a));
    }

    void render(arc::ShaderProgram* shader) {
        shader->setUniformMat4("u_world", _transform);
        mesh->render(shader, GL_TRIANGLES);
    }
};

class MyGame : public arc::IApp
{
    std::string vs = R"(
#version 330

in vec3 a_position;
in vec3 a_normal;

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_world;

out vec3 v_normal;

void main()
{
    gl_Position = u_proj * u_view * u_world * vec4(a_position, 1.0);

    v_normal = a_normal;
}
)";

    std::string fs = R"(
#version 330

in vec3 v_normal;

out vec4 f_color;

void main()
{
	f_color = vec4(1.0, 1.0, 1.0, 1.0) * vec4(v_normal, 1.0);
}
)";

    arc::CameraController* _camController;
    arc::ShaderProgram *_shader;
    arc::PerspectiveCamera *_cam;
    std::vector<Entity*> _entities{};

    void create() override {

        std::vector<float> positions = {
                -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
                -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,
                0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
                0.5f, 0.5f, -0.5f
        };

        std::vector<float> normals = {
                0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                0.0f
        };
        std::vector<float> vertices;
        vertices.resize(24 * 6);

        int pIdx = 0;
        int nIdx = 0;
        //int tIdx = 0;

        for (int i = 0; i < vertices.size();)
        {
            vertices[i++] = positions[pIdx++];
            vertices[i++] = positions[pIdx++];
            vertices[i++] = positions[pIdx++];
            vertices[i++] = normals[nIdx++];
            vertices[i++] = normals[nIdx++];
            vertices[i++] = normals[nIdx++];
            //vertices[i++] = cubeTex[tIdx++];
            //vertices[i++] = cubeTex[tIdx++];
        }

        std::vector<short> indices = {
                0, 2, 1, 0, 3, 2, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 15, 14, 12, 14, 13, 16, 17, 18, 16,
                18, 19, 20, 23, 22, 20, 22, 21
        };

        auto s = 8;

        for (int j = -s; j < s; ++j) {
            for (int i = -s; i < s; ++i) {
                auto* e = new Entity();

                auto attrPos = arc::VertexAttribute(arc::VertexUsage::Position, 3, "a_position");
                auto attrNormal = arc::VertexAttribute(arc::VertexUsage::Normal, 3, "a_normal");

                e->position = {j * 2.0f, 0, i * 2.0f};
                e->mesh = new arc::Mesh(true, 24, 36, new arc::VertexAttributes({attrPos, attrNormal}));
                e->mesh->setVertices(vertices);
                e->mesh->setIndices(indices);

                _entities.emplace_back(e);
            }
        }

        printf("Added: %d cubes\n", _entities.size());

        _shader = new arc::ShaderProgram(vs, fs);

        _cam = new arc::PerspectiveCamera(67, arc::Core::graphics->getWidth(), arc::Core::graphics->getHeight());
        //_cam->position = arc::Vec3(0, 10, 5) * 5.0f;
        //_cam->lookAt(0, 0, 0);
        _cam->update();

        _camController = new arc::CameraController(_cam);

        arc::Core::input->setInputProcessor(_camController);
        printf("Shader Log: %s\n", _shader->log.c_str());
    }

    void update(float dt) override {
        _camController->update(dt);
        for (auto& entity : _entities)
            entity->update(dt);
        _cam->update();
    }

    void render(float dt) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glEnable(GL_DEPTH_TEST);

        _shader->begin();
        _shader->setUniformMat4("u_proj", _cam->projection);
        _shader->setUniformMat4("u_view", _cam->view);

        for (auto& entity : _entities)
            entity->render(_shader);

        _shader->end();
    }

    void resize(int width, int height) override {

    }

    void dispose() override {
        delete _shader;
    }

};

int main(int argc, char **argv) {
    auto config = arc::Configuration();
    config.windowTitle = "Sample 06 - Cube";
    auto myGame = new MyGame();
    auto engine = new arc::Engine(myGame, config);
    engine->run();

    return 0;
}