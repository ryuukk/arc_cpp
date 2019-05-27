#pragma once

#include "../input/InputAdapter.h"
#include "../gfx/Camera.h"

namespace arc
{
    class CameraController : public InputAdapter
    {
    public:
        CameraController(Camera* camera) : _camera(camera) {}
        void update(float dt);
    private:
        bool keyDown(Keys keycode) override;

        bool keyUp(Keys keycode) override;

        bool touchDragged(int screenX, int screenY, int pointer) override;

    private:
        Camera* _camera = nullptr;
        static const Keys STRAFE_LEFT = Keys::A;
        static const Keys STRAFE_RIGHT = Keys::D;
        static const Keys FORWARD = Keys::W;
        static const Keys BACKWARD = Keys::S;
        static const Keys UP = Keys::Q;
        static const Keys DOWN = Keys::E;
        bool _strafeLeft = false;
        bool _strafeRight = false;
        bool _forward = false;
        bool _backward = false;
        bool _up = false;
        bool _down = false;
        float _velocity = 10;
        float _degreesPerPixel = 0.005f;
    };
}


