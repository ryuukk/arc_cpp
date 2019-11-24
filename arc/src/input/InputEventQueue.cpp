#include <cstdio>
#include "InputEventQueue.h"
#include "../utils/TimeUtils.h"

void arc::InputEventQueue::setProcessor(arc::InputProcessor* processor) {
    _processor = processor;
}

void arc::InputEventQueue::drain() {
    auto& q = _processingQueue;
    if(_processor == nullptr)
    {
        _queue.clear();
        return;
    }

    q.insert(q.end(), _queue.begin(), _queue.end());
    _queue.clear();


    auto* localProcessor = _processor;
    for (int i = 0, n = q.size(); i < n;)
    {

        _currentEventTime = (uint64_t) q[i++] << 32u | q[i++] & 0xFFFFFFFFu;
        switch (q[i++])
        {
            case arc::input::KEY_DOWN:
                localProcessor->keyDown((Keys) q[i++]);
                break;
            case arc::input::KEY_UP:
                localProcessor->keyUp((Keys) q[i++]);
                break;
            case arc::input::KEY_TYPED:
                localProcessor->keyTyped((char) q[i++]);
                break;
            case arc::input::TOUCH_DOWN:
                localProcessor->touchDown(q[i++], q[i++], q[i++], (Buttons) q[i++]);
                break;
            case arc::input::TOUCH_UP:
                localProcessor->touchUp(q[i++], q[i++], q[i++], (Buttons) q[i++]);
                break;
            case arc::input::TOUCH_DRAGGED:
                localProcessor->touchDragged(q[i++], q[i++], q[i++]);
                break;
            case arc::input::MMOUSE_MOVED:
                localProcessor->mouseMoved(q[i++], q[i++]);
                break;
            case arc::input::SCROLLED:
                localProcessor->scrolled(q[i++]);
                break;
        }
    }

    q.clear();
}

bool arc::InputEventQueue::keyDown(arc::Keys keycode) {
    queueTime();
    _queue.emplace_back(arc::input::KEY_DOWN);
    _queue.emplace_back((int)keycode);
    return false;
}

bool arc::InputEventQueue::keyUp(arc::Keys keycode) {
    queueTime();
    _queue.emplace_back(arc::input::KEY_UP);
    _queue.emplace_back((int)keycode);
    return false;
}

bool arc::InputEventQueue::keyTyped(char character) {
    queueTime();
    _queue.emplace_back(arc::input::KEY_TYPED);
    _queue.emplace_back((int) character);

    return false;
}

bool arc::InputEventQueue::touchDown(int screenX, int screenY, int pointer, arc::Buttons button) {
    queueTime();
    _queue.emplace_back(arc::input::TOUCH_DOWN);
    _queue.emplace_back(screenX);
    _queue.emplace_back(screenY);
    _queue.emplace_back(pointer);
    _queue.emplace_back((int) button);
    return false;
}

bool arc::InputEventQueue::touchUp(int screenX, int screenY, int pointer, arc::Buttons button) {
    queueTime();
    _queue.emplace_back(arc::input::TOUCH_UP);
    _queue.emplace_back(screenX);
    _queue.emplace_back(screenY);
    _queue.emplace_back(pointer);
    _queue.emplace_back((int) button);
    return false;
}

bool arc::InputEventQueue::touchDragged(int screenX, int screenY, int pointer) {
    queueTime();
    _queue.emplace_back(arc::input::TOUCH_DRAGGED);
    _queue.emplace_back(screenX);
    _queue.emplace_back(screenY);
    _queue.emplace_back(pointer);
    return false;
}

bool arc::InputEventQueue::mouseMoved(int screenX, int screenY) {
    queueTime();
    _queue.emplace_back(arc::input::MMOUSE_MOVED);
    _queue.emplace_back(screenX);
    _queue.emplace_back(screenY);
    return false;
}

bool arc::InputEventQueue::scrolled(int amount) {
    queueTime();
    _queue.emplace_back(arc::input::SCROLLED);
    _queue.emplace_back(amount);
    return false;
}

uint64_t arc::InputEventQueue::getCurrentEventTime() {
    return _currentEventTime;
}

void arc::InputEventQueue::queueTime() {
    auto time = arc::times::nanoseconds();
    _queue.emplace_back((int) (time >> 32u));
    _queue.emplace_back((int) time);
}
