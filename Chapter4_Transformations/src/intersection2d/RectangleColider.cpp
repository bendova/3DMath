#include "RectangleColider.h"
#include "../framework/VectorMath.h"
#include "../framework/Logging.h"
#include <algorithm>
#include <cassert>
#include <cfloat>
#include <map>

namespace MyCode
{
	using namespace VectorMath;

	void RectangleColider::AddRectangle(const Rectangle& rectangle)
	{
		mRectangles.push_back(&rectangle);
	}

	void RectangleColider::AddRectangles(const std::vector<Rectangle>& rectangles)
	{
		for (const auto& rectangle : rectangles)
		{
			mRectangles.push_back(&rectangle);
		}
	}

	glm::vec3 RectangleColider::GetPositionThatAvoidsCollisions(const Rectangle& rectangle, glm::vec3 targetCenter) const
	{
		using namespace Intersection2D;
		using namespace RectangleColiderHelpers;

		bool wasCollisionFound = false;
		std::vector<const Rectangle*> sortedRectangles = SortByDistanceFromPoint(mRectangles, rectangle.Center());
		for (const Rectangle* const r : sortedRectangles)
		{
			const Rectangle& obstacle = *r;
			if (&rectangle != &obstacle)
			{
				const bool doesTravelPathCollide = TravelPathBounding::DoesTravelPathCollide(rectangle, targetCenter, obstacle);
				if (doesTravelPathCollide)
				{
					wasCollisionFound = true;
					targetCenter = GetValidCenter(rectangle, obstacle, targetCenter);
					CollisionSanityCheck(rectangle, targetCenter, obstacle);
				}
			}
		}

		if (wasCollisionFound == false)
		{
			Log("Starting from center = [%f, %f, %f]\n", rectangle.Center().x, rectangle.Center().y, rectangle.Center().z);
			Log("No collision was found for targetCenter = [%f, %f, %f]\n", targetCenter.x, targetCenter.y, targetCenter.z);
		}

		return targetCenter;
	}

	namespace RectangleColiderHelpers
	{
		std::vector<const Rectangle*> SortByDistanceFromPoint(std::vector<const Rectangle*> rectangles, const glm::vec3& point)
		{
			std::sort(rectangles.begin(), rectangles.end(),
				[&point](const Rectangle* a, const Rectangle* b)
			{
				const auto distanceFromA_toPoint = glm::length(point - a->Center());
				const auto distanceFromB_toPoint = glm::length(point - b->Center());
				return distanceFromA_toPoint < distanceFromB_toPoint;
			});

			return rectangles;
		}

		glm::vec3 GetValidCenter(const Rectangle& rectangle, const Rectangle& obstacle, const glm::vec3& targetCenter)
		{
			Log("GetValidCenter() rectangle.Center() = [%f, %f, %f]\n", rectangle.Center().x, rectangle.Center().y, rectangle.Center().z);
			Log("GetValidCenter() targetCenter = [%f, %f, %f]\n", targetCenter.x, targetCenter.y, targetCenter.z);
			Log("GetValidCenter() obstacle.Center() = [%f, %f, %f]\n", obstacle.Center().x, obstacle.Center().y, obstacle.Center().z);

			using namespace Intersection2D;
			using namespace CollisionAvoidance;

			const std::vector<glm::vec3> verticesOfR1{ rectangle.A(), rectangle.B(), rectangle.C(), rectangle.D() };
			const std::vector<glm::vec3> verticesOfR2{ obstacle.A(), obstacle.B(), obstacle.C(), obstacle.D() };
			const glm::vec3& currentCenter{ rectangle.Center() };

			glm::vec3 validCenter{ targetCenter };
			if (PolygonIntersection::DoPolygonsIntersect2D(verticesOfR1, verticesOfR2))
			{
				const glm::vec3 directionVector{ targetCenter - currentCenter };
				const glm::vec3 centersVector{ currentCenter - obstacle.Center() };
				const bool isTargetDirectionCorrect = (glm::dot(directionVector, centersVector) > 0.0f);
				if (isTargetDirectionCorrect == false)
				{
					const glm::vec3 inverseTarget = currentCenter - directionVector;
					validCenter = GetPositionOnNearEdge(verticesOfR1, currentCenter,
						inverseTarget, verticesOfR2, CollisionAvoider::Avoidance::INSIDE_OUT);
				}
			}
			else
			{
				validCenter = GetPositionOnNearEdge(verticesOfR1, currentCenter,
					targetCenter, verticesOfR2, CollisionAvoider::Avoidance::OUTSIDE_IN);
			}

			Log("GetValidCenter() validCenter = [%f, %f, %f]\n\n", validCenter.x, validCenter.y, validCenter.z);

			return validCenter;
		}

