#pragma once

#include <cmath>

namespace arc
{
    struct Vec3
    {
    public:
        Vec3(float x = 0, float y = 0, float z = 0)
        {
            _data[0] = x;
            _data[1] = y;
            _data[2] = z;
        }

        inline auto x() -> float& {return _data[0];}
        inline auto y() -> float& {return _data[1];}
        inline auto z() -> float& {return _data[2];}

        bool isZero()
        {
            return x() == 0.0f && y() == 0.0f && z() == 0.0f;
        }

        float dot(Vec3& vector)
        {

            return x() * vector.x() + y() * vector.y() + z() * vector.z();
        }

        Vec3 nor()
        {
            float len2 = Vec3::len2();
            if(len2 == 0.0f || len2 == 1.0f)
                return Vec3(x(), y(), z());

            float scalar = 1.0f / std::sqrt(len2);

            return Vec3(x() * scalar, y() * scalar, z() * scalar);
        }

        Vec3 crs(Vec3& vector)
        {
            return Vec3(y() * vector.z() - z() * vector.y(), z() * vector.x() - x() * vector.z(), x() * vector.y() - y() * vector.x());
        }



        float len2()
        {
            return x() * x() + y() * y() + z() * z();
        }

        Vec3 operator-() const {
            return Vec3(-_data[0], -_data[1], -_data[2]);
        }


        Vec3 operator + (const Vec3& v) const
        {
            return Vec3(_data[0] + v._data[0], _data[1] + v._data[1],_data[2] + v._data[2]);
        }
        Vec3 operator - (const Vec3& v) const
        {
            return Vec3(_data[0] - v._data[0], _data[1] - v._data[1],_data[2] - v._data[2]);
        }

        Vec3 operator * (const float& v) const
        {
            return Vec3(_data[0] * v, _data[1] * v,_data[2] * v);
        }
        Vec3 operator / (const float& v) const
        {
            return Vec3(_data[0] / v, _data[1] / v,_data[2] / v);
        }

        Vec3& operator = (const Vec3& rhs)
        {
            _data[0] = rhs._data[0];
            _data[1] = rhs._data[1];
            _data[2] = rhs._data[2];
            return *this;
        }
        Vec3& operator += (const Vec3& rhs)
        {
            _data[0] += rhs._data[0];
            _data[1] += rhs._data[1];
            _data[2] += rhs._data[2];
            return *this;
        }
        Vec3& operator -= (const Vec3& rhs)
        {
            _data[0] -= rhs._data[0];
            _data[1] -= rhs._data[1];
            _data[2] -= rhs._data[2];
            return *this;
        }
        Vec3& operator *= (const Vec3& rhs)
        {
            _data[0] *= rhs._data[0];
            _data[1] *= rhs._data[1];
            _data[2] *= rhs._data[2];
            return *this;
        }
        Vec3& operator *= (const float& rhs)
        {
            _data[0] *= rhs;
            _data[1] *= rhs;
            _data[2] *= rhs;
            return *this;
        }
        Vec3& operator /= (const Vec3& rhs)
        {
            _data[0] /= rhs._data[0];
            _data[1] /= rhs._data[1];
            _data[2] /= rhs._data[2];
            return *this;
        }

        bool operator == (const Vec3& v) const
        {
            return _data[0] == v._data[0] && _data[1] == v._data[1] && _data[2] == v._data[2];
        }

        bool operator != (const Vec3& v) const
        {
            return _data[0] != v._data[0] || _data[1] != v._data[1] || _data[2] != v._data[2];
        }

        static float len(Vec3 v)
        {
            return sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());
        }

    private:
        float _data[3];
    };

}

