#pragma once

#include <string>
#include "Stage.h"
#include "Group.h"
#include "Touchable.hpp"
#include "../Color.h"
#include "../gfx/SpriteBatch.h"

namespace arc
{
    class Actor
    {
    private:
        Stage* _stage = nullptr;
        Group* _parent = nullptr;
        std::string _name{};
        Touchable _touchable = Touchable::enabled;
        bool _visible = true;
        bool _debug{};
        float _x{};
        float _y{};
        float _width{};
        float _height{};
        float _originX{};
        float _originY{};
        float _scaleX = 1.0f;
        float _scaleY = 1.0f;
        float _rotation{};
        Color _color = Color::WHITE;

    public:
        virtual void draw(SpriteBatch* batch, float parentAlpha);
        virtual void act(float dt);
        virtual Actor* hit(float x, float y, bool touchable);
        virtual bool remove();
        virtual void clear();

        Stage* getStage();
        void setStage(Stage* stage);

        bool isDescendantOf(Actor* actor);
        bool isAscendantOf(Actor* actor);

        bool hasParent();
        Group* getParent();
        void setParent(Group* parent);

        bool isTouchable();
        void setTouchable(Touchable touchable);
        bool isVisible();
        void setVisible(bool visible);
        bool ancestorsVisible();
        bool hasKeyboardFocus();
        bool hasScrollFocus();
        bool isTouchFocusTarget();

        float getX();
        void setX(float x);
        void setX(float x, int align);
        float getY();
        void setY(float Y);
        void setY(float Y, int align);

        void toFront();
        void toBack();
        bool setZIndex(int index);
        int getZIndex();

        bool clipBegin(float x, float y, float width, float height);
        void clipEnd();

    protected:
        void positionChanged();
        void sizeChanged();
        void rotationChanged();
    };
}


