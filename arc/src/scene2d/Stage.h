#pragma once

#include <array>
#include "../input/InputAdapter.h"
#include "../gfx/Viewport.h"
#include "../gfx/SpriteBatch.h"
#include "../gfx/ShapeRenderer.h"
#include "Group.h"
namespace arc
{
    enum class Debug
    {
        none
    };

    struct TouchFocus
    {
    public:
        // listener
        Actor* listenerActor;
        Actor* target;
        int pointer;
        int button;
    };

    class Actor;
    class Group;
    class Stage : public InputAdapter
    {
    public:
        static bool debug;

    public:
        Stage(Viewport* viewport = nullptr);
        ~Stage();
        void render(float dt);
        void act(float dt);

        void addActor(Actor* actor);

        void unfocus(Actor* actor);
        Actor* getKeyboardFocus();
        Actor* getScrollFocus();
        std::vector<TouchFocus>& getTouchFocuses();
        Rect calculateScissors(const Rect& local);
        Group& getRoot();
        float getWidth();
        float getHeight();
    private:
        Viewport* _viewport = nullptr;
        SpriteBatch* _batch = nullptr;
        bool _ownsBatch{};
        Group _root;
        std::array<Actor*, 20> _pointerOverActors{};
        bool _pointerTouched[20]{};
        int _pointerScreenX[20]{};
        int _pointerScreenY[20]{};

        int _mouseScreenX{};
        int _mouseScreenY{};

        Actor* _mouseOverActor = nullptr;
        Actor* _keyboardFocus = nullptr;
        Actor* _scrollFocus = nullptr;

        std::vector<TouchFocus> _touchFocuses;

        bool _actionsRequestRendering = true;

        ShapeRenderer _debugShapes{};

        bool debugInvisible{}, debugAll{}, debugUnderMouse{}, debugParentUnderMouse{};
        Debug debugTableUnderMouse = Debug::none;
        Color debugColor = Color(0,1.0f,0,1.0f);

    };
}