		void CollisionSanityCheck(const Rectangle& target, const glm::vec3& newTargetCenter, const Rectangle& obstacle)
		{
			const float targetInscribedCircleRadius = glm::length(target.A() - target.B()) / 2.0f;
			const float obstacleInscribedCircleRadius = glm::length(obstacle.A() - obstacle.B()) / 2.0f;
			const float minDistanceBetweenCenters = targetInscribedCircleRadius + obstacleInscribedCircleRadius;
			const float targetDistanceBetweenCenters = glm::length(obstacle.Center() - newTargetCenter);

			Log("CollisionSanityCheck() targetDistanceBetweenCenters = [%f]\n", targetDistanceBetweenCenters);

			const double errorMargin = -1e-6;
			const double delta = targetDistanceBetweenCenters - minDistanceBetweenCenters;
			if ((delta < 0) && (delta < errorMargin))
			{
				Log("CollisionSanityCheck() Invalid targetCenter = [%f, %f, %f]\n", newTargetCenter.x, newTargetCenter.y, newTargetCenter.z);
			}
		}
	}

	namespace Intersection2D
	{
		namespace PolygonIntersection
		{
			bool DoPolygonsIntersect2D(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b)
			{
				using namespace Detail;

				bool doTheyOverlap = false;

				const IntersectionType intersectionType = GetIntersectionType(a, b);
				switch (intersectionType)
				{
				case IntersectionType::POLYGON_WITH_POLYGON:
					doTheyOverlap = PolygonsIntersection::DoCoplanarPolygonsIntersect(a, b);
					break;
				case IntersectionType::POLYGON_WITH_LINE_SEGMENT:
					doTheyOverlap = DoPolygonWithLineSegmentIntersection(a, b);
					break;
				case IntersectionType::LINE_SEGMENT_WITH_POLYGON:
					doTheyOverlap = DoPolygonWithLineSegmentIntersection(b, a);
					break;
				case IntersectionType::LINE_SEGMENT_WITH_LINE_SEGMENT:
					doTheyOverlap = DoLineSegmentsIntersection(a, b);
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
					std::pair<float, float> edgeFactors = PolygonsIntersection::ProjectPolygonToAxis(collinearPoints, 
						distinctPoints[0], distinctPoints[1]);
					const glm::vec3 directionVector = distinctPoints[1] - distinctPoints[0];
					const glm::vec3 leftEdge = distinctPoints[0] + edgeFactors.first * directionVector;
					const glm::vec3 rightEdge = distinctPoints[0] + edgeFactors.second * directionVector;
					return std::make_pair(leftEdge, rightEdge);
				}

				bool DoPolygonWithLineSegmentIntersection(const std::vector<glm::vec3>& polygon, 
					const std::vector<glm::vec3>& collinearPoints)
				{
					const auto lineSegment = GetLineSegmentFromCollinearPoints(collinearPoints);
					const VectorMath::MarginPoint<glm::vec3> a{ lineSegment.first, true, true };
					const VectorMath::MarginPoint<glm::vec3> b{ lineSegment.second, true, true };
					const bool doTheyIntersect = VectorMath::GetIntersectionOfLineWithPolygon2D(a, b, polygon).second;
					return doTheyIntersect;
				}

				bool DoLineSegmentsIntersection(const std::vector<glm::vec3>& collinearPointsA, 
					const std::vector<glm::vec3>& collinearPointsB)
				{
					const auto lineSegmentA = GetLineSegmentFromCollinearPoints(collinearPointsA);
					const auto lineSegmentB = GetLineSegmentFromCollinearPoints(collinearPointsB);
					const bool doTheyIntersect = VectorMath::DoLineSegmentsIntersect(lineSegmentA.first, lineSegmentA.second,
						lineSegmentB.first, lineSegmentB.second);
					return doTheyIntersect;
				}

				namespace PolygonsIntersection
				{
					bool DoCoplanarPolygonsIntersect(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b)
					{
						bool doTheyOverlap = DoPolygonsSideIntersection(a, b);
						if (doTheyOverlap)
						{
							doTheyOverlap = DoPolygonsSideIntersection(b, a);
						}
						return doTheyOverlap;
					}

					bool DoPolygonsSideIntersection(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2)
					{
						bool doTheyIntersect = true;
						const auto pointsCount = polygon1.size();
						for (unsigned i = 0; i < pointsCount; ++i)
						{
							const auto& a = polygon1[i];
							const auto& b = polygon1[(i + 1) % pointsCount];

							doTheyIntersect = DoPolygonToAxisIntersection(polygon1, polygon2, a, b);

							if (doTheyIntersect == false)
							{
								break;
							}
						}

						return doTheyIntersect;
					}

					bool DoPolygonToAxisIntersection(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2,
						const glm::vec3& axisA, const glm::vec3& axisB)
					{
						const auto projPolygon1 = ProjectPolygonToAxis(polygon1, axisA, axisB);
						const auto projPolygon2 = ProjectPolygonToAxis(polygon2, axisA, axisB);

						const bool doTheyIntersect = DoColinearLineSegmentsIntersect(
							projPolygon1.first, projPolygon1.second,
							projPolygon2.first, projPolygon2.second);

						return doTheyIntersect;
					}

					bool DoColinearLineSegmentsIntersect(const float& factorA, const float& factorB,
						const float& factorC, const float& factorD)
					{
						const bool doTheyIntersect = DoSegmentsIntersect(factorA, factorB, factorC, factorD)
							|| DoSegmentsIntersect(factorC, factorD, factorA, factorB);
						return (doTheyIntersect);
					}

					bool DoSegmentsIntersect(const float& factorA, const float& factorB,
						const float& factorC, const float& factorD)
					{
						return !((factorC >= factorB) || (factorA >= factorD));
					}

					std::pair<float, float> ProjectPolygonToAxis(const std::vector<glm::vec3>& polygon,
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
						return std::make_pair(leftEdgeFactor, rightEdgeFactor);
					}
				}
			}
		}

