#pragma once

#include "../math/Affine2.h"
#include "../math/Mat4.h"
#include "../math/Rect.h"
#include "Actor.h"

namespace arc
{
    class Group : public Actor
    {
    public:
        void act(float dt);
        void draw(SpriteBatch* batch, float parentAlpha);
        void drawDebug(ShapeRenderer* shapes);
    protected:
        void drawChildren(SpriteBatch* batch, float parentAlpha);
        void drawDebugChildren(ShapeRenderer* shapes);

    private:
        Affine2 _worldTransform{};
        Mat4 _computedTransform{};
        Mat4 _oldTransform{};
        bool _transform = true;
        Rect _cullingArea{};
    };
}


