#ifndef _MY_CODE_VECTOR_MATH_H_
#define _MY_CODE_VECTOR_MATH_H_

#include <cmath>
#include <glm/glm.hpp>
#include <initializer_list>
#include <vector>

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

		float FloorWithPrecision(const float x, const int precision);
		double FloorWithPrecision(const double x, const int precision);
		void Floor(glm::vec3& v, const int precision = 5);
		void Floor(glm::vec4& v, const int precision = 5);

		float IsIntersectionFactorOnSegment(const float factor, const bool strictly = false);

		float GetDistanceFromPointToLine(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b);
		
		glm::vec3 GetProjectionPointOnPlane(const glm::vec3& pointToProject, const glm::vec3& pointInPlane, const glm::vec3& planeNormal);
		float GetDistanceBetweenPointAndPlane(const glm::vec3& pointToProject, const glm::vec3& pointInPlane, const glm::vec3& planeNormal);

		bool IsPointInPlane(const glm::vec4& planePointA, const glm::vec4& planePointB,
			const glm::vec4& planePointC, const glm::vec4& point);
		bool IsPointInPlane(const glm::vec3& planePointA, const glm::vec3& planePointB,
			const glm::vec3& planePointC, const glm::vec3& point);

		bool ArePointsCollinear(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
		bool DoesLineSegmentIntersectPolygon(const glm::vec3& a, const glm::vec3& b, const std::vector<glm::vec3>& polygon,
			const bool strictly = false);

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
		bool DoLineSegmentsIntersect(const T& a, const T& b, const T& c, const T& d, const bool strictly = false)
		{
			bool doTheyIntersect = false;

			const auto factorAB = GetSegmentIntersectionFactor(a, b, c, d);
			const auto factorCD = GetSegmentIntersectionFactor(c, d, a, b);
			if (factorAB.second && IsIntersectionFactorOnSegment(factorAB.first, strictly) &&
				factorCD.second && IsIntersectionFactorOnSegment(factorCD.first, strictly))
			{
				doTheyIntersect = true;
			}
			return doTheyIntersect;
		}

		template<typename T>
		std::pair<T, bool> GetLineSegmentsIntersection(const T& a, const T& b, const T& c, const T& d)
		{
			std::pair<T, bool> intersection{ T{ 0.0f }, false };

			const auto factorAB = GetSegmentIntersectionFactor(a, b, c, d);
			const auto factorCD = GetSegmentIntersectionFactor(c, d, a, b);
			if (factorAB.second && IsIntersectionFactorOnSegment(factorAB.first) &&
				factorCD.second && IsIntersectionFactorOnSegment(factorCD.first))
			{
				intersection.first = a + factorAB.first * (b - a);
				intersection.second = true;
			}
			return intersection;
		}

		template<typename T>
		std::pair<T, bool> GetIntersectionPointBetweenLines(const T& a, const T& b, const T& c, const T& d)
		{
			std::pair<T, bool> intersection{ T{ 0.0f }, false };

			const auto factorAB = GetSegmentIntersectionFactor(a, b, c, d);
			const auto factorCD = GetSegmentIntersectionFactor(c, d, a, b);
			if (factorAB.second && factorCD.second)
			{
				intersection.first = a + factorAB.first * (b - a);
				intersection.second = true;
			}
			return intersection;
		}

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
		bool IsPointInsidePolygon(const std::vector<T>& polygon, const T& point)
		{
			bool isInsidePolygon = false;
			const auto pointsCount = polygon.size();
			if (pointsCount > 2)
			{
				const bool isInSamePlane = IsPointInPlane(point, polygon[0], polygon[1], polygon[2]);
				if (isInSamePlane)
				{
					isInsidePolygon = true;
					for (size_t i = 0; i < pointsCount; ++i)
					{
						const auto& a = polygon[i];
						const auto& b = polygon[(i + 1) % pointsCount];
						const auto& c = polygon[(i + 2) % pointsCount];
						isInsidePolygon = IsPointInsideOfSide(a, b, c, point);
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

#endif //_MY_CODE_VECTOR_MATH_H_