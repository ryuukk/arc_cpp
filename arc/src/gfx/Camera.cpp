#include <cmath>

#include <cmath>

#include "Camera.h"

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
