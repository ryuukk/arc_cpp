#pragma once

#include <cmath>
#include <stdexcept>
#include "Mathf.h"
#include "Vec3.h"
#include "Quat.h"

namespace arc
{
    const int M00 = 0;
    const int M01 = 4;
    const int M02 = 8;
    const int M03 = 12;
    const int M10 = 1;
    const int M11 = 5;
    const int M12 = 9;
    const int M13 = 13;
    const int M20 = 2;
    const int M21 = 6;
    const int M22 = 10;
    const int M23 = 14;
    const int M30 = 3;
    const int M31 = 7;
    const int M32 = 11;
    const int M33 = 15;

    struct Mat4
    {
    public:
        Mat4()
        {
            idt();
        };
        Mat4(float m00, float m01, float m02, float m03, float m04, float m05, float m06, float m07, float m08, float m09,
        float m10, float m11, float m12, float m13, float m14, float m15)
        {
            data[0] = m00; data[1] = m01;  data[2] = m02;  data[3] = m03;
            data[4] = m04; data[5] = m05;  data[6] = m06;  data[7] = m07;
            data[8] = m08; data[9] = m09;  data[10]= m10;  data[11]= m11;
            data[12]= m12; data[13]= m13;  data[14]= m14;  data[15]= m15;
        }
        float data[16]{};


        Mat4 operator*(const Mat4 &n) const {
            return Mat4(data[0] * n.data[0] + data[4] * n.data[1] + data[8] * n.data[2] + data[12] * n.data[3],
                        data[1] * n.data[0] + data[5] * n.data[1] + data[9] * n.data[2] + data[13] * n.data[3],
                        data[2] * n.data[0] + data[6] * n.data[1] + data[10] * n.data[2] + data[14] * n.data[3],
                        data[3] * n.data[0] + data[7] * n.data[1] + data[11] * n.data[2] + data[15] * n.data[3],
                        data[0] * n.data[4] + data[4] * n.data[5] + data[8] * n.data[6] + data[12] * n.data[7],
                        data[1] * n.data[4] + data[5] * n.data[5] + data[9] * n.data[6] + data[13] * n.data[7],
                        data[2] * n.data[4] + data[6] * n.data[5] + data[10] * n.data[6] + data[14] * n.data[7],
                        data[3] * n.data[4] + data[7] * n.data[5] + data[11] * n.data[6] + data[15] * n.data[7],
                        data[0] * n.data[8] + data[4] * n.data[9] + data[8] * n.data[10] + data[12] * n.data[11],
                        data[1] * n.data[8] + data[5] * n.data[9] + data[9] * n.data[10] + data[13] * n.data[11],
                        data[2] * n.data[8] + data[6] * n.data[9] + data[10] * n.data[10] + data[14] * n.data[11],
                        data[3] * n.data[8] + data[7] * n.data[9] + data[11] * n.data[10] + data[15] * n.data[11],
                        data[0] * n.data[12] + data[4] * n.data[13] + data[8] * n.data[14] + data[12] * n.data[15],
                        data[1] * n.data[12] + data[5] * n.data[13] + data[9] * n.data[14] + data[13] * n.data[15],
                        data[2] * n.data[12] + data[6] * n.data[13] + data[10] * n.data[14] + data[14] * n.data[15],
                        data[3] * n.data[12] + data[7] * n.data[13] + data[11] * n.data[14] + data[15] * n.data[15]);
        }

        void setToTranslation(Vec3 position)
        {
            data[M03] = position.x;
            data[M13] = position.y;
            data[M23] = position.z;
        }

        void idt()
        {
            data[M00] = 1.f;
            data[M01] = 0.f;
            data[M02] = 0.f;
            data[M03] = 0.f;
            data[M10] = 0.f;
            data[M11] = 1.f;
            data[M12] = 0.f;
            data[M13] = 0.f;
            data[M20] = 0.f;
            data[M21] = 0.f;
            data[M22] = 1.f;
            data[M23] = 0.f;
            data[M30] = 0.f;
            data[M31] = 0.f;
            data[M32] = 0.f;
            data[M33] = 1.f;
        }

