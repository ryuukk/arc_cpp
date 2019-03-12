#pragma once

namespace arc {
    class Core
    {};

    class IApp
    {
        virtual void create() = 0;
        virtual void update(float dt) = 0;
        virtual void render(float dt) = 0;
        virtual void resize(int width, int height) = 0;
        virtual void dispose() = 0;
    };
}