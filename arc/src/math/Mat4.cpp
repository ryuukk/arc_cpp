#include "Mat4.h"

arc::Mat4 arc::Mat4::inv(const arc::Mat4& mat) {
    float lDet = mat.m30 * mat.m21 * mat.m12 * mat.m03 - mat.m20 * mat.m31 * mat.m12 * mat.m03 -
                 mat.m30 * mat.m11
                 * mat.m22 * mat.m03 + mat.m10 * mat.m31 * mat.m22 * mat.m03 +
                 mat.m20 * mat.m11 * mat.m32 * mat.m03 - mat.m10
                                                         * mat.m21 * mat.m32 * mat.m03 -
                 mat.m30 * mat.m21 * mat.m02 * mat.m13 + mat.m20 * mat.m31 * mat.m02 * mat.m13
                 + mat.m30 * mat.m01 * mat.m22 * mat.m13 -
                 mat.m00 * mat.m31 * mat.m22 * mat.m13 - mat.m20 * mat.m01 * mat.m32
                                                         * mat.m13 + mat.m00 * mat.m21 * mat.m32 * mat.m13 +
                 mat.m30 * mat.m11 * mat.m02 * mat.m23 -
                 mat.m10 * mat.m31
                 * mat.m02 * mat.m23 - mat.m30 * mat.m01 * mat.m12 * mat.m23 +
                 mat.m00 * mat.m31 * mat.m12 * mat.m23 + mat.m10
                                                         * mat.m01 * mat.m32 * mat.m23 -
                 mat.m00 * mat.m11 * mat.m32 * mat.m23 -
                 mat.m20 * mat.m11 * mat.m02 * mat.m33
                 + mat.m10 * mat.m21 * mat.m02 * mat.m33 + mat.m20 * mat.m01 * mat.m12 * mat.m33 - mat.m00 *
                                                                                                   mat.m21 * mat.m12
                                                                                                   * mat.m33 -
                 mat.m10 * mat.m01 * mat.m22 * mat.m33 + mat.m00 * mat.m11 * mat.m22 * mat.m33;


    if (lDet == 0.0f) throw std::invalid_argument("non-invertible matrix");
    float invDet = 1.0f / lDet;
    Mat4 tmp = Mat4::identity();
    tmp.m00 = mat.m12 * mat.m23 * mat.m31 - mat.m13 * mat.m22 * mat.m31 + mat.m13 * mat.m21 * mat.m32 - mat.m11
                                                                                                        * mat.m23 *
                                                                                                        mat.m32 -
              mat.m12 * mat.m21 * mat.m33 + mat.m11 * mat.m22 * mat.m33;
    tmp.m01 = mat.m03 * mat.m22 * mat.m31 - mat.m02 * mat.m23 * mat.m31 - mat.m03 * mat.m21 * mat.m32 + mat.m01
                                                                                                        * mat.m23 *
                                                                                                        mat.m32 +
              mat.m02 * mat.m21 * mat.m33 - mat.m01 * mat.m22 * mat.m33;
    tmp.m02 = mat.m02 * mat.m13 * mat.m31 - mat.m03 * mat.m12 * mat.m31 + mat.m03 * mat.m11 * mat.m32 - mat.m01
                                                                                                        * mat.m13 *
                                                                                                        mat.m32 -
              mat.m02 * mat.m11 * mat.m33 + mat.m01 * mat.m12 * mat.m33;
    tmp.m03 = mat.m03 * mat.m12 * mat.m21 - mat.m02 * mat.m13 * mat.m21 - mat.m03 * mat.m11 * mat.m22 + mat.m01
                                                                                                        * mat.m13 *
                                                                                                        mat.m22 +
              mat.m02 * mat.m11 * mat.m23 - mat.m01 * mat.m12 * mat.m23;
    tmp.m10 = mat.m13 * mat.m22 * mat.m30 - mat.m12 * mat.m23 * mat.m30 - mat.m13 * mat.m20 * mat.m32 + mat.m10
                                                                                                        * mat.m23 *
                                                                                                        mat.m32 +
              mat.m12 * mat.m20 * mat.m33 - mat.m10 * mat.m22 * mat.m33;
    tmp.m11 = mat.m02 * mat.m23 * mat.m30 - mat.m03 * mat.m22 * mat.m30 + mat.m03 * mat.m20 * mat.m32 - mat.m00
                                                                                                        * mat.m23 *
                                                                                                        mat.m32 -
              mat.m02 * mat.m20 * mat.m33 + mat.m00 * mat.m22 * mat.m33;
    tmp.m12 = mat.m03 * mat.m12 * mat.m30 - mat.m02 * mat.m13 * mat.m30 - mat.m03 * mat.m10 * mat.m32 + mat.m00
                                                                                                        * mat.m13 *
                                                                                                        mat.m32 +
              mat.m02 * mat.m10 * mat.m33 - mat.m00 * mat.m12 * mat.m33;
    tmp.m13 = mat.m02 * mat.m13 * mat.m20 - mat.m03 * mat.m12 * mat.m20 + mat.m03 * mat.m10 * mat.m22 - mat.m00
                                                                                                        * mat.m13 *
                                                                                                        mat.m22 -
              mat.m02 * mat.m10 * mat.m23 + mat.m00 * mat.m12 * mat.m23;
    tmp.m20 = mat.m11 * mat.m23 * mat.m30 - mat.m13 * mat.m21 * mat.m30 + mat.m13 * mat.m20 * mat.m31 - mat.m10
                                                                                                        * mat.m23 *
                                                                                                        mat.m31 -
              mat.m11 * mat.m20 * mat.m33 + mat.m10 * mat.m21 * mat.m33;
    tmp.m21 = mat.m03 * mat.m21 * mat.m30 - mat.m01 * mat.m23 * mat.m30 - mat.m03 * mat.m20 * mat.m31 + mat.m00
                                                                                                        * mat.m23 *
                                                                                                        mat.m31 +
              mat.m01 * mat.m20 * mat.m33 - mat.m00 * mat.m21 * mat.m33;
    tmp.m22 = mat.m01 * mat.m13 * mat.m30 - mat.m03 * mat.m11 * mat.m30 + mat.m03 * mat.m10 * mat.m31 - mat.m00
                                                                                                        * mat.m13 *
                                                                                                        mat.m31 -
              mat.m01 * mat.m10 * mat.m33 + mat.m00 * mat.m11 * mat.m33;
    tmp.m23 = mat.m03 * mat.m11 * mat.m20 - mat.m01 * mat.m13 * mat.m20 - mat.m03 * mat.m10 * mat.m21 + mat.m00
                                                                                                        * mat.m13 *
                                                                                                        mat.m21 +
              mat.m01 * mat.m10 * mat.m23 - mat.m00 * mat.m11 * mat.m23;
    tmp.m30 = mat.m12 * mat.m21 * mat.m30 - mat.m11 * mat.m22 * mat.m30 - mat.m12 * mat.m20 * mat.m31 + mat.m10
                                                                                                        * mat.m22 *
                                                                                                        mat.m31 +
              mat.m11 * mat.m20 * mat.m32 - mat.m10 * mat.m21 * mat.m32;
    tmp.m31 = mat.m01 * mat.m22 * mat.m30 - mat.m02 * mat.m21 * mat.m30 + mat.m02 * mat.m20 * mat.m31 - mat.m00
                                                                                                        * mat.m22 *
                                                                                                        mat.m31 -
              mat.m01 * mat.m20 * mat.m32 + mat.m00 * mat.m21 * mat.m32;
    tmp.m32 = mat.m02 * mat.m11 * mat.m30 - mat.m01 * mat.m12 * mat.m30 - mat.m02 * mat.m10 * mat.m31 + mat.m00
                                                                                                        * mat.m12 *
                                                                                                        mat.m31 +
              mat.m01 * mat.m10 * mat.m32 - mat.m00 * mat.m11 * mat.m32;
    tmp.m33 = mat.m01 * mat.m12 * mat.m20 - mat.m02 * mat.m11 * mat.m20 + mat.m02 * mat.m10 * mat.m21 - mat.m00
                                                                                                        * mat.m12 *
                                                                                                        mat.m21 -
              mat.m01 * mat.m10 * mat.m22 + mat.m00 * mat.m11 * mat.m22;


    tmp.m00 = tmp.m00 * invDet;
    tmp.m01 = tmp.m01 * invDet;
    tmp.m02 = tmp.m02 * invDet;
    tmp.m03 = tmp.m03 * invDet;
    tmp.m10 = tmp.m10 * invDet;
    tmp.m11 = tmp.m11 * invDet;
    tmp.m12 = tmp.m12 * invDet;
    tmp.m13 = tmp.m13 * invDet;
    tmp.m20 = tmp.m20 * invDet;
    tmp.m21 = tmp.m21 * invDet;
    tmp.m22 = tmp.m22 * invDet;
    tmp.m23 = tmp.m23 * invDet;
    tmp.m30 = tmp.m30 * invDet;
    tmp.m31 = tmp.m31 * invDet;
    tmp.m32 = tmp.m32 * invDet;
    tmp.m33 = tmp.m33 * invDet;
    return tmp;
}

