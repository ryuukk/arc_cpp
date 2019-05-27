#include "Core.h"
#include "utils/HdpiUtils.h"

arc::Graphics* arc::Core::graphics = nullptr;
arc::Input* arc::Core::input = nullptr;
arc::IApp* arc::Core::app = nullptr;
arc::Logger* arc::Core::logger = nullptr;

bool arc::Core::pushScissors(const arc::Rect& scissor) {
    auto fixed = scissor;
    fix(fixed);

    if(_scissors.empty())
    {
        if(fixed.width < 1 || fixed.height < 1) return false;
        glEnable(GL_SCISSOR_TEST);
    } else
    {
        auto& parent = _scissors[_scissors.size() - 1];
        float minX = Mathf::max(parent.x, fixed.x);
        float maxX = Mathf::min(parent.x + parent.width, fixed.x + fixed.width);
        if (maxX - minX < 1) return false;

        float minY = Mathf::max(parent.y, fixed.y);
        float maxY = Mathf::min(parent.y + parent.height, fixed.y + fixed.height);
        if (maxY - minY < 1) return false;

        fixed.x = minX;
        fixed.y = minY;
        fixed.width = maxX - minX;
        fixed.height = Mathf::max(1.0f, maxY - minY);
    }
    _scissors.emplace_back(fixed);

    arc::hdpi::glScissorr((int)fixed.x, (int)fixed.y, (int)fixed.width, (int)fixed.height);
    return true;
}

arc::Rect arc::Core::calculateScissors(arc::Camera* camera, float viewportX, float viewportY, float viewportWidth,
                                       float viewportHeight, const arc::Mat4& batchTransform, const arc::Rect& area) {

    Vec3 tmp {area.x, area.y, 0.0f};

    // todo: finish
    //    - vec3 stuff
    //    - camera stuff

    return arc::Rect();
}

void arc::Core::fix(arc::Rect& rect) {
    rect.x = Mathf::round(rect.x);
    rect.y = Mathf::round(rect.y);
    rect.width = Mathf::round(rect.width);
    rect.height = Mathf::round(rect.height);
    if (rect.width < 0) {
        rect.width = -rect.width;
        rect.x -= rect.width;
    }
    if (rect.height < 0) {
        rect.height = -rect.height;
        rect.y -= rect.height;
    }
}
