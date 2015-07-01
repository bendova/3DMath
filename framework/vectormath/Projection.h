#ifndef _MY_CODE_VECTOR_MATH_PROJECTION_H_
#define _MY_CODE_VECTOR_MATH_PROJECTION_H_

#include <cmath>
#include <glm/glm.hpp>
#include <initializer_list>
#include <vector>
#include <algorithm>
#include "DataTypes.h"
#include "Common.h"

namespace MyCode
{
	namespace VectorMath
	{
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

		template<typename T>
		T GetNormalToLineFromPoint(const T& a, const T& b, const T& c)
		{
			T projectionPoint = GetProjectionPointOnLine(a, b, c);
			T lineNormal = c - projectionPoint;
			return lineNormal;
		}

		template<typename T>
		float GetDotBetweenPointAndLine(const T& a, const T& b, const T& c, const T& p)
		{
			const T ap = p - a;
			const T abNormal = GetNormalToLineFromPoint(a, b, c);
			const float res = glm::dot(ap, abNormal);
			return res;
		}

		template<typename T>
		bool IsPointInsideOfSide(const T& a, const T& b, const T& c, const T& p)
		{
			return (GetDotBetweenPointAndLine(a, b, c, p) >= 0.0);
		}

		template<typename T>
		bool IsPointInsideOfSideStrictly(const T& a, const T& b, const T& c, const T& p)
		{
			return (GetDotBetweenPointAndLine(a, b, c, p) > 0.0);
		}

		template<typename T>
		bool IsPointInsidePolygon(const std::vector<T>& polygon, const MarginPoint<T>& point)
		{
			bool isInsidePolygon = false;
			const auto pointsCount = polygon.size();
			if (pointsCount > 2)
			{
				const bool isInSamePlane = IsPointInPlane(point.mPoint, polygon[0], polygon[1], polygon[2]);
				if (isInSamePlane)
				{
					const bool strictlyInside = ((point.mBoundingPointType == BoundingPointType::BOUNDED) &&
						(point.mPointEndType == PointType::OPEN_ENDED));
					isInsidePolygon = true;
					for (size_t i = 0; i < pointsCount; ++i)
					{
						const auto& a = polygon[i];
						const auto& b = polygon[(i + 1) % pointsCount];
						const auto& c = polygon[(i + 2) % pointsCount];
						isInsidePolygon = strictlyInside ? IsPointInsideOfSideStrictly(a, b, c, point.mPoint)
														: IsPointInsideOfSide(a, b, c, point.mPoint);
						if (isInsidePolygon == false)
						{
							break;
						}
					}
				}
			}
			return isInsidePolygon;
		}
	}
}

#endif //_MY_CODE_VECTOR_MATH_PROJECTION_H_