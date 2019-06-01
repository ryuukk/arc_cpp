#pragma once

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

    class Actor;
    class Group;
    class Stage : public InputAdapter
    {
    public:
        void unfocus(Actor* actor);
    private:
        Viewport* _viewport = nullptr;
        SpriteBatch* _batch = nullptr;
        bool _ownsBatch{};
        Group* _root = nullptr;
        Actor* _pointerOverActors[20]{};
        bool _pointerTouched[20]{};
        int _pointerScreenX[20]{};
        int _pointerScreenY[20]{};

        int _mouseScreenX{};
        int _mouseScreenY{};

        Actor* _mouseOverActor = nullptr;
        Actor* _keyboardFocus = nullptr;
        Actor* _scrollFocus = nullptr;

        bool _actionsRequestRendering = true;

        ShapeRenderer _debugShapes{};

        bool debugInvisible{}, debugAll{}, debugUnderMouse{}, debugParentUnderMouse{};
        Debug debugTableUnderMouse = Debug::none;
        Color debugColor = Color(0,1.0f,0,1.0f);

    };
}