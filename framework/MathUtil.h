#ifndef _MATH_UTIL_
#define _MATH_UTIL_

#include <math.h>
#include <glm/glm.hpp>
#include <array>
#include <initializer_list>

namespace MyCode
{
	namespace MathUtil
	{
		const float PI = 3.14159f;

		float DegreesToRadians(float angleInDegrees);
		float GetFrustumScale(float angleInDegrees);
		glm::mat4 RotateX(float angleInDegrees);
		glm::mat4 RotateY(float angleInDegrees);
		glm::mat4 RotateZ(float angleInDegrees);

		float Clamp(const float value, const float min, const float max);

		template<typename T>
		float length(const T& a);

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
				const float vectorLength = length(*this);
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

			float x, y, z;
		};

		struct Point
		{
			Point(const float fX = 0.0f, const float fY = 0.0f, const float fZ = 0.0f)
				: x(fX), y(fY), z(fZ)
			{}
			float x, y, z;
		};

		struct Color
		{
			Color(const float fR = 0.0f, const float fG = 0.0f, const float fB = 0.0f, const float fA = 1.0f)
				: r(fR), g(fG), b(fB), a(fA)
			{}

			float r, g, b, a;
		};

		struct ColoredPoint
		{
			ColoredPoint(const Point p = Point(), const Color c = Color())
				: position(p), color(c)
			{}
			Point position;
			Color color;
		};

		inline
		Vector operator-(const Point& lhs, const Point& rhs)
		{
			return Vector(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
		}

		inline
		Vector operator/(const Vector& lhs, const float& rhs)
		{
			return Vector(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
		}

		inline
		Point operator+(const Point& p, const Vector& v)
		{
			return Point(p.x + v.x, p.y + v.y, p.z + v.z);
		}

		template<typename T>
		float length(const T& a)
		{
			return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
		}

		inline
		float dot(const Vector& a, const Vector& b)
		{
			return a.x * b.x +
				a.y * b.y +
				a.z * b.z;
		}

		inline
		Vector cross(const Vector& lhs, const Vector& rhs)
		{
			return Vector(lhs.y * rhs.z - lhs.z * rhs.y,
				lhs.z * rhs.x - lhs.x * rhs.z,
				lhs.x * rhs.y - lhs.y * rhs.x);
		}

		template<size_t POINTS_COUNT>
		class Polygon
		{
		public:
			typedef std::array<glm::vec4, POINTS_COUNT> Vertices;

			Polygon(std::initializer_list<glm::vec4> vertices)
				: mVertices{}
			{
				int index = 0;
				auto itEnd = vertices.end();
				for (auto it = vertices.begin(); ((it != itEnd) && (index < POINTS_COUNT)); ++it, ++index)
				{
					mVertices[index] = *it;
				}
			}

			glm::vec4& operator[](int index) { return mVertices[index]; }
			const glm::vec4& operator[](int index) const { return mVertices[index]; }

			const Vertices& GetVertices() const { return mVertices; }
			Vertices& GetVertices() { return mVertices; }

			static
			size_t GetPointsCount() { return POINTS_COUNT; }

		private:
			Vertices mVertices;
		};
		typedef Polygon<3> Triangle;
		typedef Polygon<4> Quadrilateral;

		glm::vec4 GetIntersectionPoint(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& d);
		glm::vec4 GetProjectionPoint(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c);
		glm::vec4 GetNormalToLineFromPoint(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c);
		bool IsPointInsideOfSide(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& p);
		bool Contains(Triangle t, const glm::vec4& p);
		bool Contains(Quadrilateral q, const glm::vec4& p);

	}
}
#endif