        void set(float translationX, float translationY, float translationZ, float quaternionX, float quaternionY, float quaternionZ,
                 float quaternionW)
        {
            float xs = quaternionX * 2.0f, ys = quaternionY * 2.0f, zs = quaternionZ * 2.0f;
            float wx = quaternionW * xs, wy = quaternionW * ys, wz = quaternionW * zs;
            float xx = quaternionX * xs, xy = quaternionX * ys, xz = quaternionX * zs;
            float yy = quaternionY * ys, yz = quaternionY * zs, zz = quaternionZ * zs;

            data[M00] = (1.0f - (yy + zz));
            data[M01] = (xy - wz);
            data[M02] = (xz + wy);
            data[M03] = translationX;

            data[M10] = (xy + wz);
            data[M11] = (1.0f - (xx + zz));
            data[M12] = (yz - wx);
            data[M13] = translationY;

            data[M20] = (xz - wy);
            data[M21] = (yz + wx);
            data[M22] = (1.0f - (xx + yy));
            data[M23] = translationZ;

            data[M30] = 0.0f;
            data[M31] = 0.0f;
            data[M32] = 0.0f;
            data[M33] = 1.0f;
        }

        void set(Vec3 translation, Quat quat)
        {
            float xs = quat.x * 2.0f, ys = quat.y * 2.0f, zs = quat.z * 2.0f;
            float wx = quat.w * xs, wy = quat.w * ys, wz = quat.w * zs;
            float xx = quat.x * xs, xy = quat.x * ys, xz = quat.x * zs;
            float yy = quat.y * ys, yz = quat.y * zs, zz = quat.z * zs;

            data[M00] = (1.0f - (yy + zz));
            data[M01] = (xy - wz);
            data[M02] = (xz + wy);
            data[M03] = translation.x;

            data[M10] = (xy + wz);
            data[M11] = (1.0f - (xx + zz));
            data[M12] = (yz - wx);
            data[M13] = translation.y;

            data[M20] = (xz - wy);
            data[M21] = (yz + wx);
            data[M22] = (1.0f - (xx + yy));
            data[M23] = translation.z;

            data[M30] = 0.0f;
            data[M31] = 0.0f;
            data[M32] = 0.0f;
            data[M33] = 1.0f;
        }

        Mat4& set(Vec3 translation, Quat rotation, Vec3 scale) {
            float xs = rotation.x * 2.0f, ys = rotation.y * 2.0f, zs = rotation.z * 2.0f;
            float wx = rotation.w * xs, wy = rotation.w * ys, wz = rotation.w * zs;
            float xx = rotation.x * xs, xy = rotation.x * ys, xz = rotation.x * zs;
            float yy = rotation.y * ys, yz = rotation.y * zs, zz = rotation.z * zs;

            data[M00] = scale.x * (1.0f - (yy + zz));
            data[M01] = scale.y * (xy - wz);
            data[M02] = scale.z * (xz + wy);
            data[M03] = translation.x;

            data[M10] = scale.x * (xy + wz);
            data[M11] = scale.y * (1.0f - (xx + zz));
            data[M12] = scale.z * (yz - wx);
            data[M13] = translation.y;

            data[M20] = scale.x * (xz - wy);
            data[M21] = scale.y * (yz + wx);
            data[M22] = scale.z * (1.0f - (xx + yy));
            data[M23] = translation.z;

            data[M30] = 0.0f;
            data[M31] = 0.0f;
            data[M32] = 0.0f;
            data[M33] = 1.0f;
            return *this;
        }

        static Mat4 identity()
        {
            Mat4 ret = Mat4();
            ret.data[M00] = 1.f;
            ret.data[M01] = 0.f;
            ret.data[M02] = 0.f;
            ret.data[M03] = 0.f;
            ret.data[M10] = 0.f;
            ret.data[M11] = 1.f;
            ret.data[M12] = 0.f;
            ret.data[M13] = 0.f;
            ret.data[M20] = 0.f;
            ret.data[M21] = 0.f;
            ret.data[M22] = 1.f;
            ret.data[M23] = 0.f;
            ret.data[M30] = 0.f;
            ret.data[M31] = 0.f;
            ret.data[M32] = 0.f;
            ret.data[M33] = 1.f;
            return ret;
        }