arc::Mat4 arc::Mat4::createLookAt(arc::Vec3 position, arc::Vec3 target, arc::Vec3 up) {
    auto tmp = target - position;

    auto ret = createLookAt(tmp, up) * createTranslation(-position);

    return ret;
}

arc::Mat4 arc::Mat4::createLookAt(arc::Vec3 direction, arc::Vec3 up) {
    auto l_vez = direction.nor();
    auto l_vex = direction.nor();

    l_vex = l_vex.crs(up).nor();
    auto l_vey = l_vex.crs(l_vez).nor();

    auto ret = identity();
    ret.m00 = l_vex.x;
    ret.m01 = l_vex.y;
    ret.m02 = l_vex.z;
    ret.m10 = l_vey.x;
    ret.m11 = l_vey.y;
    ret.m12 = l_vey.z;
    ret.m20 = -l_vez.x;
    ret.m21 = -l_vez.y;
    ret.m22 = -l_vez.z;

    return ret;
}

arc::Mat4 arc::Mat4::createTranslation(arc::Vec3 position) {
    auto ret = identity();
    ret.m03 = position.x;
    ret.m13 = position.y;
    ret.m23 = position.z;
    return ret;
}

arc::Mat4 arc::Mat4::createProjection(float nearr, float farr, float fovy, float aspectRatio) {
    auto ret = identity();
    float l_fd = (1.0f / std::tan((fovy * (Mathf::PI() / 180)) / 2.0f));
    float l_a1 = (farr + nearr) / (nearr - farr);
    float l_a2 = (2 * farr * nearr) / (nearr - farr);
    ret.m00 = l_fd / aspectRatio;
    ret.m10 = 0;
    ret.m20 = 0;
    ret.m30 = 0;
    ret.m01 = 0;
    ret.m11 = l_fd;
    ret.m21 = 0;
    ret.m31 = 0;
    ret.m02 = 0;
    ret.m12 = 0;
    ret.m22 = l_a1;
    ret.m32 = -1;
    ret.m03 = 0;
    ret.m13 = 0;
    ret.m23 = l_a2;
    ret.m33 = 0;
    return ret;
}

