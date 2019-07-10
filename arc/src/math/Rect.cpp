#include "Rect.h"

bool arc::Rect::isZero() {
    return x == 0.0f && y == 0.0f && width == 0.0f && height == 0.0f;
}
