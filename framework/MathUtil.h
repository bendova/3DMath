#ifndef _MATH_UTIL_
#define _MATH_UTIL_

#include <cmath>
#include <glm/glm.hpp>
#include <array>
#include <initializer_list>

namespace MyCode
{
	namespace MathUtil
	{
		extern const float PI;

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

		private:
			Vertices mVertices;

		public:

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

			auto begin() -> decltype(mVertices.begin())
			{
				return mVertices.begin();
			}

			auto end() -> decltype(mVertices.end())
			{
				return mVertices.end();
			}

			auto begin() const -> decltype(mVertices.begin())
			{
				return mVertices.begin();
			}

			auto end() const -> decltype(mVertices.end())
			{
				return mVertices.end();
			}
			
			auto rbegin() -> decltype(mVertices.rbegin())
			{
				return mVertices.rbegin();
			}

			auto rend() -> decltype(mVertices.rend())
			{
				return mVertices.rend();
			}

			auto rbegin() const -> decltype(mVertices.rbegin())
			{
				return mVertices.rbegin();
			}

			auto rend() const -> decltype(mVertices.rend())
			{
				return mVertices.rend();
			}

		};
		typedef Polygon<3> Triangle;
		typedef Polygon<4> Quadrilateral;

		template<typename T>
		T GetProjectionPointOnLine(const T& linePointA, const T& linePointB, const T& pointToProject)
		{
			const T lineDirection = linePointB - linePointA;
			const T::value_type lineDirectionLength = glm::length(lineDirection);
			const T vectorFromLineToPoint = pointToProject - linePointA;
			const T::value_type factor = glm::dot(lineDirection, vectorFromLineToPoint) / (lineDirectionLength * lineDirectionLength);
			const T projectionPoint = linePointA + factor * lineDirection;
			return projectionPoint;
		}

		float IsIntersectionFactorOnSegment(const float factor);
		float IsIntersectionFactorOnSegmentStrictly(const float factor);

		template<typename T>
		std::pair<float, bool> GetSegmentIntersectionFactor(const T& a, const T& b, const T& c, const T& d)
		{
			bool doesIntersectionPointExist = false;
			float factorAB = FLT_MAX;
			const glm::vec3 lineDirectionAB{ b - a };
			const glm::vec3 lineDirectionCD{ d - c };
			const glm::vec3 lineDirectionsCross{ glm::cross(lineDirectionAB, lineDirectionCD) };
			const auto lineDirectionsCrossLength = glm::length(lineDirectionsCross);
			if (lineDirectionsCrossLength)
			{
				const glm::vec3 ca{ a - c };
				factorAB = -glm::dot(glm::cross(ca, lineDirectionCD), lineDirectionsCross)
					/ (lineDirectionsCrossLength * lineDirectionsCrossLength);
				doesIntersectionPointExist = true;
			}
			return std::make_pair(factorAB, doesIntersectionPointExist);
		}

		template<typename T>
		bool DoLineSegmentsIntersect(const T& a, const T& b, const T& c, const T& d)
		{
			const T intersectionPoint = GetIntersectionPointBetweenSegments(a, b, c, d);
			return (intersectionPoint.w != 0);
		}

		template<typename T>
		T GetIntersectionPointBetweenSegments(const T& a, const T& b, const T& c, const T& d)
		{
			T intersectionPoint{ 0.0f };

			const auto factorAB = GetSegmentIntersectionFactor(a, b, c, d);
			const auto factorCD = GetSegmentIntersectionFactor(c, d, a, b);
			if (factorAB.second && IsIntersectionFactorOnSegment(factorAB.first) &&
				factorCD.second && IsIntersectionFactorOnSegment(factorCD.first))
			{
				intersectionPoint = a + factorAB.first * (b - a);
			}
			return intersectionPoint;
		}

		template<typename T>
		T GetIntersectionPointBetweenLines(const T& a, const T& b, const T& c, const T& d)
		{
			T intersectionPoint{ 0.0f };

			const auto factorAB = GetSegmentIntersectionFactor(a, b, c, d);
			const auto factorCD = GetSegmentIntersectionFactor(c, d, a, b);
			if (factorAB.second && factorCD.second)
			{
				intersectionPoint = a + factorAB.first * (b - a);
			}
			return intersectionPoint;
		}

		template<typename T>
		bool DoesLineSegmentIntersectQuad(const Quadrilateral& q, const T& lineSegmentPointA, const T& lineSegmentPointB)
		{
			const auto& a = q[0];
			const auto& b = q[1];
			const auto& c = q[2];
			const auto& d = q[3];

			return DoLineSegmentsIntersect(a, b, lineSegmentPointA, lineSegmentPointB)
				|| DoLineSegmentsIntersect(b, c, lineSegmentPointA, lineSegmentPointB)
				|| DoLineSegmentsIntersect(c, d, lineSegmentPointA, lineSegmentPointB)
				|| DoLineSegmentsIntersect(d, a, lineSegmentPointA, lineSegmentPointB);
		}

		template<typename T>
		T GetIntersectionPointBetweenSegmentsStrictly(const T& a, const T& b, const T& c, const T& d)
		{
			T intersectionPoint{ 0.0f };

			const auto factorAB = GetSegmentIntersectionFactor(a, b, c, d);
			const auto factorCD = GetSegmentIntersectionFactor(c, d, a, b);
			if (factorAB.second && IsIntersectionFactorOnSegmentStrictly(factorAB.first) &&
				factorCD.second && IsIntersectionFactorOnSegmentStrictly(factorCD.first))
			{
				intersectionPoint = a + factorAB.first * (b - a);
			}
			return intersectionPoint;
		}

		bool DoLineSegmentsIntersectStrictly(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d);
		bool DoLineSegmentsIntersectStrictly(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& d);

		template<typename T>
		bool DoesLineSegmentIntersectQuadStrictly(const Quadrilateral& q, const T& lineSegmentPointA, const T& lineSegmentPointB)
		{
			const auto& a = q[0];
			const auto& b = q[1];
			const auto& c = q[2];
			const auto& d = q[3];

			return DoLineSegmentsIntersectStrictly(a, b, lineSegmentPointA, lineSegmentPointB)
				|| DoLineSegmentsIntersectStrictly(b, c, lineSegmentPointA, lineSegmentPointB)
				|| DoLineSegmentsIntersectStrictly(c, d, lineSegmentPointA, lineSegmentPointB)
				|| DoLineSegmentsIntersectStrictly(d, a, lineSegmentPointA, lineSegmentPointB);
		}


		bool IsPointInPlane(const glm::vec3& planePointA, const glm::vec3& planePointB, const glm::vec3& planePointC, const glm::vec3& point);
		bool IsPointInPlane(const glm::vec4& planePointA, const glm::vec4& planePointB, const glm::vec3& planePointC, const glm::vec3& point);

		float GetDistanceFromPointToLine(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b);
		glm::vec4 GetNormalToLineFromPoint(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c);
		bool IsPointInsideOfSide(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& p);
		bool Contains(const Triangle& t, const glm::vec4& p);
		bool Contains(const Quadrilateral& q, const glm::vec4& p);

		bool IsPointInsideOfSideStrictly(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& p);
		bool ContainsStrictly(const Triangle& t, const glm::vec4& p);
		bool ContainsStrictly(const Quadrilateral& q, const glm::vec4& p);

		float FloorWithPrecision(const float x, const int precision);
		double FloorWithPrecision(const double x, const int precision);
		void Floor(glm::vec3& v, const int precision = 5);
		void Floor(glm::vec4& v, const int precision = 5);
	}
}
#endif