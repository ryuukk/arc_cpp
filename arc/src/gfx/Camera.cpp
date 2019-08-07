#include <cmath>
#include "Camera.h"
#include "../Core.h"

void arc::Camera::lookAt(float x, float y, float z) {
    Vec3 a(x, y, z);
    Vec3 tmp = (a - position).nor();

    if (!tmp.isZero()) {
        float dot = tmp.dot(up);

        if (std::fabs(dot - 1.0f) < 0.000000001f) {
            up = direction * -1;
        } else if (std::fabs(dot + 1) < 0.000000001f) {
            up = direction;
        }
        direction = tmp;
        normalizeUp();
    }
}

void arc::Camera::normalizeUp() {
    Vec3 tmpVec = direction.crs(up).nor();
    up = tmpVec.crs(direction).nor();
}

void arc::Camera::rotate(Vec3 axis, float angle) {
    // todo: test
    direction = Vec3::rotate(direction, axis, angle);
    up = Vec3::rotate(up, axis, angle);
}

arc::Vec3 arc::Camera::project(arc::Vec3 worldCoords)
{
    return project(worldCoords, 0, 0, Core::graphics->getWidth(), Core::graphics->getHeight());
}

arc::Vec3 arc::Camera::project(arc::Vec3 worldCoords, float viewportX, float viewportY, float viewportWidth,
                               float viewportHeight) {
    // todo: test
    Vec3 ret = Vec3::project(worldCoords, combined);
    ret.x = viewportWidth * (ret.x + 1) / 2 + viewportX;
    ret.y = viewportHeight * (ret.y + 1) / 2 + viewportY;
    ret.z = (ret.z + 1) / 2;
    return ret;
}

arc::Vec3 arc::Camera::unproject(arc::Vec3 screenCoords)
{
    return unproject(screenCoords, 0, 0, Core::graphics->getWidth(), Core::graphics->getHeight());
}

arc::Vec3 arc::Camera::unproject(arc::Vec3 screenCoords, float viewportX, float viewportY, float viewportWidth,
                                 float viewportHeight) {
    // todo: test
    float x = screenCoords.x;
    float y = screenCoords.y;
    x = x - viewportX;
    y = arc::Core::graphics->getHeight() - y - 1;
    y = y - viewportY;

    Vec3 ret;
    ret.x = (2 * x) / viewportWidth - 1;
    ret.y = (2 * y) / viewportHeight - 1;
    ret.z = 2 * screenCoords.z - 1;

    ret = Vec3::project(ret, invProjectionView);
    return ret;
}

void arc::PerspectiveCamera::update(bool updateFrustrum) {

    float aspect = viewportWidth / viewportHeight;
    projection = Mat4::createProjection(std::fabs(nearr), std::fabs(farr), fieldOfView, aspect);
    view = Mat4::createLookAt(position, position + direction, up);
    combined = projection * view;
}

void arc::OrthographicCamera::update(bool updateFrustrum) {

    projection = Mat4::createOrthographic(zoom * -viewportWidth / 2, zoom * (viewportWidth / 2), zoom * -(viewportHeight / 2),
                                         zoom * viewportHeight / 2, nearr, farr);

    view = Mat4::createLookAt(position, position + direction, up);

    combined = projection * view;

    if (updateFrustrum)
    {
        // todo: finish

    }
}

void arc::OrthographicCamera::setToOrtho(float vw, float vh, bool yDown) {
    if (yDown)
    {
        up = {0, -1, 0};
        direction = {0, 0, 1};
    }
    else
    {
        up = {0, 1, 0};
        direction = {0, 0, -1};
    }
    position = Vec3(zoom * vw / 2.0f, zoom * vh / 2.0f, 0);
    viewportWidth = vw;
    viewportHeight = vh;
    update();
}
