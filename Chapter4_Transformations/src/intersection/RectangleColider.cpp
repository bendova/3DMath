#include "RectangleColider.h"
#include "../framework/MathUtil.h"
#include "../framework/Logging.h"
#include <algorithm>
#include <cassert>
#include <cfloat>
#include <map>

namespace MyCode
{
	void RectangleColider::AddRectangle(const Rectangle& rectangle)
	{
		mRectangles.push_back(&rectangle);
	}

	glm::vec3 RectangleColider::GetPositionThatAvoidCollisions(const Rectangle& rectangle, glm::vec3 targetCenter) const
	{
		for (const Rectangle* const r : mRectangles)
		{
			const Rectangle& other = *r;
			if (&rectangle != &other)
			{
				const bool doesTravelPathCollide = PolygonCollision::DoesTravelPathCollide(rectangle, targetCenter, other);
				if (doesTravelPathCollide)
				{
					Log("Starting from center = [%f, %f, %f]\n", rectangle.Center().x, rectangle.Center().y, rectangle.Center().z);
					Log("The desired targetCenter = [%f, %f, %f]\n", targetCenter.x, targetCenter.y, targetCenter.z);
					Log("Collides with rectangle from = [%f, %f, %f]\n", other.Center().x, other.Center().y, other.Center().z);
					
					targetCenter = PolygonCollision::GetPositionOnNearEdge(rectangle, targetCenter, other);

					Log("Valid targetCenter = [%f, %f, %f]\n", targetCenter.x, targetCenter.y, targetCenter.z);
					Log("\n");

					CollisionSanityCheck(rectangle, targetCenter, other);
				}
			}
		}
		return targetCenter;
	}

	void CollisionSanityCheck(const Rectangle& target, const glm::vec3& newTargetCenter, const Rectangle& obstacle)
	{
		const float targetInscribedCircleRadius = glm::length(target.A() - target.B()) / 2.0f;
		const float obstacleInscribedCircleRadius = glm::length(obstacle.A() - obstacle.B()) / 2.0f;
		const float minDistanceBetweenCenters = targetInscribedCircleRadius + obstacleInscribedCircleRadius;
		const float targetDistanceBetweenCenters = glm::length(obstacle.Center() - newTargetCenter);
		if (targetDistanceBetweenCenters < minDistanceBetweenCenters)
		{
			Log("Invalid targetCenter = [%f, %f, %f]\n", newTargetCenter.x, newTargetCenter.y, newTargetCenter.z);
		}
	}

	namespace PolygonCollision
	{
		bool DoPolygonsIntersect(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b)
		{
			bool doTheyOverlap = false;
			const bool overlapAB = DoPolygonProjectionsIntersect(a, b);
			if (overlapAB)
			{
				doTheyOverlap = DoPolygonProjectionsIntersect(b, a);
			}

			return doTheyOverlap;
		}

		bool DoPolygonProjectionsIntersect(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2)
		{
			const auto pointsCount = polygon1.size();
			for (unsigned i = 0; i < pointsCount; ++i)
			{
				const auto& a = polygon1[i];
				const auto& b = polygon1[(i + 1) % pointsCount];

				const auto projPolygon1 = ProjectPolygonToAxis(polygon1, a, b);
				const auto projPolygon2 = ProjectPolygonToAxis(polygon2, a, b);

				const bool intersectionOnAxis = DoColinearLineSegmentsIntersect(
					projPolygon1.first, projPolygon1.second,
					projPolygon2.first, projPolygon2.second);

				if (intersectionOnAxis == false)
				{
					return false;
				}
			}

			return true;
		}

		bool DoColinearLineSegmentsIntersect(const glm::vec3& a, const glm::vec3& b,
			const glm::vec3& c, const glm::vec3& d)
		{
			bool doTheyIntersect = (a == c && b == d);
			if (!doTheyIntersect)
			{
				const auto abLength = glm::length(b - a);
				const auto cdLength = glm::length(d - c);
				const auto acLength = glm::length(c - a);
				doTheyIntersect = (abLength > acLength || cdLength > acLength);
			}
			return (doTheyIntersect);
		}

		std::pair<glm::vec3, glm::vec3> ProjectPolygonToAxis(const std::vector<glm::vec3>& polygon,
			const glm::vec3& axisPointA, const glm::vec3& axisPointB)
		{
			float leftEdgeFactor = FLT_MAX;
			float rightEdgeFactor = FLT_MIN;

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

			const glm::vec3 projectionLeftEdge{ axisPointA + leftEdgeFactor * lineDirection };
			const glm::vec3 projectionRightEdge{ axisPointA + rightEdgeFactor * lineDirection };

			return std::make_pair(projectionLeftEdge, projectionRightEdge);
		}

