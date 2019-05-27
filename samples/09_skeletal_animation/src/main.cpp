#include <fstream>
#include <string.h>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

#include <gfx/ShaderProgram.h>
#include <gfx/Mesh.h>
#include <gfx/Camera.h>
#include <gfx/Model.h>
#include <gfx/ModelInstance.h>
#include <gfx/animation/AnimationController.h>
#include <gfx/data/ModelData.h>
#include <utils/CameraController.h>

class MyGame : public arc::IApp
{
    std::string vs = R"(
#version 330

in vec3 a_position;
in vec3 a_normal;
in vec4 a_color;
in vec2 a_texCoord0;
in vec2 a_boneWeight0;
in vec2 a_boneWeight1;
in vec2 a_boneWeight2;
in vec2 a_boneWeight3;

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_world;

uniform mat4 u_bones[20];

out vec4 v_color;
out vec2 v_texCoord;

void main()
{

	mat4 skinning = mat4(0.0);
	skinning += (a_boneWeight0.y) * u_bones[int(a_boneWeight0.x)];
	skinning += (a_boneWeight1.y) * u_bones[int(a_boneWeight1.x)];
	skinning += (a_boneWeight2.y) * u_bones[int(a_boneWeight2.x)];
	skinning += (a_boneWeight3.y) * u_bones[int(a_boneWeight3.x)];


    vec4 pos = u_world * skinning * vec4(a_position, 1.0);
    gl_Position = u_proj * u_view * pos;

    v_color = a_color;
    v_texCoord = a_texCoord0;
}
)";

    std::string fs = R"(
#version 330

in vec4 v_color;
in vec2 v_texCoord;

uniform sampler2D u_texture;

out vec4 f_color;

void main()
{
    vec3 color = texture2D(u_texture, v_texCoord).rgb;
    f_color = vec4(color, 1.0) * v_color;
    //f_color = v_color;
}
)";

    arc::CameraController* _camController;
    arc::ShaderProgram* _shader;
    arc::PerspectiveCamera* _cam;
    arc::Model* _model;
    arc::ModelInstance* _instance;
    arc::AnimationController* _animController;
    arc::Mat4 _transform;

    float _a = 0.0f;
    float _timerChangeAnim = 0.0f;
    int _selector = 0;

    void create() override {

        _shader = new arc::ShaderProgram(vs, fs);

        _cam = new arc::PerspectiveCamera(67, arc::Core::graphics->getWidth(), arc::Core::graphics->getHeight());
        _cam->position = arc::Vec3(0, 0, 10);
        _cam->update();

        _camController = new arc::CameraController(_cam);
        arc::Core::input->setInputProcessor(_camController);

        printf("Shader Log  : %s\n", _shader->log.c_str());


        auto modelData = arc::ModelData::load("data/models/knight.g3dj");
        _model = new arc::Model(modelData);
        _instance = new arc::ModelInstance(*_model);
        _animController = new arc::AnimationController(*_instance);
        _animController->animate("Attack");
    }

    void update(float dt) override {
        _camController->update(dt);
        _cam->update();

        _a += dt;
        _transform.set({0, 0, 0}, arc::Quat::fromAxis({0.5f, 1, -0.5f}, _a));

        _timerChangeAnim -=dt;
        if(_timerChangeAnim <= 0.0f && !_instance->animations.empty())
        {
            auto& animation =_instance->animations[_selector];
            //_animController->animate(animation->id);

            _selector = (++_selector % _instance->animations.size());
            _timerChangeAnim = 2.0;
        }

        _animController->update(dt);
        _instance->calculateTransforms();
    }

    void render(float dt) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glEnable(GL_DEPTH_TEST);

        _shader->begin();
        _shader->setUniformMat4("u_proj", _cam->projection);
        _shader->setUniformMat4("u_view", _cam->view);

        for(auto& node : _instance->nodes)
        {
           renderNode(node);
        }

        _shader->end();
    }

    void renderNode(arc::Node* node)
    {
        auto transform = _transform * node->globalTransform;
        _shader->setUniformMat4("u_world", transform);
        for(auto& part : node->parts)
        {
            _shader->setUniformMat4Array("u_bones", part->bones.size(), part->bones);

            if(part->material->has(arc::DiffuseTextureAttribute::stype))
            {
                auto* ta = part->material->get<arc::DiffuseTextureAttribute>(arc::DiffuseTextureAttribute::stype);
                ta->descriptor.texture->bind();

                _shader->setUniformi("u_texture", 0);
            }

            part->meshPart->render(_shader, true);
        }

        for(auto& child : node->children)
            renderNode(child);
    }

    void resize(int width, int height) override {

    }

    void dispose() override {
        delete _shader;
    }

};


int main(int argc, char** argv) {
    auto config = arc::Configuration();
    config.windowTitle = "Sample 09 - Skeletal Animation";
    auto myGame = new MyGame();
    auto engine = new arc::Engine(myGame, config);
    engine->run();

    return 0;
}