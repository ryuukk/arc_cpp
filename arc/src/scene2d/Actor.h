#pragma once

#include <string>
#include "Touchable.h"
#include "../Color.h"
#include "../gfx/SpriteBatch.h"
#include "../gfx/ShapeRenderer.h"

namespace arc
{
    class Group;
    class Stage;
    class Actor
    {
    public:
        Actor();
        virtual ~Actor();
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

        virtual void setBounds(float x, float y, float width, float height);

        virtual void drawDebug(ShapeRenderer* shapes);

        Stage* getStage();
        virtual void setStage(Stage* stage);

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

        void setDebug(bool enabled);
        bool getDebug();

        void setSize(float width, float height);

        float getX();
        void setX(float x);
        void setX(float x, int align);
        float getY();
        void setY(float y);
        void setY(float y, int align);

        float getWidth();
        float getHeight();

        float getOriginX();
        float getOriginY();

        float getRotation();
        float getScaleX();
        float getScaleY();

        void toFront();
        void toBack();
        bool setZIndex(int index);
        int getZIndex();

        bool clipBegin();
        bool clipBegin(float x, float y, float width, float height);
        void clipEnd();

        Color& getColor();
        Touchable getTouchable();

        Vec2 parentToLocalCoordinates(const Vec2& parentCoords);

    protected:
        void positionChanged();
        void sizeChanged();
        void rotationChanged();
        void drawDebugBounds(ShapeRenderer* shapes);
    };
}


