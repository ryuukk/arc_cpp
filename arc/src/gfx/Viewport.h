#pragma once

#include "Camera.h"
#include "Scaling.h"
#include "../math/Rect.h"

namespace arc
{
    class Viewport
    {
    public:
        void apply(bool centerCamera = false);
        virtual void update(int screenWidth, int screenHeight, bool centerCamera = false);
        void setScreenBounds(int screenX, int screenY, int screenWidth, int screenHeight);
        void setScreenSize(int screenWidth, int screenHeight);
        void setScreenPosition(int screenX, int screenY);
        void setWorldSize (float worldWidth, float worldHeight);
        void setCamera(Camera* camera);
        Camera* getCamera();
        float getWorldWidth();
        float getWorldHeight();
        Rect calculateScissors(const Mat4& batchTransform, const Rect& area);

    private:
        Camera* _camera = nullptr;
        float _worldWidth{};
        float _worldHeight{};
        int _screenX{};
        int _screenY{};
        int _screenWidth{};
        int _screenHeight{};
    };

    class ScreenViewport : public Viewport
    {
    public:
        void update(int screenWidth, int screenHeight, bool centerCamera = false) override;
    private:
        float _unitsPerPixel = 1;
    };

    class ScalingViewport : public Viewport
    {
    public:
        ScalingViewport(Scaling scaling, float worldWidth, float worldHeight, Camera* camera);
        void update(int screenWidth, int screenHeight, bool centerCamera = false) override;
    private:
        Scaling _scaling{};
    };
}