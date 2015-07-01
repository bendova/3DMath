#include "PolygonIntersection.h"
#include "Logging.h"
#include "vectormath/Intersection.h"

namespace MyCode
{
	namespace Intersection2D
	{
		namespace PolygonIntersection
		{
			bool DoPolygonsIntersect2D(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b, 
				const VectorMath::PointType pointType)
			{
				using namespace Detail;

				bool doTheyOverlap = false;

				const IntersectionType intersectionType = GetIntersectionType(a, b);
				switch (intersectionType)
				{
				case IntersectionType::POLYGON_WITH_POLYGON:
					doTheyOverlap = DoCoplanarPolygonsIntersect(a, b, pointType);
					break;
				case IntersectionType::POLYGON_WITH_LINE_SEGMENT:
					doTheyOverlap = DoPolygonWithLineSegmentIntersection(a, b, pointType);
					break;
				case IntersectionType::LINE_SEGMENT_WITH_POLYGON:
					doTheyOverlap = DoPolygonWithLineSegmentIntersection(b, a, pointType);
					break;
				case IntersectionType::LINE_SEGMENT_WITH_LINE_SEGMENT:
					doTheyOverlap = DoLineSegmentsIntersection(a, b, pointType);
					break;
				}

				return doTheyOverlap;
			}

			namespace Detail
			{
				IntersectionType GetIntersectionType(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b)
				{
					IntersectionType intersectionType = IntersectionType::POLYGON_WITH_POLYGON;
					const bool isLineSegmentA = IsLineSegment(a);
					const bool isLineSegmentB = IsLineSegment(b);
					if (isLineSegmentA && isLineSegmentB)
					{
						intersectionType = IntersectionType::LINE_SEGMENT_WITH_LINE_SEGMENT;
					}
					else if (isLineSegmentA)
					{
						intersectionType = IntersectionType::LINE_SEGMENT_WITH_POLYGON;
					}
					else if (isLineSegmentB)
					{
						intersectionType = IntersectionType::POLYGON_WITH_LINE_SEGMENT;
					}
					return intersectionType;
				}

				bool IsLineSegment(const std::vector<glm::vec3>& points)
				{
					std::vector<glm::vec3> distinctPoints{ GetPairwiseDistinctPoints(points, 3) };
					bool isLineSegment = true;
					if (distinctPoints.size() == 3)
					{
						isLineSegment = VectorMath::ArePointsCollinear(distinctPoints[0],
							distinctPoints[1], distinctPoints[2]);
					}
					return isLineSegment;
				}

				std::vector<glm::vec3> GetPairwiseDistinctPoints(const std::vector<glm::vec3>& points, const int count)
				{
					std::vector<glm::vec3> distinctPoints;
					distinctPoints.push_back(points[0]);
					for (unsigned i = 1; i < points.size(); ++i)
					{
						const auto& point = points[i];
						if (point != distinctPoints.back())
						{
							distinctPoints.push_back(point);
							if (distinctPoints.size() == count)
							{
								break;
							}
						}
					}
					return distinctPoints;
				}

				std::pair<glm::vec3, glm::vec3> GetLineSegmentFromCollinearPoints(const std::vector<glm::vec3>& collinearPoints)
				{
					std::vector<glm::vec3> distinctPoints{ GetPairwiseDistinctPoints(collinearPoints, 2) };
					return ProjectPolygonToAxis(collinearPoints,
						distinctPoints[0], distinctPoints[1]);
				}

				bool DoPolygonWithLineSegmentIntersection(const std::vector<glm::vec3>& polygon,
					const std::vector<glm::vec3>& collinearPoints, const VectorMath::PointType pointType)
				{
					const auto lineSegment = GetLineSegmentFromCollinearPoints(collinearPoints);
					const bool doTheyIntersect = DoCoplanarPolygonsIntersect(polygon,
						{lineSegment.first, lineSegment.second}, pointType);

					return doTheyIntersect;
				}

