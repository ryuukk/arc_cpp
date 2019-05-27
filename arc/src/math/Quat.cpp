#include "Quat.h"

arc::Quat arc::Quat::fromAxis(arc::Vec3 axis, float rad) {
    float d = Vec3::len(axis);

    if (d == 0.f) return Quat::identity();
    d = 1.0f / d;
    float l_ang = rad < 0 ? Mathf::PI2() - (std::fmod(-rad, Mathf::PI2())) : std::fmod(rad, Mathf::PI2());
    float l_sin = std::sin(l_ang / 2);
    float l_cos = std::cos(l_ang / 2);

    return Quat(d * axis.x * l_sin, d * axis.y * l_sin, d * axis.z * l_sin, l_cos).nor();
}
