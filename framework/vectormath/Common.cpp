#include "Common.h"
#include "Intersection.h"

namespace MyCode
{
	namespace VectorMath
	{
		bool operator==(const Plane& a, const Plane& b)
		{
			return (a.mNormalToPlane == b.mNormalToPlane)
				&& (a.mPointInPlane == b.mPointInPlane);
		}

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

		bool IsInInterval(const float p, float a, float b)
		{
			return ((a <= p) && (p <= b));
		}

		float GetDistanceFromPointToLine(const glm::vec3& p, const glm::vec3& pointOnLine, const glm::vec3& lineDirection)
		{
			const float lineDirectionLength = glm::length(lineDirection);
			glm::vec3 ap = p - pointOnLine;
			float shadowLength = glm::dot(lineDirection, ap) / lineDirectionLength;
			shadowLength = FloorWithPrecision(shadowLength, 6);
			const float apLength = glm::length(ap);
			float distanceToLine = sqrt(apLength * apLength - shadowLength * shadowLength);
			distanceToLine = FloorWithPrecision(distanceToLine, 6);
			return distanceToLine;
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

		glm::vec3 GetNormalToPolygonPlane(const std::vector<glm::vec3>& polygon)
		{
			const auto& a = polygon[0];
			const auto& b = polygon[1];
			const auto& c = polygon[2];
			return glm::cross(b - a, c - a);
		}

		bool IsPointCoplanarWithPolygon(const glm::vec3& p, const std::vector<glm::vec3>& polygon)
		{
			const auto& a = polygon[0];
			const glm::vec3 normalToPlane{ GetNormalToPolygonPlane(polygon) };
			const glm::vec3 ap = p - a;
			const bool isInPlane = AreEqualWithMargin(glm::dot(normalToPlane, ap), 0.0f);
			return isInPlane;
		}

		bool IsLineInPlane(const glm::vec3& a, const glm::vec3& b, const Plane& plane)
		{
			const glm::vec3 ap = plane.mPointInPlane - a;
			const glm::vec3 bp = plane.mPointInPlane - b;
			const bool isInPlane = (AreEqualWithMargin(glm::dot(plane.mNormalToPlane, ap), 0.0f) &&
									AreEqualWithMargin(glm::dot(plane.mNormalToPlane, bp), 0.0f));
			return isInPlane;
		}

		bool DoesRayIntersectPolygon(const glm::vec3& origin, const glm::vec3& direction,
			const std::vector<glm::vec3>& polygon)
		{
			const MarginPoint<glm::vec3> originPoint{origin, BoundingPointType::BOUNDED, PointType::OPEN_ENDED};
			const MarginPoint<glm::vec3> directionVector{direction, BoundingPointType::UNBOUNDED};

			const auto vectorIntersection = GetIntersectionBetweenLineAndPolygon(originPoint, directionVector, polygon);
			return vectorIntersection.second;
		}

		Plane GetPolygonPlane(const std::vector<glm::vec3>& polygon)
		{
			return Plane{ polygon[0], GetNormalToPolygonPlane(polygon) };
		}

		bool ArePolygonsCoplanar(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2)
		{
			const Plane plane1 = GetPolygonPlane(polygon1);
			const glm::vec3 vectorBetweenPolygonPlanes = polygon2[0] - plane1.mPointInPlane;
			const bool areCoplanar = AreEqualWithMargin(glm::dot(vectorBetweenPolygonPlanes, plane1.mNormalToPlane), 0.0f);
			return areCoplanar;
		}

		bool IsFullSegment(const std::vector<float>& factors)
		{
			const float LEFT_MARGIN = 0.0f;
			const float RIGHT_MARGIN = 1.0f;
			bool isFullSegment = false;
			bool hasLeftMargin = false;
			bool hasRightMargin = false;
			for (const auto& factor : factors)
			{
				if (factor == LEFT_MARGIN)
				{
					hasLeftMargin = true;
				}
				else if (factor == RIGHT_MARGIN)
				{
					hasRightMargin = true;
				}
				if (hasLeftMargin && hasRightMargin)
				{
					isFullSegment = true;
					break;
				}
			}
			return isFullSegment;
		}

		bool AreColinearSegmentsEqual(const std::vector<float>& factorsAB, const std::vector<float>& factorsCD)
		{
			const bool isFullSegmentAB = IsFullSegment(factorsAB);
			const bool isFullSegmentCD = IsFullSegment(factorsCD);
			return (isFullSegmentAB && isFullSegmentCD);
		}

		bool IsSegment(const std::vector<float>& factors)
		{
			bool isSegment = false;
			if (factors.size() > 1)
			{
				const float factorA = factors[0];
				for (size_t i = 1; i < factors.size(); ++i)
				{
					if (factors[i] != factorA)
					{
						isSegment = true;
						break;
					}
				}
			}
			return isSegment;
		}

		bool DoColinearSegmentsOverlap(const std::vector<float>& factorsAB, const std::vector<float>& factorsCD)
		{
			const bool isSegmentAB = IsSegment(factorsAB);
			const bool isSegmentCD = IsSegment(factorsCD);
			return (isSegmentAB && isSegmentCD);
		}
	}
}