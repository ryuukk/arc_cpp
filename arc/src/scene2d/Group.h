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
        Group();
        ~Group() override;

        void act(float dt) override;
        void draw(SpriteBatch* batch, float parentAlpha) override;
        void drawDebug(ShapeRenderer* shapes) override;
        void setCullingArea(const Rect& area) override;
        Actor* hit(float x, float y, bool touchable) override;
        void addActor(Actor* actor);
        void addActorAt(int index, Actor* actor);
        bool removeActor(Actor* actor, bool unfocus = true);
        virtual void clearChildren();
        void clear() override;
        Actor* findActor(const std::string& name);
        Vec2 localToDescendantCoordinates(Actor* descendant);
        void setDebug(bool enabled, bool recursively);
        bool isTransform();
        void setTransform(bool transform);
        std::vector<Actor*>& getChildren();
        void setStage(Stage* stage) override;
    protected:
        void drawChildren(SpriteBatch* batch, float parentAlpha);
        void drawDebugChildren(ShapeRenderer* shapes);
        Mat4 computeTransform();
        void applyTransform(SpriteBatch* batch, const Mat4& transform);
        void resetTransform(SpriteBatch* batch);
        void applyTransform(ShapeRenderer* shapes, const Mat4& transform);
        void resetTransform(ShapeRenderer* shapes);
        void childrenChanged();

    private:
        std::vector<Actor*> _children;
        Affine2 _worldTransform{};
        Mat4 _computedTransform{};
        Mat4 _oldTransform{};
        bool _transform = true;
        Rect _cullingArea{};
    };
}


