#ifndef _MY_CODE_VECTOR_MATH_DATA_TYPES_H_
#define _MY_CODE_VECTOR_MATH_DATA_TYPES_H_

#include <glm/glm.hpp>

namespace MyCode
{
	namespace VectorMath
	{
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

		struct Plane
		{
			Plane(const glm::vec3& point, const glm::vec3& normal)
				: mPointInPlane(point)
				, mNormalToPlane(normal)
			{
			}

			glm::vec3 mPointInPlane;
			glm::vec3 mNormalToPlane;
		};

		enum class BoundingPointType
		{
			UNBOUNDED = 0,
			BOUNDED,
		};

		enum class PointType
		{
			CLOSED_ENDED = 0,
			OPEN_ENDED,
		};

		template<typename T>
		struct MarginPoint
		{
			MarginPoint(const T& point, const BoundingPointType boundingType = BoundingPointType::BOUNDED,
				const PointType pointType = PointType::CLOSED_ENDED)
				: mPoint(point)
				, mBoundingPointType(boundingType)
				, mPointEndType(pointType)
			{}
			const T mPoint;
			const BoundingPointType mBoundingPointType;
			const PointType mPointEndType;
		};
	}
}

#endif //_MY_CODE_VECTOR_MATH_DATA_TYPES_H_