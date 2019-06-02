#pragma once

#include "../math/Affine2.h"
#include "../math/Mat4.h"
#include "../math/Rect.h"
#include "Actor.h"

namespace arc
{
    class ICullable
    {
    public:
        virtual void setCullingArea(const Rect& area) = 0;
    };

    class Group : public Actor, ICullable
    {
    public:
        void act(float dt);
        void draw(SpriteBatch* batch, float parentAlpha);
        void drawDebug(ShapeRenderer* shapes);
        void setCullingArea(const Rect& area) override;
        Actor* hit(float x, float y, bool touchable);
        void addActor(Actor* actor);
        void addActorAt(int index, Actor* actor);
        bool removeActor(Actor* actor, bool unfocus = true);
        void clearChildren();
        void clear();
        Actor* findActor(const std::string& name);
        Vec2 localToDescendantCoordinates(Actor* descendant);
        void setDebug(bool enabled, bool recursively);
        bool isTransform();
    protected:
        void drawChildren(SpriteBatch* batch, float parentAlpha);
        void drawDebugChildren(ShapeRenderer* shapes);
        Mat4 computeTransform();
        void applyTransform(SpriteBatch* batch, const Mat4& transform);
        void resetTransform(SpriteBatch* batch);
        void applyTransform(ShapeRenderer* shapes, const Mat4& transform);
        void resetTransform(ShapeRenderer* shapes);
        void childrenChanged();
        void setStage(Stage* stage);

    private:
        std::vector<Actor*> _children;
        Affine2 _worldTransform{};
        Mat4 _computedTransform{};
        Mat4 _oldTransform{};
        bool _transform = true;
        Rect _cullingArea{};
    };
}


