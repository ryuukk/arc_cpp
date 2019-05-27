#include "CameraController.h"
#include "../Core.h"

void arc::CameraController::update(float dt) {

    if (_forward)
    {
        _camera->position += _camera->direction.nor() * (_velocity * dt);
    }

    if (_backward)
    {
        _camera->position += _camera->direction.nor() * -(_velocity * dt);
    }

    if (_strafeLeft)
    {
        _camera->position += Vec3::cross(_camera->direction, _camera->up).nor() * -(_velocity * dt);
    }

    if (_strafeRight)
    {
        _camera->position += Vec3::cross(_camera->direction, _camera->up).nor() * (_velocity * dt);
    }

    if (_up)
    {
        _camera->position += _camera->up.nor() * (_velocity * dt);
    }

    if (_down)
    {
        _camera->position += _camera->up.nor() * -(_velocity * dt);
    }

    _camera->update(true);
}

bool arc::CameraController::keyDown(arc::Keys keycode) {
    switch (keycode)
    {
        case STRAFE_LEFT:
            _strafeLeft = true;
            break;
        case STRAFE_RIGHT:
            _strafeRight = true;
            break;

        case FORWARD:
            _forward = true;
            break;
        case BACKWARD:
            _backward = true;
            break;
        case UP:
            _up = true;
            break;
        case DOWN:
            _down = true;
            break;
    }

    return true;
}

bool arc::CameraController::keyUp(arc::Keys keycode) {
    switch (keycode)
    {
        case STRAFE_LEFT:
            _strafeLeft = false;
            break;
        case STRAFE_RIGHT:
            _strafeRight = false;
            break;

        case FORWARD:
            _forward = false;
            break;
        case BACKWARD:
            _backward = false;
            break;
        case UP:
            _up = false;
            break;
        case DOWN:
            _down = false;
            break;
    }

    return true;
}

bool arc::CameraController::touchDragged(int screenX, int screenY, int pointer)
{
    float deltaX = -arc::Core::input->getDeltaX() * _degreesPerPixel;
    float deltaY = -arc::Core::input->getDeltaY() * _degreesPerPixel;

    _camera->direction = Vec3::rotate(_camera->direction, _camera->up, deltaX);

    auto tmp = Vec3::cross(_camera->direction, _camera->up).nor();
    _camera->direction = Vec3::rotate(_camera->direction, tmp, deltaY);
    return true;
}
