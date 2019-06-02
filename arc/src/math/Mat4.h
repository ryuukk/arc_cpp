#pragma once

#include <cmath>
#include <stdexcept>
#include "Mathf.h"
#include "Vec3.h"
#include "Quat.h"
#include "Affine2.h"

namespace arc
{
    struct Vec3;
    struct Quat;
    struct Mat4
    {
    public:
        Mat4() {
            idt();
        };

        Mat4(float m00, float m01, float m02, float m03, float m04, float m05, float m06, float m07, float m08,
             float m09,
             float m10, float m11, float m12, float m13, float m14, float m15) {
            this->m00 = m00;
            this->m10 = m01;
            m20 = m02;
            m30 = m03;
            this->m01 = m04;
            this->m11 = m05;
            m21 = m06;
            m31 = m07;
            this->m02 = m08;
            this->m12 = m09;
            m22 = m10;
            m32 = m11;
            this->m03 = m12;
            this->m13 = m13;
            m23 = m14;
            m33 = m15;
        }

        float m00{};
        float m10{};
        float m20{};
        float m30{};
        float m01{};
        float m11{};
        float m21{};
        float m31{};
        float m02{};
        float m12{};
        float m22{};
        float m32{};
        float m03{};
        float m13{};
        float m23{};
        float m33{};


        Mat4 operator*(const Mat4& rhs) const {
            return Mat4(
                    m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30,
                    m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30,
                    m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30,
                    m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + m33 * rhs.m30,

                    m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31,
                    m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31,
                    m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31,
                    m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31,

                    m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32,
                    m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32,
                    m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32,
                    m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32,

                    m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33,
                    m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33,
                    m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33,
                    m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33

            );
        }

        void rotate(const Vec3& axis, float degree);
        void scale(const Vec3& scale);

        void setToTranslation(Vec3 position);

        void idt();

        void set(float translationX, float translationY, float translationZ, float quaternionX, float quaternionY,
                 float quaternionZ,
                 float quaternionW);

        void set(Vec3 translation, Quat quat);

        Mat4& set(Vec3 translation, Quat rotation, Vec3 scale);
        Mat4& set(const Affine2& affine);

        static Mat4 identity();

        static Mat4
        createOrthographic(float left, float right, float bottom, float top, float nearr = 0.0f, float farr = 1.0f);

        static Mat4 createOrthographicOffCenter(float x, float y, float width, float height);


        // todo: figure out what i can't use near and far as name
        static Mat4 createProjection(float nearr, float farr, float fovy, float aspectRatio);

        static Mat4 createTranslation(Vec3 position);

        static Mat4 createLookAt(Vec3 direction, Vec3 up);

        static Mat4 createLookAt(Vec3 position, Vec3 target, Vec3 up);

        static Mat4 inv(const Mat4& mat);


        static void print(const arc::Mat4& matrix) {
            printf("----\n");
            printf("----\n");

            printf("%f\n", matrix.m00);
            printf("%f\n", matrix.m10);
            printf("%f\n", matrix.m20);
            printf("%f\n", matrix.m30);
            printf("%f\n", matrix.m01);
            printf("%f\n", matrix.m11);
            printf("%f\n", matrix.m21);
            printf("%f\n", matrix.m31);
            printf("%f\n", matrix.m02);
            printf("%f\n", matrix.m12);
            printf("%f\n", matrix.m22);
            printf("%f\n", matrix.m32);
            printf("%f\n", matrix.m03);
            printf("%f\n", matrix.m13);
            printf("%f\n", matrix.m23);
            printf("%f\n", matrix.m33);

            printf("----\n");
            printf("----\n");
        }
    };


}