		namespace TravelPathBounding
		{
			bool DoesTravelPathCollide(const Rectangle& rectangle, const glm::vec3& targetCenter, const Rectangle& obstacle)
			{
				const auto travelPathPolygon = TravelPathBounding::GetTravelPathBounding(rectangle, targetCenter);
				const std::vector<glm::vec3> obstacleVertices{ obstacle.A(), obstacle.B(), obstacle.C(), obstacle.D() };
				bool doTheyOverlap = PolygonIntersection::DoPolygonsIntersect2D(travelPathPolygon, obstacleVertices);
				return doTheyOverlap;
			}

			std::vector<glm::vec3> GetTravelPathBounding(const Rectangle& rectangle, const glm::vec3& targetCenter)
			{
				const auto& currentCenter = rectangle.Center();
				std::vector<glm::vec3> vertices{ rectangle.A(), rectangle.B(), rectangle.C(), rectangle.D() };
				auto indices = GetSortedIndicesByDistanceFromPoint(vertices, targetCenter);

				const glm::vec3 directionVector = targetCenter - currentCenter;

				std::vector<glm::vec3> travelPathBoundingRectangle;
				bool isBoundingPathRectangle = IsBoundingPathRectangle(vertices, directionVector);
				if (isBoundingPathRectangle)
				{
					travelPathBoundingRectangle = GetBoundingPathAsRectangle(vertices, indices, directionVector);
				}
				else
				{
					travelPathBoundingRectangle = GetBoundingPathAsHexagon(vertices, indices, directionVector);
				}

				return travelPathBoundingRectangle;
			}

			std::vector<int> GetSortedIndicesByDistanceFromPoint(const std::vector<glm::vec3>& vertices,
				const glm::vec3& targetPoint)
			{
				std::vector<int> indices{ 0, 1, 2, 3 };
				std::sort(indices.begin(), indices.end(),
					[&vertices, &targetPoint](const int i, const int j)
				{
					return glm::length(vertices[i] - targetPoint) < glm::length(vertices[j] - targetPoint);
				});
				return indices;
			}

