#include "RectangleColider.h"
#include "../framework/MathUtil.h"
#include "../framework/Logging.h"
#include <algorithm>
#include <cassert>

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
		const float targetCircumscribedCircleRadius = glm::length(target.A() - target.B()) / 2.0f;
		const float obstacleCircumscribedCircleRadius = glm::length(obstacle.A() - obstacle.B()) / 2.0f;

		const float minDistanceBetweenCenters = targetCircumscribedCircleRadius + obstacleCircumscribedCircleRadius;
		const float currentDistanceBetweenCenters = glm::length(obstacle.Center() - target.Center());
		const float targetDistanceBetweenCenters = glm::length(obstacle.Center() - newTargetCenter);
		if (targetDistanceBetweenCenters < minDistanceBetweenCenters)
		{
			Log("Invalid targetCenter = [%f, %f, %f]\n", newTargetCenter.x, newTargetCenter.y, newTargetCenter.z);
		}
	}

	namespace PolygonCollision
	{
		bool DoPolygonsOverlap(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b)
		{
			// FIXME this doesn't work properly - not even for rectangles
			// We need implement to full Separating Axis algorithm to correctly
			// determine if two polygons overlap.

			const auto projectionA_X = ProjectPolygonToAxisX(a);
			const auto projectionA_Y = ProjectPolygonToAxisY(a);
			const auto projectionA_Z = ProjectPolygonToAxisZ(a);

			const auto projectionB_X = ProjectPolygonToAxisX(b);
			const auto projectionB_Y = ProjectPolygonToAxisY(b);
			const auto projectionB_Z = ProjectPolygonToAxisZ(b);

			const bool intersectionOnAxisX = DoLineSegmentsOverlap(
				projectionA_X.first.x, projectionA_X.second.x, 
				projectionB_X.first.x, projectionB_X.second.x);
			const bool intersectionOnAxisY = DoLineSegmentsOverlap(
				projectionA_Y.first.y, projectionA_Y.second.y,
				projectionB_Y.first.y, projectionB_Y.second.y);
			const bool intersectionOnAxisZ = DoLineSegmentsOverlap(
				projectionA_Z.first.z, projectionA_Z.second.z,
				projectionB_Z.first.z, projectionB_Z.second.z);

			return intersectionOnAxisX && intersectionOnAxisY && intersectionOnAxisZ;
		}

		bool DoLineSegmentsOverlap(const float a, const float b, const float c, const float d)
		{
			const bool doTheyOverlap =
				(a == c && b == d)
				|| !(b <= c || d <= a);
			return doTheyOverlap;
		}

		std::pair<glm::vec3, glm::vec3> ProjectPolygonToAxisX(const std::vector<glm::vec3>& polygon)
		{
			glm::vec3 projectionPointA{ polygon[0].x, 0.0f, 0.0f };
			glm::vec3 projectionPointB{ polygon[1].x, 0.0f, 0.0f };

			for (unsigned i = 2; i < polygon.size(); ++i)
			{
				if (polygon[i].x < projectionPointA.x)
				{
					projectionPointA.x = polygon[i].x;
				}
				else if (polygon[i].x > projectionPointB.x)
				{
					projectionPointB.x = polygon[i].x;
				}
			}

			std::pair<glm::vec3, glm::vec3> lineSegment{ projectionPointA, projectionPointB };
			return lineSegment;
		}
		std::pair<glm::vec3, glm::vec3> ProjectPolygonToAxisY(const std::vector<glm::vec3>& polygon)
		{
			glm::vec3 projectionPointA{ 0.0f, polygon[0].y, 0.0f };
			glm::vec3 projectionPointB{ 0.0f, polygon[1].y, 0.0f };

			for (unsigned i = 2; i < polygon.size(); ++i)
			{
				if (polygon[i].y < projectionPointA.y)
				{
					projectionPointA.y = polygon[i].y;
				}
				else if (polygon[i].y > projectionPointB.y)
				{
					projectionPointB.y = polygon[i].y;
				}
			}

			std::pair<glm::vec3, glm::vec3> lineSegment{ projectionPointA, projectionPointB };
			return lineSegment;
		}
		std::pair<glm::vec3, glm::vec3> ProjectPolygonToAxisZ(const std::vector<glm::vec3>& polygon)
		{
			glm::vec3 projectionPointA{ 0.0f, 0.0f, polygon[0].z };
			glm::vec3 projectionPointB{ 0.0f, 0.0f, polygon[1].z };

			for (unsigned i = 2; i < polygon.size(); ++i)
			{
				if (polygon[i].z < projectionPointA.z)
				{
					projectionPointA.z = polygon[i].z;
				}
				else if (polygon[i].z > projectionPointB.z)
				{
					projectionPointB.z = polygon[i].z;
				}
			}

			std::pair<glm::vec3, glm::vec3> lineSegment{ projectionPointA, projectionPointB };
			return lineSegment;
		}

		bool DoesTravelPathCollide(const Rectangle& rectangle, const glm::vec3& targetCenter, const Rectangle& obstacle)
		{
			const auto travelPathPolygon = GetTravelPathBounding(rectangle, targetCenter);
			const std::vector<glm::vec3> obstacleVertices{ obstacle.A(), obstacle.B(), obstacle.C(), obstacle.D() };
			bool doTheyOverlap = PolygonCollision::DoPolygonsOverlap(travelPathPolygon, obstacleVertices);
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
			const auto& currentCenter = r1.Center();
			const glm::vec3 directionVector = targetCenter - currentCenter;
			std::pair<glm::vec3, glm::vec3> forwardPointsOfR1{ GetNearestPoints(r1, targetCenter) };
			forwardPointsOfR1.first += directionVector;
			forwardPointsOfR1.second += directionVector;
			std::pair<glm::vec3, glm::vec3> verticesOfR2NearestToR1{ GetNearestPoints(r2, currentCenter) };

			const glm::vec3 edgeIntersectionPoint = GetEdgeIntersectionPoint(forwardPointsOfR1.first, forwardPointsOfR1.second,
				verticesOfR2NearestToR1.first, verticesOfR2NearestToR1.second, directionVector);

			std::vector<glm::vec3> possibleCollisionPoints{ verticesOfR2NearestToR1.first, verticesOfR2NearestToR1.second, edgeIntersectionPoint };
			SortPointsByDistanceFromPoint(possibleCollisionPoints, currentCenter);

			const glm::vec3 closestPointToLineOfMotion = GetClosestPointToLine(
					{ possibleCollisionPoints[0], possibleCollisionPoints[1] },
					currentCenter, targetCenter);

			const glm::vec3 forwardPointsVector = forwardPointsOfR1.second - forwardPointsOfR1.first;
			const glm::vec3 p = MathUtil::GetIntersectionPointBetweenLines(currentCenter, targetCenter, 
				closestPointToLineOfMotion, closestPointToLineOfMotion + forwardPointsVector);
			const glm::vec3 m = MathUtil::GetIntersectionPointBetweenLines(currentCenter, targetCenter,
				forwardPointsOfR1.first, forwardPointsOfR1.second);
			const float delta = glm::length(m - p);
			const float directionVectorLength = glm::length(directionVector);
			const float factor = (directionVectorLength - delta) / directionVectorLength;
			const glm::vec3 returnCenter = currentCenter + factor * directionVector;

			return returnCenter;
		}

		std::pair<glm::vec3, glm::vec3> GetNearestPoints(const Rectangle& r1, const glm::vec3& targetPoint)
		{
			std::vector<glm::vec3> vertices{ r1.A(), r1.B(), r1.C(), r1.D() };
			SortPointsByDistanceFromPoint(vertices, targetPoint);
			return std::pair<glm::vec3, glm::vec3>(vertices[0], vertices[1]);
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

		void SortPointsByDistanceFromPoint(std::vector<glm::vec3>& vertices, const glm::vec3& targetPoint)
		{
			std::sort(vertices.begin(), vertices.end(),
				[&targetPoint](const glm::vec3& a, const glm::vec3& b)
			{
				return glm::length(a - targetPoint) < glm::length(b - targetPoint);
			});
		}

		glm::vec3 GetEdgeIntersectionPoint(const glm::vec3& a, const glm::vec3& b,
			const glm::vec3& c, const glm::vec3& d, const glm::vec3& directionVector)
		{
			const glm::vec3 av = a + directionVector;
			const glm::vec3 bv = b + directionVector;
			const glm::vec3 ax{ MathUtil::GetIntersectionPointBetweenLines(a, av, c, d) };
			const glm::vec3 bx{ MathUtil::GetIntersectionPointBetweenLines(b, bv, c, d) };

			const glm::vec3 x = glm::length(ax - a) > glm::length(bx - b) ? ax : bx;
			return x;
		}

		glm::vec3 GetClosestPointToLine(std::vector<glm::vec3> points, const glm::vec3& a, const glm::vec3& b)
		{
			std::sort(points.begin(), points.end(),
				[&a, &b](const glm::vec3& p1, const glm::vec3& p2)
			{
				return MathUtil::GetDistanceFromPointToLine(p1, a, b) < MathUtil::GetDistanceFromPointToLine(p2, a, b);
			}
			);
			return points[0];
		}

		glm::vec3 GetClosestPointToPoint(std::vector<glm::vec3> points, const glm::vec3& a)
		{
			std::sort(points.begin(), points.end(),
				[&a](const glm::vec3& p1, const glm::vec3& p2)
			{
				return glm::length(p1 - a)  < glm::length(p2 - a);
			}
			);
			return points[0];
		}
	}
}