        // todo: figure out what i can't use near and far as name
        static Mat4 createProjection(float nearr, float farr, float fovy, float aspectRatio)
        {
            auto ret = identity();
            float l_fd = (1.0f / std::tan((fovy * (Mathf::PI() / 180)) / 2.0f));
            float l_a1 = (farr + nearr) / (nearr - farr);
            float l_a2 = (2 * farr * nearr) / (nearr - farr);
            ret.data[M00] = l_fd / aspectRatio;
            ret.data[M10] = 0;
            ret.data[M20] = 0;
            ret.data[M30] = 0;
            ret.data[M01] = 0;
            ret.data[M11] = l_fd;
            ret.data[M21] = 0;
            ret.data[M31] = 0;
            ret.data[M02] = 0;
            ret.data[M12] = 0;
            ret.data[M22] = l_a1;
            ret.data[M32] = -1;
            ret.data[M03] = 0;
            ret.data[M13] = 0;
            ret.data[M23] = l_a2;
            ret.data[M33] = 0;
            return ret;
        }

        static Mat4 createTranslation(Vec3 position)
        {
            auto ret = identity();
            ret.data[M03] = position.x;
            ret.data[M13] = position.y;
            ret.data[M23] = position.z;
            return ret;

        }
        static Mat4 createLookAt(Vec3 direction, Vec3 up)
        {
            auto l_vez = direction.nor();
            auto l_vex = direction.nor();

            l_vex = l_vex.crs(up).nor();
            auto l_vey = l_vex.crs(l_vez).nor();

            auto ret = identity();
            ret.data[M00] = l_vex.x;
            ret.data[M01] = l_vex.y;
            ret.data[M02] = l_vex.z;
            ret.data[M10] = l_vey.x;
            ret.data[M11] = l_vey.y;
            ret.data[M12] = l_vey.z;
            ret.data[M20] = -l_vez.x;
            ret.data[M21] = -l_vez.y;
            ret.data[M22] = -l_vez.z;

            return ret;
        }

        static Mat4 createLookAt(Vec3 position, Vec3 target, Vec3 up)
        {
            auto tmp = target - position;

            auto ret = createLookAt(tmp, up) * createTranslation(-position);

            return ret;
        }

