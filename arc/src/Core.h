#pragma once

#include "Graphics.h"
#include "Input.h"

namespace arc {

    class Graphics;
    class Input;
    class IApp;
    class Core
    {
    public:
        static Graphics *graphics;
        static Input *input;
        static arc::IApp *app;
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