			bool IsBoundingPathRectangle(const std::vector<glm::vec3>& vertices,
				const glm::vec3& directionVector)
			{
				const glm::vec3 ab = vertices[1] - vertices[0];
				const glm::vec3 bc = vertices[2] - vertices[1];

				const float dotAbDirection = glm::dot(ab, directionVector);
				const float dotBcDirection = glm::dot(bc, directionVector);
				const bool isDirectionNormalToAB = (dotAbDirection == 0.0f);
				const bool isDirectionNormalToBC = (dotBcDirection == 0.0f);

				return (isDirectionNormalToAB || isDirectionNormalToBC);
			}

			std::vector<glm::vec3> GetBoundingPathAsRectangle(std::vector<glm::vec3> vertices,
				const std::vector<int> indices, const glm::vec3& directionVector)
			{
				vertices[indices[0]] += directionVector;
				vertices[indices[1]] += directionVector;

				const std::vector<glm::vec3> travelPathBoundingRectangle{ vertices.begin(), vertices.end() };
				return travelPathBoundingRectangle;
			}

			std::vector<glm::vec3> GetBoundingPathAsHexagon(const std::vector<glm::vec3>& vertices,
				const std::vector<int>& indices, const glm::vec3& directionVector)
			{
				const int hexagonVerticesCount = 6;
				std::vector<glm::vec3> travelPathBoundingRectangle{ static_cast<unsigned>(hexagonVerticesCount) };

				const int rectangleVerticesCount = 4;
				const int triangleVerticesCount = 3;
				int j = 0;
				for (int i = indices[0] + 1; j < triangleVerticesCount; ++j)
				{
					i %= rectangleVerticesCount;
					travelPathBoundingRectangle[j] = vertices[i];
					++i;
				}

				for (int i = indices[3] + 1; j < hexagonVerticesCount; ++j)
				{
					i %= rectangleVerticesCount;
					travelPathBoundingRectangle[j] = vertices[i] + directionVector;
					++i;
				}

				return travelPathBoundingRectangle;
			}
		}

		namespace CollisionAvoidance
		{
			glm::vec3 GetPositionOnNearEdge(const std::vector<glm::vec3>& verticesOfR1, const glm::vec3& currentCenter,
				const glm::vec3& targetCenter, const std::vector<glm::vec3>& verticesOfR2, const CollisionAvoider::Avoidance avoidance)
			{
				CollisionAvoider avoider{ verticesOfR1, verticesOfR2, currentCenter, targetCenter, avoidance };
				glm::vec3 validCenter = avoider.GetValidCenter();
				return validCenter;
			}

			CollisionAvoider::CollisionAvoider(const std::vector<glm::vec3>& verticesR1, const std::vector<glm::vec3>& verticesR2,
				const glm::vec3& currentCenter, const glm::vec3& targetCenter, const Avoidance avoidance)
				: mVerticesR1(verticesR1)
				, mVerticesR2(verticesR2)
				, mCurrentCenter(currentCenter)
				, mTargetCenter(targetCenter)
				, mDirectionVector(targetCenter - currentCenter)
				, mValidCenter(0.0f)
				, mAvoidance(avoidance)
			{
				DetermineValidCenter();
			}

			void CollisionAvoider::DetermineValidCenter()
			{
				std::pair<Collision, bool> collisionPair = GetNearEdgeCollision();
				if (collisionPair.second)
				{
					mValidCenter = GetCenterThatAvoidsCollision(mCurrentCenter, collisionPair.first);
				}
				else
				{
					mValidCenter = mCurrentCenter;
				}
			}

			std::pair<Collision, bool> CollisionAvoider::GetNearEdgeCollision()
			{
				std::vector<Collision> collisions{ GetCollisionsFromBothPolygons() };
				std::pair<Collision, bool> nearEdgeCollision{ Collision{ glm::vec3{ 0.0f }, glm::vec3{ 0.0f } }, false };
				if (collisions.empty() == false)
				{
					SortAscending(collisions, mTargetCenter);
					nearEdgeCollision.first = (mAvoidance == Avoidance::OUTSIDE_IN) ? collisions.front() : collisions.back();
					nearEdgeCollision.second = true;
				}
				else
				{
					Log("CollisionAvoidance::GetNearEdgeCollision() No collision was actually found!\n This should NOT happen.\n");
				}
				return nearEdgeCollision;
			}

