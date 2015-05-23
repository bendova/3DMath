#ifndef _MY_CODE_SNOWFLAKE_VECTOR_H
#define _MY_CODE_SNOWFLAKE_VECTOR_H

#include <cmath>
#include "../framework/VectorMath.h"

namespace MyCode
{
	namespace VectorMath
	{
		struct Vector
		{
			Vector(const float v) : x(v), y(v), z(v)
			{}
			Vector(const float fX, const float fY, const float fZ) : x(fX), y(fY), z(fZ)
			{}
			Vector() : x(0.0f), y(0.0f), z(0.0f)
			{}

			void normalize()
			{
				const float vectorLength = length();
				x /= vectorLength;
				y /= vectorLength;
				z /= vectorLength;
			}

			void scale(const float value)
			{
				x *= value;
				y *= value;
				z *= value;
			}

			float length()
			{
				return std::sqrt(x * x + y * y + z * z);
			}

			float x, y, z;
		};

		inline Vector operator-(const Point& lhs, const Point& rhs)
		{
			return Vector(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
		}

		inline Vector operator/(const Vector& lhs, const float& rhs)
		{
			return Vector(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
		}

		inline Point operator+(const Point& p, const Vector& v)
		{
			return Point(p.x + v.x, p.y + v.y, p.z + v.z);
		}

		inline float dot(const Vector& a, const Vector& b)
		{
			return a.x * b.x +
				a.y * b.y +
				a.z * b.z;
		}

		inline Vector cross(const Vector& lhs, const Vector& rhs)
		{
			return Vector(lhs.y * rhs.z - lhs.z * rhs.y,
				lhs.z * rhs.x - lhs.x * rhs.z,
				lhs.x * rhs.y - lhs.y * rhs.x);
		}
	}
}

#endif //_MY_CODE_SNOWFLAKE_VECTOR_H