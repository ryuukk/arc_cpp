#include "ScissorStack.h"
#include "HdpiUtils.h"

arc::Rect arc::ScissorStack::_viewport;
std::vector<arc::Rect> arc::ScissorStack::_scissors;

bool arc::ScissorStack::pushScissors(const arc::Rect& scissor) {
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

arc::Rect arc::ScissorStack::calculateScissors(arc::Camera* camera, const arc::Mat4& batchTransform, const arc::Rect& area) {
    return calculateScissors(camera, 0, 0, arc::Core::graphics->getWidth(), arc::Core::graphics->getHeight(), batchTransform, area);
}

arc::Rect arc::ScissorStack::calculateScissors(arc::Camera* camera, float viewportX, float viewportY, float viewportWidth,
                                       float viewportHeight, const arc::Mat4& batchTransform, const arc::Rect& area) {

    // todo: test
    Vec3 tmp {area.x, area.y, 0.0f};

    tmp = Vec3::transform(tmp, batchTransform);

    tmp = camera->project(tmp, viewportX, viewportY, viewportWidth, viewportHeight);

    Rect scissor;
    scissor.x = tmp.x;
    scissor.y = tmp.y;

    tmp = {area.x + area.width, area.y + area.height, 0};
    tmp = Vec3::transform(tmp, batchTransform);

    tmp = camera->project(tmp, viewportX, viewportY, viewportWidth, viewportHeight);
    scissor.width = tmp.x - scissor.x;
    scissor.height = tmp.y - scissor.y;

    return scissor;
}

void arc::ScissorStack::fix(arc::Rect& rect) {
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

arc::Rect arc::ScissorStack::popScissors() {
    auto old = _scissors[_scissors.size() - 1];
    _scissors.pop_back();
    if(_scissors.empty())
        glDisable(GL_SCISSOR_TEST);
    else
    {
        auto scissor = _scissors[_scissors.size() - 1];
        arc::hdpi::glScissorr((int)scissor.x, (int)scissor.y, (int)scissor.width, (int)scissor.height);
    }
    return old;
}