			std::vector<Collision> CollisionAvoider::GetCollisionsFromBothPolygons()
			{
				std::vector<Collision> collisions{ GetCollisions(mVerticesR1, mVerticesR2, mDirectionVector) };
				std::vector<Collision> collisionsR2R1 = GetCollisions(mVerticesR2, mVerticesR1, -mDirectionVector);
				for (auto& collision : collisionsR2R1)
				{
					std::swap(collision.mPointOfCollision, collision.mCollidingVertex);
				}
				collisions.insert(collisions.end(), collisionsR2R1.begin(), collisionsR2R1.end());
				return collisions;
			}

			std::vector<Collision> CollisionAvoider::GetCollisions(const std::vector<glm::vec3>& verticesR1, 
				const std::vector<glm::vec3>& verticesR2,
				const glm::vec3& directionVector) const
			{
				const bool insidePointsOnly = (mAvoidance == Avoidance::INSIDE_OUT);

				std::vector<Collision> collisions;
				for (const auto& vertex : verticesR1)
				{
					if (insidePointsOnly && (VectorMath::IsPointInsidePolygon(verticesR2, vertex) == false))
					{
						continue;
					}

					const auto intersection = GetClosestIntersectionPoint(vertex, vertex + directionVector, verticesR2);
					if (intersection.second)
					{
						Collision collision;
						collision.mCollidingVertex = vertex;
						collision.mPointOfCollision = intersection.first;
						collisions.push_back(collision);
					}
				}
				return collisions;
			}

			std::pair<glm::vec3, bool> CollisionAvoider::GetClosestIntersectionPoint(const glm::vec3& a, const glm::vec3& b,
				const std::vector<glm::vec3>& lineSegments) const
			{
				std::pair<glm::vec3, bool> result{ glm::vec3{ FLT_MAX, FLT_MAX, FLT_MAX }, false };
				float currentMinDistanceToA = FLT_MAX;

				VectorMath::MarginPoint<glm::vec3> marginA{ a, true, true };
				VectorMath::MarginPoint<glm::vec3> marginB{ b, false, true };

				const auto lineSegmentPoints = lineSegments.size();
				for (size_t i = 0; i < lineSegmentPoints; ++i)
				{
					const auto& c = lineSegments[i];
					const auto& d = lineSegments[(i + 1) % lineSegmentPoints];
					
					VectorMath::MarginPoint<glm::vec3> marginC{ c, true, true };
					VectorMath::MarginPoint<glm::vec3> marginD{ d, true, true };
					const auto intersection = GetLinesIntersection(marginA, marginB, marginC, marginD);
					if (intersection.second)
					{
						result.second = true;
						const auto distanceToA = glm::length(intersection.first - a);
						if (distanceToA < currentMinDistanceToA)
						{
							currentMinDistanceToA = distanceToA;
							result.first = intersection.first;
						}
					}
				}
				return result;
			}

			void CollisionAvoider::SortAscending(std::vector<Collision>& collisions, const glm::vec3& biasPoint) const
			{
				std::sort(collisions.begin(), collisions.end(),
					[&biasPoint](const Collision& a, const Collision& b)
				{
					const auto collisionDistanceA = glm::length(a.mCollidingVertex - a.mPointOfCollision);
					const auto collisionDistanceB = glm::length(b.mCollidingVertex - b.mPointOfCollision);
					if (collisionDistanceA == collisionDistanceB)
					{
						const auto distanceToTargetA = glm::length(a.mCollidingVertex - biasPoint);
						const auto distanceToTargetB = glm::length(b.mCollidingVertex - biasPoint);
						return distanceToTargetA < distanceToTargetB;
					}
					else
					{
						return collisionDistanceA < collisionDistanceB;
					}
				});
			}

			glm::vec3 CollisionAvoider::GetCenterThatAvoidsCollision(const glm::vec3& currentCenter, const Collision& collision) const
			{
				const glm::vec3 vectorToNearEdgePoint = collision.mPointOfCollision - collision.mCollidingVertex;
				const glm::vec3 returnCenter = currentCenter + vectorToNearEdgePoint;
				return returnCenter;
			}
		 }
	}
}