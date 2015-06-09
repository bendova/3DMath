#ifndef _MY_CODE_VECTOR_MATH_H_
#define _MY_CODE_VECTOR_MATH_H_

#include <cmath>
#include <glm/glm.hpp>
#include <initializer_list>
#include <vector>
#include <algorithm>

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

		bool AreSegmentsEqualWithinMargin(const std::pair<glm::vec3, glm::vec3>& ab, const std::pair<glm::vec3, glm::vec3>& cd,
			const double margin = 1e-6);
		bool AreVectorsEqualWithinMargin(const glm::vec3& a, const glm::vec3& b, const double margin = 1e-6);
		bool AreVectorsEqualWithinMargin(const glm::vec4& a, const glm::vec4& b, const double margin = 1e-6);

		float FloorWithPrecision(const float x, const int precision);
		double FloorWithPrecision(const double x, const int precision);
		void Floor(glm::vec3& v, const int precision = 5);
		void Floor(glm::vec4& v, const int precision = 5);
		bool AreEqualWithMargin(const float a, const float b, const float errorMargin = 1e-5);

		float IsIntersectionFactorOnSegment(const float factor, const bool strictly = false);

		float GetDistanceFromPointToLine(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b);
		
		glm::vec3 GetProjectionPointOnPlane(const glm::vec3& pointToProject, const glm::vec3& pointInPlane, const glm::vec3& planeNormal);
		float GetDistanceBetweenPointAndPlane(const glm::vec3& pointToProject, const glm::vec3& pointInPlane, const glm::vec3& planeNormal);

		bool IsPointInPlane(const glm::vec4& planePointA, const glm::vec4& planePointB,
			const glm::vec4& planePointC, const glm::vec4& point);
		bool IsPointInPlane(const glm::vec3& planePointA, const glm::vec3& planePointB,
			const glm::vec3& planePointC, const glm::vec3& point);

		bool ArePointsCollinear(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
		bool ArePointsCollinear(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d);

		std::pair<float, float> GetMinMaxLengthsPair(const std::vector<glm::vec3>& points);

		template<typename T>
		float GetColinearSegmentsIntersectionFactor(const T& a, const T& b, const T& c, const T& d)
		{
			const T lineDirection = b - a;
			const T::value_type lineDirectionLengthSquared = glm::dot(lineDirection, lineDirection);
			const T ac = c - a;
			const float T::value_type factor = glm::dot(lineDirection, vectorFromLineToPoint) / lineDirectionLengthSquared;
			return factor;
		}

		template<typename T>
		std::pair<float, bool> GetIntersectionFactor1D(const T& a, const T& b, const T& c, const T& d)
		{
			const auto ab = b - a;
			const float abDot = glm::dot(ab, ab);
			const float factorA = 0.0f;
			const float factorB = 1.0f;
			
			const auto ac = c - a;
			const float factorC = glm::dot(ab, ac) / abDot;
			
			const auto ad = d - a;
			const float factorD = glm::dot(ab, ad) / abDot;
			
			const bool isValidFactorA = (factorA >= factorC) && (factorA <= factorD);
			const bool isValidFactorB = (factorB >= factorC) && (factorB <= factorD);
			const bool isValidFactorC = (factorC >= factorA) && (factorC <= factorB);
			const bool isValidFactorD = (factorD >= factorA) && (factorD <= factorB);
			
			std::vector<std::pair<float, bool>> factors;
			factors.emplace_back(factorA, isValidFactorA);
			factors.emplace_back(factorB, isValidFactorB);
			factors.emplace_back(factorC, isValidFactorC);
			factors.emplace_back(factorD, isValidFactorD);

			std::pair<float, bool> result{ FLT_MAX, false};
			for (const std::pair<float, bool>& factorPair: factors)
			{
				if (factorPair.second && factorPair.first < result.first)
				{
					result = factorPair;
				}
			}

			return result;
		}

		template<typename T>
		std::pair<float, bool> GetIntersectionFactor(const T& a, const T& b, const T& c, const T& d)
		{
			std::pair<float, bool> intersectionFactor{ FLT_MAX , false };
			const glm::vec3 ab{ b - a };
			const glm::vec3 cd{ d - c };
			const glm::vec3 lineDirectionsCross{ glm::cross(ab, cd) };
			const auto lineDirectionsCrossLengthSquared = glm::dot(lineDirectionsCross, lineDirectionsCross);
			if (lineDirectionsCrossLengthSquared)
			{
				intersectionFactor.second = true;
				const glm::vec3 ca{ a - c };
				intersectionFactor.first = -glm::dot(glm::cross(ca, cd), lineDirectionsCross)
					/ lineDirectionsCrossLengthSquared;
			}
			else
			{
				if (ArePointsCollinear(a, b, c, d))
				{
					intersectionFactor = GetIntersectionFactor1D(a, b, c, d);
				}
			}
			return intersectionFactor;
		}

		template<typename T>
		bool DoLineSegmentsIntersect(const T& a, const T& b, const T& c, const T& d, const bool strictly = false)
		{
			bool doTheyIntersect = false;

			const auto factorAB = GetIntersectionFactor(a, b, c, d);
			const auto factorCD = GetIntersectionFactor(c, d, a, b);
			if (factorAB.second && IsIntersectionFactorOnSegment(factorAB.first, strictly) &&
				factorCD.second && IsIntersectionFactorOnSegment(factorCD.first, strictly))
			{
				doTheyIntersect = true;
			}
			return doTheyIntersect;
		}

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

		template<typename T>
		bool IsFactorValidForLeftMarginPoint(const float factor, const MarginPoint<T>& a)
		{
			bool isValid = true;
			if (a.mBoundingPointType == BoundingPointType::BOUNDED)
			{
				if (a.mPointEndType == PointType::CLOSED_ENDED)
				{
					isValid = (factor >= 0.0f);
				}
				else
				{
					isValid = (factor > 0.0f);
				}
			}
			return isValid;
		}

		template<typename T>
		bool IsFactorValidForRightMarginPoint(const float factor, const MarginPoint<T>& b)
		{
			bool isValid = true;
			if (b.mBoundingPointType == BoundingPointType::BOUNDED)
			{
				if (b.mPointEndType == PointType::CLOSED_ENDED)
				{
					isValid = (factor <= 1.0f);
				}
				else
				{
					isValid = (factor < 1.0f);
				}
			}
			return isValid;
		}

		template<typename T>
		bool IsFactorValidForMarginPoints(const float factor, const MarginPoint<T>& a, const MarginPoint<T>& b)
		{
			const bool isFactorValidForA = IsFactorValidForLeftMarginPoint(factor, a);
			const bool isFactorValidForB = IsFactorValidForRightMarginPoint(factor, b);
			return isFactorValidForA && isFactorValidForB;
		}

		template<typename T>
		std::pair<T, bool> GetLinesIntersection(const MarginPoint<T>& a, const MarginPoint<T>& b, 
			const MarginPoint<T>& c, const MarginPoint<T>& d)
		{
			std::pair<T, bool> intersection{ T{ 0.0f }, false };

			const auto factorAB = GetIntersectionFactor(a.mPoint, b.mPoint, c.mPoint, d.mPoint);
			const auto factorCD = GetIntersectionFactor(c.mPoint, d.mPoint, a.mPoint, b.mPoint);
			
			if (factorAB.second && factorCD.second)
			{
				const bool isFactorValidForAB = IsFactorValidForMarginPoints(factorAB.first, a, b);
				const bool isFactorValidForCD = IsFactorValidForMarginPoints(factorCD.first, c, d);
				if (isFactorValidForAB && isFactorValidForCD)
				{
					const T pointOnAB = a.mPoint + factorAB.first * (b.mPoint - a.mPoint);
					const T pointOnCD = c.mPoint + factorCD.first * (d.mPoint - c.mPoint);
					if (AreVectorsEqualWithinMargin(pointOnAB, pointOnCD))
					{
						intersection.first = pointOnAB;
						intersection.second = true;
					}
				}
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

		template<typename T>
		std::pair<T, bool> GetIntersectionOfLineWithPolygon2D(const MarginPoint<T>& a, const MarginPoint<T>& b, 
			const std::vector<T>& polygon)
		{
			std::pair<T, bool> intersection{ T{ 0.0f }, false };

			const auto pointsCount = polygon.size();
			for (size_t i = 0; i < pointsCount; ++i)
			{
				const MarginPoint<T> c{ polygon[i] };
				const MarginPoint<T> d{ polygon[(i + 1) % pointsCount] };
				intersection = GetLinesIntersection(a, b, c, d);
				if (intersection.second)
				{
					break;
				}
			}
			if (intersection.second == false)
			{
				if (a.mBoundingPointType == BoundingPointType::BOUNDED &&
					a.mPointEndType == PointType::CLOSED_ENDED &&
					IsPointInsidePolygon(polygon, a.mPoint))
				{
					intersection.first = a.mPoint;
					intersection.second = true;
				}
				else if (b.mBoundingPointType == BoundingPointType::BOUNDED &&
					b.mPointEndType == PointType::CLOSED_ENDED &&
					IsPointInsidePolygon(polygon, b.mPoint))
				{
					intersection.first = b.mPoint;
					intersection.second = true;
				}
			}
			return intersection;
		}

		template<typename T>
		std::pair<T, bool> GetIntersectionOfLineWithPlane3D(const MarginPoint<T>& a, const MarginPoint<T>& b,
			const T& planePoint, const T& planeNormal)
		{
			std::pair<T, bool> intersection{ T{ 0.0f }, false };

			const auto vectorPlaneToLine = planePoint - a.mPoint;
			const auto lineDirection = b.mPoint - a.mPoint;
			const auto numerator = glm::dot(planeNormal, vectorPlaneToLine);
			const auto denominator = glm::dot(planeNormal, lineDirection);
			if ((numerator == 0.0f) && (a.mBoundingPointType == BoundingPointType::BOUNDED))
			{
				// the line is in the plane
				intersection.first = a.mPoint;
				intersection.second = true;
			}
			else if (denominator == 0.0f)
			{
				// the line is parallel with the plane, and never shall the two meet
			}
			else
			{
				const auto factor = numerator / denominator;
				if (IsFactorValidForMarginPoints(factor, a, b))
				{
					intersection.first = a.mPoint + factor * lineDirection;
					intersection.second = true;
				}
			}
			return intersection;
		}

		template<typename T>
		std::pair<T, bool> GetIntersectionBetweenLineAndPolygon(const MarginPoint<T>& a, const MarginPoint<T>& b, 
			const std::vector<T>& polygon)
		{
			const T lineDirection = b.mPoint - a.mPoint;
			const T& polygonPoint = polygon[0];
			const T normalToPolygonPlane = T{ glm::cross(polygon[1] - polygonPoint, polygon[2] - polygonPoint) };
			bool isSegmentInPolygonPlane = (glm::dot(normalToPolygonPlane, lineDirection) == 0);
			
			std::pair<glm::vec3, bool> intersection{ T{0.0f}, false };
			if (isSegmentInPolygonPlane)
			{
				intersection = GetIntersectionOfLineWithPolygon2D(a, b, polygon);
			}
			else
			{
				intersection = GetIntersectionOfLineWithPlane3D(a, b, polygonPoint, normalToPolygonPlane);
				if (intersection.second)
				{
					intersection.second = IsPointInsidePolygon(polygon, intersection.first);
				}
			}
			
			return intersection;
		}
	}
}

#endif //_MY_CODE_VECTOR_MATH_H_