#include "Affine2.h"

arc::Affine2& arc::Affine2::idt() {
    m00 = 1;
    m01 = 0;
    m02 = 0;
    m10 = 0;
    m11 = 1;
    m12 = 0;
    return *this;
}
