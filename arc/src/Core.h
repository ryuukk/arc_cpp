#pragma once

#include "math/Rect.h"
#include "Logger.h"
#include "Input.h"
#include "Graphics.h"
#include "gfx/Camera.h"

namespace arc
{
    class Graphics;
    class Input;
    class IApp;
    class Core
    {
    public:
        static Graphics* graphics;
        static Input* input;
        static IApp* app;
        static Logger* logger;

    public:
        static bool pushScissors(const Rect& scissor);
        static Rect calculateScissors(Camera* camera, float viewportX, float viewportY, float viewportWidth, float viewportHeight, const Mat4& batchTransform, const Rect& area);

    private:
        static void fix(Rect& rect);

    private:
        static Rect _viewport;
        static std::vector<Rect> _scissors;
    };

    class IApp
    {
    public:
        virtual void create() = 0;

        virtual void update(float dt) = 0;

        virtual void render(float dt) = 0;

        virtual void resize(int width, int height) = 0;

        virtual void dispose() = 0;
    };
}