        static Mat4 inv(const Mat4& mat)
        {
            float l_det = mat.data[M30] * mat.data[M21] * mat.data[M12] * mat.data[M03] - mat.data[M20] * mat.data[M31] * mat.data[M12] * mat.data[M03] - mat.data[M30] * mat.data[M11]
                                                                                                                                                  * mat.data[M22] * mat.data[M03] + mat.data[M10] * mat.data[M31] * mat.data[M22] * mat.data[M03] + mat.data[M20] * mat.data[M11] * mat.data[M32] * mat.data[M03] - mat.data[M10]
                                                                                                                                                                                                                                                                                                          * mat.data[M21] * mat.data[M32] * mat.data[M03] - mat.data[M30] * mat.data[M21] * mat.data[M02] * mat.data[M13] + mat.data[M20] * mat.data[M31] * mat.data[M02] * mat.data[M13]
                          + mat.data[M30] * mat.data[M01] * mat.data[M22] * mat.data[M13] - mat.data[M00] * mat.data[M31] * mat.data[M22] * mat.data[M13] - mat.data[M20] * mat.data[M01] * mat.data[M32]
                                                                                                                                                    * mat.data[M13] + mat.data[M00] * mat.data[M21] * mat.data[M32] * mat.data[M13] + mat.data[M30] * mat.data[M11] * mat.data[M02] * mat.data[M23] - mat.data[M10] * mat.data[M31]
                                                                                                                                                                                                                                                                                             * mat.data[M02] * mat.data[M23] - mat.data[M30] * mat.data[M01] * mat.data[M12] * mat.data[M23] + mat.data[M00] * mat.data[M31] * mat.data[M12] * mat.data[M23] + mat.data[M10]
                                                                                                                                                                                                                                                                                                                                                                                                                                                     * mat.data[M01] * mat.data[M32] * mat.data[M23] - mat.data[M00] * mat.data[M11] * mat.data[M32] * mat.data[M23] - mat.data[M20] * mat.data[M11] * mat.data[M02] * mat.data[M33]
                          + mat.data[M10] * mat.data[M21] * mat.data[M02] * mat.data[M33] + mat.data[M20] * mat.data[M01] * mat.data[M12] * mat.data[M33] - mat.data[M00] * mat.data[M21] * mat.data[M12]
                                                                                                                                                    * mat.data[M33] - mat.data[M10] * mat.data[M01] * mat.data[M22] * mat.data[M33] + mat.data[M00] * mat.data[M11] * mat.data[M22] * mat.data[M33];
            if (l_det == 0.0f) throw std::invalid_argument("non-invertible matrix");
            float inv_det = 1.0f / l_det;
            Mat4 tmp = Mat4::identity();
            tmp.data[M00] = mat.data[M12] * mat.data[M23] * mat.data[M31] - mat.data[M13] * mat.data[M22] * mat.data[M31] + mat.data[M13] * mat.data[M21] * mat.data[M32] - mat.data[M11]
                                                                                                                                                                  * mat.data[M23] * mat.data[M32] - mat.data[M12] * mat.data[M21] * mat.data[M33] + mat.data[M11] * mat.data[M22] * mat.data[M33];
            tmp.data[M01] = mat.data[M03] * mat.data[M22] * mat.data[M31] - mat.data[M02] * mat.data[M23] * mat.data[M31] - mat.data[M03] * mat.data[M21] * mat.data[M32] + mat.data[M01]
                                                                                                                                                                  * mat.data[M23] * mat.data[M32] + mat.data[M02] * mat.data[M21] * mat.data[M33] - mat.data[M01] * mat.data[M22] * mat.data[M33];
            tmp.data[M02] = mat.data[M02] * mat.data[M13] * mat.data[M31] - mat.data[M03] * mat.data[M12] * mat.data[M31] + mat.data[M03] * mat.data[M11] * mat.data[M32] - mat.data[M01]
                                                                                                                                                                  * mat.data[M13] * mat.data[M32] - mat.data[M02] * mat.data[M11] * mat.data[M33] + mat.data[M01] * mat.data[M12] * mat.data[M33];
            tmp.data[M03] = mat.data[M03] * mat.data[M12] * mat.data[M21] - mat.data[M02] * mat.data[M13] * mat.data[M21] - mat.data[M03] * mat.data[M11] * mat.data[M22] + mat.data[M01]
                                                                                                                                                                  * mat.data[M13] * mat.data[M22] + mat.data[M02] * mat.data[M11] * mat.data[M23] - mat.data[M01] * mat.data[M12] * mat.data[M23];
            tmp.data[M10] = mat.data[M13] * mat.data[M22] * mat.data[M30] - mat.data[M12] * mat.data[M23] * mat.data[M30] - mat.data[M13] * mat.data[M20] * mat.data[M32] + mat.data[M10]
                                                                                                                                                                  * mat.data[M23] * mat.data[M32] + mat.data[M12] * mat.data[M20] * mat.data[M33] - mat.data[M10] * mat.data[M22] * mat.data[M33];
            tmp.data[M11] = mat.data[M02] * mat.data[M23] * mat.data[M30] - mat.data[M03] * mat.data[M22] * mat.data[M30] + mat.data[M03] * mat.data[M20] * mat.data[M32] - mat.data[M00]
                                                                                                                                                                  * mat.data[M23] * mat.data[M32] - mat.data[M02] * mat.data[M20] * mat.data[M33] + mat.data[M00] * mat.data[M22] * mat.data[M33];
            tmp.data[M12] = mat.data[M03] * mat.data[M12] * mat.data[M30] - mat.data[M02] * mat.data[M13] * mat.data[M30] - mat.data[M03] * mat.data[M10] * mat.data[M32] + mat.data[M00]
                                                                                                                                                                  * mat.data[M13] * mat.data[M32] + mat.data[M02] * mat.data[M10] * mat.data[M33] - mat.data[M00] * mat.data[M12] * mat.data[M33];
            tmp.data[M13] = mat.data[M02] * mat.data[M13] * mat.data[M20] - mat.data[M03] * mat.data[M12] * mat.data[M20] + mat.data[M03] * mat.data[M10] * mat.data[M22] - mat.data[M00]
                                                                                                                                                                  * mat.data[M13] * mat.data[M22] - mat.data[M02] * mat.data[M10] * mat.data[M23] + mat.data[M00] * mat.data[M12] * mat.data[M23];
            tmp.data[M20] = mat.data[M11] * mat.data[M23] * mat.data[M30] - mat.data[M13] * mat.data[M21] * mat.data[M30] + mat.data[M13] * mat.data[M20] * mat.data[M31] - mat.data[M10]
                                                                                                                                                                  * mat.data[M23] * mat.data[M31] - mat.data[M11] * mat.data[M20] * mat.data[M33] + mat.data[M10] * mat.data[M21] * mat.data[M33];
            tmp.data[M21] = mat.data[M03] * mat.data[M21] * mat.data[M30] - mat.data[M01] * mat.data[M23] * mat.data[M30] - mat.data[M03] * mat.data[M20] * mat.data[M31] + mat.data[M00]
                                                                                                                                                                  * mat.data[M23] * mat.data[M31] + mat.data[M01] * mat.data[M20] * mat.data[M33] - mat.data[M00] * mat.data[M21] * mat.data[M33];
            tmp.data[M22] = mat.data[M01] * mat.data[M13] * mat.data[M30] - mat.data[M03] * mat.data[M11] * mat.data[M30] + mat.data[M03] * mat.data[M10] * mat.data[M31] - mat.data[M00]
                                                                                                                                                                  * mat.data[M13] * mat.data[M31] - mat.data[M01] * mat.data[M10] * mat.data[M33] + mat.data[M00] * mat.data[M11] * mat.data[M33];
            tmp.data[M23] = mat.data[M03] * mat.data[M11] * mat.data[M20] - mat.data[M01] * mat.data[M13] * mat.data[M20] - mat.data[M03] * mat.data[M10] * mat.data[M21] + mat.data[M00]
                                                                                                                                                                  * mat.data[M13] * mat.data[M21] + mat.data[M01] * mat.data[M10] * mat.data[M23] - mat.data[M00] * mat.data[M11] * mat.data[M23];
            tmp.data[M30] = mat.data[M12] * mat.data[M21] * mat.data[M30] - mat.data[M11] * mat.data[M22] * mat.data[M30] - mat.data[M12] * mat.data[M20] * mat.data[M31] + mat.data[M10]
                                                                                                                                                                  * mat.data[M22] * mat.data[M31] + mat.data[M11] * mat.data[M20] * mat.data[M32] - mat.data[M10] * mat.data[M21] * mat.data[M32];
            tmp.data[M31] = mat.data[M01] * mat.data[M22] * mat.data[M30] - mat.data[M02] * mat.data[M21] * mat.data[M30] + mat.data[M02] * mat.data[M20] * mat.data[M31] - mat.data[M00]
                                                                                                                                                                  * mat.data[M22] * mat.data[M31] - mat.data[M01] * mat.data[M20] * mat.data[M32] + mat.data[M00] * mat.data[M21] * mat.data[M32];
            tmp.data[M32] = mat.data[M02] * mat.data[M11] * mat.data[M30] - mat.data[M01] * mat.data[M12] * mat.data[M30] - mat.data[M02] * mat.data[M10] * mat.data[M31] + mat.data[M00]
                                                                                                                                                                  * mat.data[M12] * mat.data[M31] + mat.data[M01] * mat.data[M10] * mat.data[M32] - mat.data[M00] * mat.data[M11] * mat.data[M32];
            tmp.data[M33] = mat.data[M01] * mat.data[M12] * mat.data[M20] - mat.data[M02] * mat.data[M11] * mat.data[M20] + mat.data[M02] * mat.data[M10] * mat.data[M21] - mat.data[M00]
                                                                                                                                                                  * mat.data[M12] * mat.data[M21] - mat.data[M01] * mat.data[M10] * mat.data[M22] + mat.data[M00] * mat.data[M11] * mat.data[M22];
            tmp.data[M00] = tmp.data[M00] * inv_det;
            tmp.data[M01] = tmp.data[M01] * inv_det;
            tmp.data[M02] = tmp.data[M02] * inv_det;
            tmp.data[M03] = tmp.data[M03] * inv_det;
            tmp.data[M10] = tmp.data[M10] * inv_det;
            tmp.data[M11] = tmp.data[M11] * inv_det;
            tmp.data[M12] = tmp.data[M12] * inv_det;
            tmp.data[M13] = tmp.data[M13] * inv_det;
            tmp.data[M20] = tmp.data[M20] * inv_det;
            tmp.data[M21] = tmp.data[M21] * inv_det;
            tmp.data[M22] = tmp.data[M22] * inv_det;
            tmp.data[M23] = tmp.data[M23] * inv_det;
            tmp.data[M30] = tmp.data[M30] * inv_det;
            tmp.data[M31] = tmp.data[M31] * inv_det;
            tmp.data[M32] = tmp.data[M32] * inv_det;
            tmp.data[M33] = tmp.data[M33] * inv_det;
            return tmp;
        }
    };


}
