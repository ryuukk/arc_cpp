#include "Vec3.h"

arc::Vec3 arc::Vec3::transform(arc::Vec3 lhs, arc::Mat4 matrix) {    // todo: that is how mat4_proj is done, maybe should move there ?
    // or maybe this is not how to do it ?
    // works for now, dunno
    float inv_w = 1.0f / (lhs.x * matrix.m30 + lhs.y * matrix.m31 + lhs.z * matrix.m32 + matrix.m33);
    Vec3 ret;
    ret.x = (lhs.x * matrix.m00 + lhs.y * matrix.m01 + lhs.z * matrix.m02 + matrix.m03) * inv_w;
    ret.y = (lhs.x * matrix.m10 + lhs.y * matrix.m11 + lhs.z * matrix.m12 + matrix.m13) * inv_w;
    ret.z = (lhs.x * matrix.m20 + lhs.y * matrix.m21 + lhs.z * matrix.m22 + matrix.m23) * inv_w;
    return ret;
}

arc::Vec3 arc::Vec3::rotate(arc::Vec3 lhs, arc::Vec3 axis, float angle) {
    auto rotation = Quat::fromAxis(axis, angle);
    auto matrix = Mat4::identity();
    matrix.set(0.0f, 0.0f, 0.0f, rotation.x, rotation.y, rotation.z, rotation.w);
    return transform(lhs, matrix);
}

arc::Vec3 arc::Vec3::project(arc::Vec3 lhs, arc::Mat4 matrix)
{
    float l_w = 1.0f / (
            lhs.x * matrix.m30 +
            lhs.y * matrix.m31 +
            lhs.z * matrix.m32 +
            matrix.m33);

    Vec3 ret = {
            (lhs.x * matrix.m00 + lhs.y * matrix.m01 + lhs.z * matrix.m02 + matrix.m03) * l_w,
            (lhs.x * matrix.m10 + lhs.y * matrix.m11 + lhs.z * matrix.m12 + matrix.m13) * l_w,
            (lhs.x * matrix.m20 + lhs.y * matrix.m21 + lhs.z * matrix.m22 + matrix.m23) * l_w
    };

    return ret;
}
