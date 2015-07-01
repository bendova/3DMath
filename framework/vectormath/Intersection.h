#ifndef _MY_CODE_VECTOR_MATH_INTERSECTION_H_
#define _MY_CODE_VECTOR_MATH_INTERSECTION_H_

#include <cmath>
#include <glm/glm.hpp>
#include <initializer_list>
#include <vector>
#include <algorithm>
#include "DataTypes.h"
#include "Common.h"
#include "Projection.h"

namespace MyCode
{
	namespace VectorMath
	{
		template<typename T>
		bool ArePointsCollinear(const T& a, const T& b, const T& c)
		{
			const T ab = b - a;
			const T ac = c - a;
			const auto dotValue = glm::dot(ab, ac);
			const auto lengthsProduct = (glm::length(ab) * glm::length(ac));
			const bool areCollinear = (AreEqualWithMargin(std::abs(dotValue), lengthsProduct));
			return areCollinear;
		}

		template<typename T>
		bool ArePointsCollinear(const std::vector<T>& points)
		{
			const auto& a = points[0];
			const auto& b = points[1];
			bool areCollinear = true;
			for (size_t i = 2; i < points.size(); ++i)
			{
				const auto& c = points[i];
				if (ArePointsCollinear(a, b, c) == false)
				{
					areCollinear = false;
					break;
				}
			}
			return areCollinear;
		}

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
		std::vector<float> GetIntersectionFactor1D(const T& a, const T& b, const T& c, const T& d)
		{
			const auto ab = b - a;
			const float abDot = glm::dot(ab, ab);
			const float factorA = 0.0f;
			const float factorB = 1.0f;
			
			const auto ac = c - a;
			const float factorC = glm::dot(ab, ac) / abDot;
			
			const auto ad = d - a;
			const float factorD = glm::dot(ab, ad) / abDot;
			
			std::vector<float> factors;
			if ((factorA >= factorC) && (factorA <= factorD))
			{
				factors.push_back(factorA);
			}
			if ((factorB >= factorC) && (factorB <= factorD))
			{
				factors.push_back(factorB);
			}
			if ((factorC >= factorA) && (factorC <= factorB))
			{
				factors.push_back(factorC);
			}
			if ((factorD >= factorA) && (factorD <= factorB))
			{
				factors.push_back(factorD);
			}
			std::sort(factors.begin(), factors.end());

			return factors;
		}

		template<typename T>
		std::pair<float, bool> GetIntersectionFactor(const T& a, const T& b, const T& c, const T& d)
		{
			std::pair<float, bool> intersectionFactor{ FLT_MAX, false };
			const glm::vec3 ab{ b - a };
			const glm::vec3 cd{ d - c };
			const glm::vec3 lineDirectionsCross{ glm::cross(ab, cd) };
			const auto lineDirectionsCrossLengthSquared = glm::dot(lineDirectionsCross, lineDirectionsCross);
			if (lineDirectionsCrossLengthSquared)
			{
				const glm::vec3 ca{ a - c };
				intersectionFactor.first = -glm::dot(glm::cross(ca, cd), lineDirectionsCross)
					/ lineDirectionsCrossLengthSquared;
				intersectionFactor.second = true;
			}
			return intersectionFactor;
		}

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
		std::pair<float, bool> GetValidFactor(const std::vector<float>& factors, const MarginPoint<T>& a, const MarginPoint<T>& b)
		{
			std::pair<float, bool> result{ FLT_MAX, false };
			for (const auto& factor : factors)
			{
				if (IsFactorValidForMarginPoints(factor, a, b))
				{
					result.first = factor;
					result.second = true;
					break;
				}
			}
			return result;
		}