arc::Mat4 arc::Mat4::createOrthographicOffCenter(float x, float y, float width, float height) {
    return createOrthographic(x, x + width, y, y + height, 0, 1);
}

arc::Mat4 arc::Mat4::createOrthographic(float left, float right, float bottom, float top, float nearr, float farr) {
    auto ret = Mat4::identity();

    float x_orth = 2 / (right - left);
    float y_orth = 2 / (top - bottom);
    float z_orth = -2 / (farr - nearr);

    float tx = -(right + left) / (right - left);
    float ty = -(top + bottom) / (top - bottom);
    float tz = -(farr + nearr) / (farr - nearr);


    ret.m00 = x_orth;
    ret.m10 = 0;
    ret.m20 = 0;
    ret.m30 = 0;
    ret.m01 = 0;
    ret.m11 = y_orth;
    ret.m21 = 0;
    ret.m31 = 0;
    ret.m02 = 0;
    ret.m12 = 0;
    ret.m22 = z_orth;
    ret.m32 = 0;
    ret.m03 = tx;
    ret.m13 = ty;
    ret.m23 = tz;
    ret.m33 = 1;

    return ret;
}

arc::Mat4 arc::Mat4::identity() {
    Mat4 ret = Mat4();
    ret.m00 = 1.0f;
    ret.m01 = 0.0f;
    ret.m02 = 0.0f;
    ret.m03 = 0.0f;
    ret.m10 = 0.0f;
    ret.m11 = 1.0f;
    ret.m12 = 0.0f;
    ret.m13 = 0.0f;
    ret.m20 = 0.0f;
    ret.m21 = 0.0f;
    ret.m22 = 1.0f;
    ret.m23 = 0.0f;
    ret.m30 = 0.0f;
    ret.m31 = 0.0f;
    ret.m32 = 0.0f;
    ret.m33 = 1.0f;
    return ret;
}

