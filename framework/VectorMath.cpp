#include "VectorMath.h"

namespace MyCode
{
	namespace VectorMath
	{
		float IsIntersectionFactorOnSegmentStrictly(const float factor)
		{
			const float minFactor = 0.0f;
			const float maxFactor = 1.0f;
			return ((minFactor < factor) && (factor < maxFactor));
		}

		float IsIntersectionFactorOnSegment(const float factor)
		{
			const float minFactor = 0.0f;
			const float maxFactor = 1.0f;
			return ((minFactor <= factor) && (factor <= maxFactor));
		}

		float FloorWithPrecision(const float x, const int precision)
		{
			const int scale = static_cast<int>(pow(10, precision));

			double temp = x * scale;
			temp = temp < 0.0 ? ceil(temp) : floor(temp);
			temp /= scale;
			return static_cast<float>(temp);
		}

		double FloorWithPrecision(const double x, const int precision)
		{
			const int scale = static_cast<int>(pow(10, precision));

			double temp = x * scale;
			temp = temp < 0.0 ? ceil(temp) : floor(temp);
			temp /= scale;
			return static_cast<float>(temp);
		}

		void Floor(glm::vec3& v, const int precision)
		{
			v.x = FloorWithPrecision(v.x, precision);
			v.y = FloorWithPrecision(v.y, precision);
			v.z = FloorWithPrecision(v.z, precision);
		}

		void Floor(glm::vec4& v, const int precision)
		{
			v.x = FloorWithPrecision(v.x, precision);
			v.y = FloorWithPrecision(v.y, precision);
			v.z = FloorWithPrecision(v.z, precision);
			v.w = FloorWithPrecision(v.w, precision);
		}

		float GetDistanceFromPointToLine(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b)
		{
			glm::vec3 lineDirection = b - a;

			const float lineDirectionLength = glm::length(lineDirection);
			glm::vec3 ap = p - a;
			float shadowLength = glm::dot(lineDirection, ap) / lineDirectionLength;
			shadowLength = FloorWithPrecision(shadowLength, 6);
			const float apLength = glm::length(ap);
			float distanceToLine = sqrt(apLength * apLength - shadowLength * shadowLength);
			distanceToLine = FloorWithPrecision(distanceToLine, 6);
			return distanceToLine;
		}

		bool DoLineSegmentsIntersectStrictly(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d)
		{
			bool doTheyIntersect = false;

			const auto factorAB = GetSegmentIntersectionFactor(a, b, c, d);
			const auto factorCD = GetSegmentIntersectionFactor(c, d, a, b);
			if (factorAB.second && IsIntersectionFactorOnSegmentStrictly(factorAB.first) &&
				factorCD.second && IsIntersectionFactorOnSegmentStrictly(factorCD.first))
			{
				doTheyIntersect = true;
			}
			return doTheyIntersect;
		}

		bool DoLineSegmentsIntersectStrictly(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& d)
		{
			const glm::vec4 intersectionPoint = GetIntersectionPointBetweenSegmentsStrictly(a, b, c, d);
			return (intersectionPoint.w != 0);
		}

		glm::vec3 GetProjectionPointOnPlane(const glm::vec3& pointToProject, const glm::vec3& pointInPlane, const glm::vec3& planeNormal)
		{
			const float distance = GetDistanceBetweenPointAndPlane(pointToProject, pointInPlane, planeNormal);
			const bool isPointOnSameSide = IsPointOnSameSideOfPlane(pointToProject, pointInPlane, planeNormal);
			const int signOfDisplacement = (isPointOnSameSide) ? -1 : 1;
			const float factor = signOfDisplacement * (distance / glm::length(planeNormal));
			const glm::vec3 projectedPoint = pointToProject + factor * planeNormal;
			return projectedPoint;
		}

		float GetDistanceBetweenPointAndPlane(const glm::vec3& pointToProject, const glm::vec3& pointInPlane, const glm::vec3& planeNormal)
		{
			const glm::vec3 vectorFromPlaneToPoint = pointToProject - pointInPlane;
			const float distance = glm::dot(vectorFromPlaneToPoint, planeNormal) / glm::length(planeNormal);
			return distance;
		}

		bool IsPointOnSameSideOfPlane(const glm::vec3& pointToProject, const glm::vec3& pointInPlane, const glm::vec3& planeNormal)
		{
			const glm::vec3 vectorFromPlaneToPoint = pointToProject - pointInPlane;
			const float dotValue = glm::dot(vectorFromPlaneToPoint, planeNormal);
			return (dotValue >= 0);
		}

		bool IsPointInPlane(const glm::vec4& planePointA, const glm::vec4& planePointB,
			const glm::vec4& planePointC, const glm::vec4& point)
		{
			return IsPointInPlane(glm::vec3{ planePointA }, glm::vec3{ planePointB },
				glm::vec3{ planePointC }, glm::vec3{ point });
		}

		bool IsPointInPlane(const glm::vec3& planePointA, const glm::vec3& planePointB,
			const glm::vec3& planePointC, const glm::vec3& point)
		{
			const glm::vec3 ab = planePointB - planePointA;
			const glm::vec3 ac = planePointC - planePointA;
			const glm::vec3 planeNormal = glm::cross(ab, ac);
			const glm::vec3 ap = point - planePointA;
			const bool isInPlane = (glm::dot(planeNormal, ap) == 0);
			return isInPlane;
		}
	}
}