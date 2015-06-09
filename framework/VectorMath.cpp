#include "VectorMath.h"

namespace MyCode
{
	namespace VectorMath
	{

		bool AreSegmentsEqualWithinMargin(const std::pair<glm::vec3, glm::vec3>& ab, const std::pair<glm::vec3, glm::vec3>& cd,
			const double margin)
		{
			return (AreVectorsEqualWithinMargin(ab.first, cd.first, margin)
				&& AreVectorsEqualWithinMargin(ab.second, cd.second, margin));
		}

		bool AreVectorsEqualWithinMargin(const glm::vec3& a, const glm::vec3& b, const double margin)
		{
			return ((std::abs(a.x - b.x) <= margin)
				&& (std::abs(a.y - b.y) <= margin)
				&& (std::abs(a.z - b.z) <= margin));
		}

		bool AreVectorsEqualWithinMargin(const glm::vec4& a, const glm::vec4& b, const double margin)
		{
			return ((std::abs(a.x - b.x) <= margin)
				&& (std::abs(a.y - b.y) <= margin)
				&& (std::abs(a.z - b.z) <= margin))
				&& (std::abs(a.w - b.w) <= margin);
		}

		float IsIntersectionFactorOnSegment(const float factor, const bool strictly)
		{
			const float minFactor = 0.0f;
			const float maxFactor = 1.0f;
			
			const bool isOnSegment = (strictly) ? ((minFactor < factor) && (factor < maxFactor))
												: ((minFactor <= factor) && (factor <= maxFactor));
			return isOnSegment;
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

		bool AreEqualWithMargin(const float a, const float b, const float errorMargin)
		{
			return std::abs(a - b) <= errorMargin;
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

		glm::vec3 GetProjectionPointOnPlane(const glm::vec3& pointToProject, const glm::vec3& pointInPlane, const glm::vec3& planeNormal)
		{
			const float distance = GetDistanceBetweenPointAndPlane(pointToProject, pointInPlane, planeNormal);
			const float factor = (distance / glm::length(planeNormal));
			const glm::vec3 projectedPoint = pointToProject + factor * planeNormal;
			return projectedPoint;
		}

		float GetDistanceBetweenPointAndPlane(const glm::vec3& pointToProject, const glm::vec3& pointInPlane, const glm::vec3& planeNormal)
		{
			const glm::vec3 vectorFromPlaneToPoint = pointToProject - pointInPlane;
			const float dotValue = glm::dot(vectorFromPlaneToPoint, planeNormal);
			const float distance = -dotValue / glm::length(planeNormal);
			return distance;
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

		bool ArePointsCollinear(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
		{
			const glm::vec3 ab = b - a;
			const glm::vec3 ac = c - a;
			const auto dotValue = glm::dot(ab, ac);
			const float lengthsProduct = (glm::length(ab) * glm::length(ac));
			const bool areCollinear = (AreEqualWithMargin(std::abs(dotValue), lengthsProduct));
			return areCollinear;
		}

		bool ArePointsCollinear(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d)
		{
			return ArePointsCollinear(a, b, c) && ArePointsCollinear(a, b, d);
		}

		std::pair<float, float> GetMinMaxLengthsPair(const std::vector<glm::vec3>& points)
		{
			float minLength = FLT_MAX;
			float maxLength = -FLT_MAX;
			for (const auto& point: points)
			{
				const auto pointLength = glm::length(point);
				if (pointLength > maxLength)
				{
					maxLength = pointLength;
				}
				if (pointLength < minLength)
				{
					minLength = pointLength;
				}
			}
			return std::make_pair(minLength, maxLength);
		}
	}
}