arc::Mat4& arc::Mat4::set(arc::Vec3 translation, arc::Quat rotation, arc::Vec3 scale) {
    float xs = rotation.x * 2.0f, ys = rotation.y * 2.0f, zs = rotation.z * 2.0f;
    float wx = rotation.w * xs, wy = rotation.w * ys, wz = rotation.w * zs;
    float xx = rotation.x * xs, xy = rotation.x * ys, xz = rotation.x * zs;
    float yy = rotation.y * ys, yz = rotation.y * zs, zz = rotation.z * zs;

    m00 = scale.x * (1.0f - (yy + zz));
    m01 = scale.y * (xy - wz);
    m02 = scale.z * (xz + wy);
    m03 = translation.x;

    m10 = scale.x * (xy + wz);
    m11 = scale.y * (1.0f - (xx + zz));
    m12 = scale.z * (yz - wx);
    m13 = translation.y;

    m20 = scale.x * (xz - wy);
    m21 = scale.y * (yz + wx);
    m22 = scale.z * (1.0f - (xx + yy));
    m23 = translation.z;

    m30 = 0.0f;
    m31 = 0.0f;
    m32 = 0.0f;
    m33 = 1.0f;
    return *this;
}

void arc::Mat4::set(arc::Vec3 translation, arc::Quat quat) {
    float xs = quat.x * 2.0f, ys = quat.y * 2.0f, zs = quat.z * 2.0f;
    float wx = quat.w * xs, wy = quat.w * ys, wz = quat.w * zs;
    float xx = quat.x * xs, xy = quat.x * ys, xz = quat.x * zs;
    float yy = quat.y * ys, yz = quat.y * zs, zz = quat.z * zs;

    m00 = (1.0f - (yy + zz));
    m01 = (xy - wz);
    m02 = (xz + wy);
    m03 = translation.x;

    m10 = (xy + wz);
    m11 = (1.0f - (xx + zz));
    m12 = (yz - wx);
    m13 = translation.y;

    m20 = (xz - wy);
    m21 = (yz + wx);
    m22 = (1.0f - (xx + yy));
    m23 = translation.z;

    m30 = 0.0f;
    m31 = 0.0f;
    m32 = 0.0f;
    m33 = 1.0f;
}

void arc::Mat4::set(float translationX, float translationY, float translationZ, float quaternionX, float quaternionY,
                    float quaternionZ, float quaternionW) {
    float xs = quaternionX * 2.0f, ys = quaternionY * 2.0f, zs = quaternionZ * 2.0f;
    float wx = quaternionW * xs, wy = quaternionW * ys, wz = quaternionW * zs;
    float xx = quaternionX * xs, xy = quaternionX * ys, xz = quaternionX * zs;
    float yy = quaternionY * ys, yz = quaternionY * zs, zz = quaternionZ * zs;

    m00 = (1.0f - (yy + zz));
    m01 = (xy - wz);
    m02 = (xz + wy);
    m03 = translationX;

    m10 = (xy + wz);
    m11 = (1.0f - (xx + zz));
    m12 = (yz - wx);
    m13 = translationY;

    m20 = (xz - wy);
    m21 = (yz + wx);
    m22 = (1.0f - (xx + yy));
    m23 = translationZ;

    m30 = 0.0f;
    m31 = 0.0f;
    m32 = 0.0f;
    m33 = 1.0f;
}

void arc::Mat4::idt()
{
    m00 = 1.0f;
    m01 = 0.0f;
    m02 = 0.0f;
    m03 = 0.0f;
    m10 = 0.0f;
    m11 = 1.0f;
    m12 = 0.0f;
    m13 = 0.0f;
    m20 = 0.0f;
    m21 = 0.0f;
    m22 = 1.0f;
    m23 = 0.0f;
    m30 = 0.0f;
    m31 = 0.0f;
    m32 = 0.0f;
    m33 = 1.0f;
}

void arc::Mat4::setToTranslation(arc::Vec3 position)
{
    m03 = position.x;
    m13 = position.y;
    m23 = position.z;
}