		bool DoesTravelPathCollide(const Rectangle& rectangle, const glm::vec3& targetCenter, const Rectangle& obstacle)
		{
			const auto travelPathPolygon = GetTravelPathBounding(rectangle, targetCenter);
			const std::vector<glm::vec3> obstacleVertices{ obstacle.A(), obstacle.B(), obstacle.C(), obstacle.D() };
			bool doTheyOverlap = PolygonCollision::DoPolygonsIntersect(travelPathPolygon, obstacleVertices);
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

		glm::vec3 GetPositionOnNearEdge(const Rectangle& r1, const glm::vec3& targetCenter, const Rectangle& r2)
		{
			const std::vector<glm::vec3> forwardPointsOfR1 = GetNearestPoints(r1, targetCenter, r2.Center());
			const std::vector<glm::vec3> forwardPointsOfR2 = GetNearestPoints(r2, r1.Center(), r1.Center());
			
			const auto& currentCenter = r1.Center();
			const std::pair<Collision, bool> collision = GetCollision(forwardPointsOfR1, forwardPointsOfR2, currentCenter, targetCenter);
			glm::vec3 validCenter = targetCenter;
			if (collision.second)
			{ 
				validCenter = GetCenterThatAvoidCollision(currentCenter, targetCenter, collision.first);
			}
			return validCenter;
		}

		glm::vec3 GetCenterThatAvoidCollision(const glm::vec3& currentCenter, const glm::vec3& targetCenter, 
			const Collision& collision)
		{
			const glm::vec3 directionVector = targetCenter - currentCenter;
			const auto delta = glm::length(directionVector - (collision.mCollidingVertex - collision.mPointOfCollision));
			const float directionVectorLength = glm::length(directionVector);
			const float factor = (directionVectorLength - delta) / directionVectorLength;
			const glm::vec3 returnCenter = currentCenter + factor * directionVector;
			return returnCenter;
		}

		std::vector<glm::vec3> GetNearestPoints(const Rectangle& r1, const glm::vec3& targetPoint, const glm::vec3& otherCenter)
		{
			std::vector<glm::vec3> vertices{ r1.A(), r1.B(), r1.C(), r1.D() };
			std::sort(vertices.begin(), vertices.end(),
				[&targetPoint, &otherCenter](const glm::vec3& a, const glm::vec3& b)
			{
				const auto distanceFromA_ToTarget = glm::length(a - targetPoint);
				const auto distanceFromB = glm::length(b - targetPoint);
				if (distanceFromA_ToTarget == distanceFromB)
				{
					const auto distanceFromA_ToOther = glm::length(a - otherCenter);
					const auto distanceFromB_ToOther = glm::length(b - otherCenter);

					return distanceFromA_ToOther < distanceFromB_ToOther;
				}
				else
				{
					return distanceFromA_ToTarget < distanceFromB;
				}
			});
			vertices.pop_back();
			return vertices;
		}

		std::pair<Collision, bool> GetCollision(const std::vector<glm::vec3>& forwardsR1, const std::vector<glm::vec3>& forwardsR2,
			const glm::vec3& currentCenter, const glm::vec3& targetCenter)
		{
			const auto directionVector = targetCenter - currentCenter;
			std::vector<Collision> collisions = GetCollisions(forwardsR1, forwardsR2, directionVector);
			const auto collisionsR2R1 = GetCollisions(forwardsR2, forwardsR1, -directionVector);
			collisions.insert(collisions.end(), collisionsR2R1.begin(), collisionsR2R1.end());
			
			std::pair<Collision, bool> result{ Collision{ glm::vec3{ 0.0f }, glm::vec3{ 0.0f } }, false };
			if (collisions.empty() == false)
			{
				std::sort(collisions.begin(), collisions.end(),
					[&targetCenter](const Collision& a, const Collision& b)
				{
					const auto collisionDistanceA = glm::length(a.mCollidingVertex - a.mPointOfCollision);
					const auto collisionDistanceB = glm::length(b.mCollidingVertex - b.mPointOfCollision);
					if (collisionDistanceA == collisionDistanceB)
					{
						const auto distanceToTargetA = glm::length(a.mCollidingVertex - targetCenter);
						const auto distanceToTargetB = glm::length(b.mCollidingVertex - targetCenter);
						return distanceToTargetA < distanceToTargetB;
					}
					else
					{
						return collisionDistanceA < collisionDistanceB;
					}
				});

				result.first = collisions[0];
				result.second = true;
			}
			return result;
		}

		std::vector<Collision> GetCollisions(const std::vector<glm::vec3>& forwardsR1, const std::vector<glm::vec3>& forwardsR2,
			const glm::vec3& directionVector)
		{
			std::vector<Collision> collisions;
			for (const auto& vertex: forwardsR1)
			{
				const auto closestIntersection = GetClosestIntersectionPoint(vertex, vertex + directionVector, forwardsR2);
				if (closestIntersection.second)
				{
					Collision collision;
					collision.mCollidingVertex = vertex;
					collision.mPointOfCollision = closestIntersection.first;
					collisions.push_back(collision);
				}
			}
			return collisions;
		}

		std::pair<glm::vec3, bool> GetClosestIntersectionPoint(const glm::vec3& a, const glm::vec3& b, const std::vector<glm::vec3>& lineSegments)
		{
			std::pair<glm::vec3, bool> result{ glm::vec3{ FLT_MAX, FLT_MAX, FLT_MAX }, false };

			const auto lineSegmentPoints = lineSegments.size();
			for (size_t i = 0; i < lineSegmentPoints; ++i)
			{
				const auto& c = lineSegments[i];
				const auto& d = lineSegments[(i+1) % lineSegmentPoints];
				const auto intersection = MathUtil::GetLineSegmentsIntersection(a, b, c, d);
				if (intersection.second)
				{
					result.second = true;
					const auto distanceToA = glm::length(intersection.first - a);
					const auto currentMinDistanceToA = glm::length(result.first - a);
					if (distanceToA < currentMinDistanceToA)
					{
						result.first = intersection.first;
					}
				}
			}
			return result;
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
	}
}