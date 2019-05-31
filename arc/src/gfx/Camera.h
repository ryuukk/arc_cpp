#pragma once

#include "../math/Mat4.h"
#include "../math/Vec3.h"

namespace arc
{
    class Camera
    {
    public:
        Vec3 position = {0,0,0};
        Vec3 direction = {0, 0, -1};
        Vec3 up = {0, 1, 0};

        Mat4 projection = {};
        Mat4 view = {};
        Mat4 combined = {};
        Mat4 invProjectionView = {};

        float nearr = 1.0f;
        float farr = 100.0f;

        float viewportWidth = 0.0f;
        float viewportHeight = 0.0f;

        virtual void update(bool updateFrustrum = true) = 0;

        void lookAt(float x, float y, float z);

        void normalizeUp();

        void rotate(Vec3 axis, float angle);

        Vec3 project(Vec3 worldCoords);
        Vec3 project(Vec3 worldCoords, float viewportX, float viewportY, float viewportWidth, float viewportHeight);
        Vec3 unproject(Vec3 screenCoords);
        Vec3 unproject(Vec3 screenCoords, float viewportX, float viewportY, float viewportWidth, float viewportHeight);
    };

    class PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera(float fov, float vw, float vh) {
            fieldOfView = fov;
            viewportWidth = vw;
            viewportHeight = vh;
        }

        void update(bool updateFrustrum = true) override;

        float fieldOfView = 67.0f;
    };

    class OrthographicCamera : public Camera
    {
    public:
        OrthographicCamera()
        {
            nearr = 0.0f;
        }
        OrthographicCamera(float vw, float vh)
        {
            viewportWidth = vw;
            viewportHeight = vh;
            nearr = 0.0f;
        }

        void update(bool updateFrustrum = true) override;
        void setToOrtho(float vw, float vh, bool yDown = false);
        float zoom = 1.0f;
    };
}