				bool DoLineSegmentsIntersection(const std::vector<glm::vec3>& collinearPointsA,
					const std::vector<glm::vec3>& collinearPointsB, const VectorMath::PointType pointType)
				{
					using namespace VectorMath;

					const auto lineSegmentA = GetLineSegmentFromCollinearPoints(collinearPointsA);
					const auto lineSegmentB = GetLineSegmentFromCollinearPoints(collinearPointsB);

					const BoundingPointType boundingType = BoundingPointType::BOUNDED;
					const MarginPoint<glm::vec3> a{ lineSegmentA.first, boundingType, pointType };
					const MarginPoint<glm::vec3> b{ lineSegmentA.second, boundingType, pointType };
					const MarginPoint<glm::vec3> c{ lineSegmentB.first, boundingType, pointType };
					const MarginPoint<glm::vec3> d{ lineSegmentB.second, boundingType, pointType };

					bool doTheyIntersect = false;
					if (ArePointsCollinear(std::vector<glm::vec3>{ a.mPoint, b.mPoint, c.mPoint, d.mPoint }))
					{
						doTheyIntersect = DoCollinearLineSegmentsIntersect(a, b, c, d);
					}
					else
					{
						const auto intersection = GetLinesIntersection(a, b, c, d);
						doTheyIntersect = intersection.second;
					}

					return doTheyIntersect;
				}
				
				bool DoCoplanarPolygonsIntersect(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b,
					const VectorMath::PointType pointType)
				{
					bool doTheyOverlap = DoPolygonsSideIntersection(a, b, pointType);
					if (doTheyOverlap)
					{
						doTheyOverlap = DoPolygonsSideIntersection(b, a, pointType);
					}
					return doTheyOverlap;
				}

				bool DoPolygonsSideIntersection(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2,
					const VectorMath::PointType pointType)
				{
					bool doTheyIntersect = true;
					const auto pointsCount = polygon1.size();
					for (unsigned i = 0; i < pointsCount; ++i)
					{
						const auto& a = polygon1[i];
						const auto& b = polygon1[(i + 1) % pointsCount];

						doTheyIntersect = DoPolygonToAxisIntersection(polygon1, polygon2, a, b, pointType);

						if (doTheyIntersect == false)
						{
							break;
						}
					}

					return doTheyIntersect;
				}

				bool DoPolygonToAxisIntersection(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2,
					const glm::vec3& axisA, const glm::vec3& axisB,
					const VectorMath::PointType pointType)
				{
					using namespace VectorMath;

					const auto projPolygon1 = ProjectPolygonToAxis(polygon1, axisA, axisB);
					const auto projPolygon2 = ProjectPolygonToAxis(polygon2, axisA, axisB);

					const BoundingPointType boundingType = BoundingPointType::BOUNDED;
					const MarginPoint<glm::vec3> a{ projPolygon1.first, boundingType, pointType };
					const MarginPoint<glm::vec3> b{ projPolygon1.second, boundingType, pointType };
					const MarginPoint<glm::vec3> c{ projPolygon2.first, boundingType, pointType };
					const MarginPoint<glm::vec3> d{ projPolygon2.second, boundingType, pointType };

					bool doTheyIntersect = false;
					if (a.mPoint == b.mPoint)
					{
						doTheyIntersect = IsPointOnLineSegment(c, d, a.mPoint);
					}
					else if (c.mPoint == d.mPoint)
					{
						doTheyIntersect = IsPointOnLineSegment(a, b, c.mPoint);
					}
					else
					{
						doTheyIntersect = DoCollinearLineSegmentsIntersect(a, b, c, d);
					}
					return doTheyIntersect;
				}

				std::pair<glm::vec3, glm::vec3> ProjectPolygonToAxis(const std::vector<glm::vec3>& polygon,
					const glm::vec3& axisPointA, const glm::vec3& axisPointB)
				{
					float leftEdgeFactor = FLT_MAX;
					float rightEdgeFactor = -FLT_MAX;

					const glm::vec3 lineDirection = axisPointB - axisPointA;
					const glm::vec3::value_type lineDirectionLengthSquared = glm::dot(lineDirection, lineDirection);

					for (unsigned i = 0; i < polygon.size(); ++i)
					{
						const glm::vec3 vectorFromLineToPoint = polygon[i] - axisPointA;
						const glm::vec3::value_type factor = glm::dot(lineDirection, vectorFromLineToPoint) / lineDirectionLengthSquared;
						if (factor < leftEdgeFactor)
						{
							leftEdgeFactor = factor;
						}
						if (factor > rightEdgeFactor)
						{
							rightEdgeFactor = factor;
						}
					}
					const auto leftEdge = axisPointA + leftEdgeFactor * lineDirection;
					const auto rightEdge = axisPointA + rightEdgeFactor * lineDirection;
					return std::make_pair(leftEdge, rightEdge);
				}
			}
		}
	}
}