		template<typename T>
		bool DoCollinearLineSegmentsIntersect(const MarginPoint<T>& a, const MarginPoint<T>& b,
			const MarginPoint<T>& c, const MarginPoint<T>& d)
		{
			bool doTheyIntersect = false;

			const std::vector<float> factorsAB = GetIntersectionFactor1D(a.mPoint, b.mPoint, c.mPoint, d.mPoint);
			const std::vector<float> factorsCD = GetIntersectionFactor1D(c.mPoint, d.mPoint, a.mPoint, b.mPoint);
			if ((factorsAB.empty() == false) && (factorsCD.empty() == false))
			{
				doTheyIntersect = AreColinearSegmentsEqual(factorsAB, factorsCD);
				if (doTheyIntersect == false)
				{
					const std::pair<float, bool> factorAbValid = GetValidFactor(factorsAB, a, b);
					const std::pair<float, bool> factorCdValid = GetValidFactor(factorsCD, c, d);
					doTheyIntersect = (factorAbValid.second && factorCdValid.second);
				}
			}
			return doTheyIntersect;
		}

		template<typename T>
		std::pair<T, bool> GetCollinearLinesIntersection(const MarginPoint<T>& a, const MarginPoint<T>& b,
			const MarginPoint<T>& c, const MarginPoint<T>& d)
		{
			std::pair<T, bool> intersection{ T{ 0.0f }, false };

			const std::vector<float> factorsAB = GetIntersectionFactor1D(a.mPoint, b.mPoint, c.mPoint, d.mPoint);
			const std::vector<float> factorsCD = GetIntersectionFactor1D(c.mPoint, d.mPoint, a.mPoint, b.mPoint);
			if ((factorsAB.empty() == false) && (factorsCD.empty() == false))
			{
				if (AreColinearSegmentsEqual(factorsAB, factorsCD))
				{
					intersection.first = a.mPoint + (b.mPoint - a.mPoint);
					intersection.second = true;
				}
				else
				{
					const std::pair<float, bool> factorAB = GetValidFactor(factorsAB, a, b);
					const std::pair<float, bool> factorCD = GetValidFactor(factorsCD, c, d);
					if (factorAB.second && factorCD.second)
					{
						intersection.first = a.mPoint + factorAB.first * (b.mPoint - a.mPoint);
						intersection.second = true;
					}
				}
			}

			return intersection;
		}

		template<typename T>
		bool IsPointOnLineSegment(const MarginPoint<T>& lineSegmentA, const MarginPoint<T>& lineSegmentB,
			const T& point)
		{
			bool doTheyIntersect = false;

			const std::vector<float> factorsAB = GetIntersectionFactor1D(lineSegmentA.mPoint, lineSegmentB.mPoint, point, point);
			if (factorsAB.empty() == false)
			{
				const std::pair<float, bool> factorAB = GetValidFactor(factorsAB, lineSegmentA, lineSegmentB);
				doTheyIntersect = factorAB.second;
			}
			return doTheyIntersect;
		}

		template<typename T>
		std::pair<T, bool> GetLinesIntersection(const MarginPoint<T>& a, const MarginPoint<T>& b, 
			const MarginPoint<T>& c, const MarginPoint<T>& d)
		{
			std::pair<T, bool> intersection{ T{ 0.0f }, false };

			if (ArePointsCollinear(std::vector<T> { a.mPoint, b.mPoint, c.mPoint, d.mPoint }))
			{
				intersection = GetCollinearLinesIntersection(a, b, c, d);
			}
			else
			{
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
			}

			return intersection;
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
				if (IsPointInsidePolygon(polygon, a))
				{
					intersection.first = a.mPoint;
					intersection.second = true;
				}
				else if (IsPointInsidePolygon(polygon, b))
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
			if ((numerator == 0.0f) && 
				(a.mBoundingPointType == BoundingPointType::BOUNDED) &&
				(a.mPointEndType == PointType::CLOSED_ENDED))
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
			const bool isSegmentInPolygonPlane = (glm::dot(normalToPolygonPlane, lineDirection) == 0);
			
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
					intersection.second = IsPointInsidePolygon(polygon, MarginPoint<T>{intersection.first});
				}
			}
			
			return intersection;
		}
	}
}

#endif //_MY_CODE_VECTOR_MATH_INTERSECTION_H_