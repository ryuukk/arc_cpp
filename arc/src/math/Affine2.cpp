#include "Affine2.h"
#include "Mathf.h"

arc::Affine2& arc::Affine2::idt() {
    m00 = 1;
    m01 = 0;
    m02 = 0;
    m10 = 0;
    m11 = 1;
    m12 = 0;
    return *this;
}

arc::Affine2& arc::Affine2::setToTrnRotScl(float x, float y, float degrees, float scaleX, float scaleY) {
    m02 = x;
    m12 = y;

    if (degrees == 0) {
        m00 = scaleX;
        m01 = 0;
        m10 = 0;
        m11 = scaleY;
    } else {
        float sin = arc::Mathf::sinDeg(degrees);
        float cos = arc::Mathf::cosDeg(degrees);

        m00 = cos * scaleX;
        m01 = -sin * scaleY;
        m10 = sin * scaleX;
        m11 = cos * scaleY;
    }
    return *this;
}

arc::Affine2& arc::Affine2::translate(float x, float y) {
    m02 += m00 * x + m01 * y;
    m12 += m10 * x + m11 * y;
    return *this;
}

arc::Affine2& arc::Affine2::preMul(const arc::Affine2& other) {
    float tmp00 = other.m00 * m00 + other.m01 * m10;
    float tmp01 = other.m00 * m01 + other.m01 * m11;
    float tmp02 = other.m00 * m02 + other.m01 * m12 + other.m02;
    float tmp10 = other.m10 * m00 + other.m11 * m10;
    float tmp11 = other.m10 * m01 + other.m11 * m11;
    float tmp12 = other.m10 * m02 + other.m11 * m12 + other.m12;

    m00 = tmp00;
    m01 = tmp01;
    m02 = tmp02;
    m10 = tmp10;
    m11 = tmp11;
    m12 = tmp12;
    return *this;
}
