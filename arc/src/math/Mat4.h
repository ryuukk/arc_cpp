#pragma once

#include <cmath>
#include "Mathf.h"
#include "Vec3.h"

namespace arc
{
    struct Mat4
    {
    public:
        Mat4() = default;
        Mat4(float m00, float m01, float m02, float m03, float m04, float m05, float m06, float m07, float m08, float m09,
        float m10, float m11, float m12, float m13, float m14, float m15)
        {
            data[0] = m00; data[1] = m01;  data[2] = m02;  data[3] = m03;
            data[4] = m04; data[5] = m05;  data[6] = m06;  data[7] = m07;
            data[8] = m08; data[9] = m09;  data[10]= m10;  data[11]= m11;
            data[12]= m12; data[13]= m13;  data[14]= m14;  data[15]= m15;
        }
        float data[16];


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
    };

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

    arc::Mat4 mat4_identity()
    {
        auto ret = Mat4();
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

    arc::Mat4 mat4_createProjection(float near, float far, float fovy, float aspectRatio)
    {
        auto ret = arc::mat4_identity();
        float l_fd = (1.0f / std::tan((fovy * (PI / 180)) / 2.0f));
        float l_a1 = (far + near) / (near - far);
        float l_a2 = (2 * far * near) / (near - far);
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

    arc::Mat4 mat4_createTranslation(Vec3 position)
    {
        auto ret = mat4_identity();
        ret.data[M03] = position.x();
        ret.data[M13] = position.y();
        ret.data[M23] = position.z();
        return ret;

    }
    arc::Mat4 mat4_createLookAt(Vec3 direction, Vec3 up)
    {
        auto l_vez = direction.nor();
        auto l_vex = direction.nor();

        l_vex = l_vex.crs(up).nor();
        auto l_vey = l_vex.crs(l_vez).nor();

        auto ret = mat4_identity();
        ret.data[M00] = l_vex.x();
        ret.data[M01] = l_vex.y();
        ret.data[M02] = l_vex.z();
        ret.data[M10] = l_vey.x();
        ret.data[M11] = l_vey.y();
        ret.data[M12] = l_vey.z();
        ret.data[M20] = -l_vez.x();
        ret.data[M21] = -l_vez.y();
        ret.data[M22] = -l_vez.z();

        return ret;
    }

    arc::Mat4 mat4_createLookAt(Vec3 position, Vec3 target, Vec3 up)
    {
        auto tmp = target - position;

        auto ret = mat4_createLookAt(tmp, up) * mat4_createTranslation(-position);

        return ret;
    }
}
