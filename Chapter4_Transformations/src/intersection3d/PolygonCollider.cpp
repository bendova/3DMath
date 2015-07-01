#include "PolygonCollider.h"
#include "../framework/VectorMath.h"
#include "../framework/Logging.h"
#include "../intersection2d/PolygonIntersection.h"
#include <algorithm>

namespace MyCode
{
	using namespace VectorMath;

	void PolygonCollider::AddPolygon(const Polygon& polygon)
	{
		mPolygons.push_back(&polygon);
	}

	void PolygonCollider::AddPolygons(const std::vector<Polygon>& polygons)
	{
		for (const auto& polygon : polygons)
		{
			mPolygons.push_back(&polygon);
		}
	}

	glm::vec3 PolygonCollider::GetPositionThatAvoidCollisions(const Polygon& target, glm::vec3 targetCenter) const
	{
		using namespace Intersection3D;
		using namespace PolygonColliderHelpers;

		bool wasCollisionFound = false;
		std::vector<const Polygon*> sortedPolygons = SortByDistanceFromPoint(mPolygons, target.Center());
		for (const Polygon* const r : sortedPolygons)
		{
			const Polygon& obstacle = *r;
			if (&target != &obstacle)
			{
				if (CollisionDetection::DoesPathCollide(target, targetCenter, obstacle))
				{
					wasCollisionFound = true;
					targetCenter = GetValidCenter(target, obstacle, targetCenter);
					CollisionSanityCheck(target, targetCenter, obstacle);
				}
			}
		}

		if (wasCollisionFound == false)
		{
			Log("Starting from center = [%f, %f, %f]\n", target.Center().x, target.Center().y, target.Center().z);
			Log("No collision was found for targetCenter = [%f, %f, %f]\n", targetCenter.x, targetCenter.y, targetCenter.z);
		}

		return targetCenter;
	}

	namespace PolygonColliderHelpers
	{
		std::vector<const Polygon*> SortByDistanceFromPoint(std::vector<const Polygon*> polygons, const glm::vec3& point)
		{
			std::sort(polygons.begin(), polygons.end(),
				[&point](const Polygon* a, const Polygon* b)
			{
				const auto distanceFromA_toPoint = glm::length(point - a->Center());
				const auto distanceFromB_toPoint = glm::length(point - b->Center());
				return distanceFromA_toPoint < distanceFromB_toPoint;
			});

			return polygons;
		}

		glm::vec3 GetValidCenter(const Polygon& target, const Polygon& obstacle, const glm::vec3& targetCenter)
		{
			Log("GetValidCenter() target.Center() = [%f, %f, %f]\n", target.Center().x, target.Center().y, target.Center().z);
			Log("GetValidCenter() targetCenter = [%f, %f, %f]\n", targetCenter.x, targetCenter.y, targetCenter.z);
			Log("GetValidCenter() obstacle.Center() = [%f, %f, %f]\n", obstacle.Center().x, obstacle.Center().y, obstacle.Center().z);

			using namespace Intersection3D;
			using namespace CollisionAvoidance;

			const auto& targetVertices = target.Vertices();
			const auto& obstacleVertices = obstacle.Vertices();
			const glm::vec3& currentCenter{ target.Center() };

			glm::vec3 validCenter{ targetCenter };
			if (PolygonIntersection::DoPolygonsIntersect3D(targetVertices, obstacleVertices))
			{
				if (currentCenter != obstacle.Center())
				{
					const glm::vec3 centersVector{ obstacle.Center() - currentCenter };
					const glm::vec3 directionVector{ targetCenter - currentCenter };
					const auto dotValue = glm::dot(directionVector, centersVector);
					const bool isTargetDirectionCorrect = (dotValue <= 0.0f);
					if (isTargetDirectionCorrect == false)
					{
						const glm::vec3 inverseTarget = currentCenter - directionVector;
						validCenter = GetPositionOnNearEdge(target, inverseTarget,
							obstacle, Avoidance::INSIDE_OUT);
					}
				}
			}
			else
			{
				validCenter = GetPositionOnNearEdge(target, targetCenter, 
					obstacle, Avoidance::OUTSIDE_IN);
			}

			Log("GetValidCenter() validCenter = [%f, %f, %f]\n\n", validCenter.x, validCenter.y, validCenter.z);

			return validCenter;
		}

		float GetInscribedCircleRadius(const Polygon& polygon)
		{
			const auto& vertices = polygon.Vertices();
			const float halfSideLength = glm::length(vertices[1] - vertices[0]) / 2.0f;
			const float halfDiagonalLength = glm::length(vertices[0] - polygon.Center()) / 2.0f;
			const float radius = std::sqrt(halfDiagonalLength * halfDiagonalLength - 
				halfSideLength * halfSideLength);
			return radius;
		}

		void CollisionSanityCheck(const Polygon& target, const glm::vec3& newTargetCenter, const Polygon& obstacle)
		{
			const float targetInscribedCircleRadius = GetInscribedCircleRadius(target);
			const float obstacleInscribedCircleRadius = GetInscribedCircleRadius(obstacle);

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

	namespace Intersection3D
	{
		namespace PolygonIntersection
		{
			bool DoPolygonsIntersect3D(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b)
			{
				bool doTheyOverlap = DoPolygonProjectionsIntersect(a, b);
				if (doTheyOverlap == false)
				{
					doTheyOverlap = DoPolygonProjectionsIntersect(b, a);
				}

				return doTheyOverlap;
			}

			bool DoPolygonProjectionsIntersect(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b)
			{
				using namespace Intersection2D::PolygonIntersection;

				bool doTheyIntersect = true;
				const std::vector<Plane> planes = GetCoordinatePlanesRelativeToPlane(a);
				for (const auto& plane : planes)
				{
					const std::vector<glm::vec3> projectionA = GetPolygonProjectionToPlane(a, plane);
					const std::vector<glm::vec3> projectionB = GetPolygonProjectionToPlane(b, plane);
					doTheyIntersect = DoPolygonsIntersect2D(projectionA, projectionB);
					if (doTheyIntersect == false)
					{
						break;
					}
				}
				return doTheyIntersect;
			}

			std::vector<VectorMath::Plane> GetCoordinatePlanesRelativeToPlane(const std::vector<glm::vec3>& a)
			{
				const glm::vec3& pointA = a[0];
				const glm::vec3& pointB = a[1];
				const glm::vec3& pointC = a[2];
				const glm::vec3 normal1 = pointB - pointA;
				const glm::vec3 normal2 = glm::cross(normal1, (pointC - pointA));
				const glm::vec3 normal3 = glm::cross(normal1, normal2);

				std::vector<VectorMath::Plane> axes;
				axes.emplace_back(pointA, normal1);
				axes.emplace_back(pointA, normal2);
				axes.emplace_back(pointA, normal3);
				return axes;
			}

			std::vector<glm::vec3> GetPolygonProjectionToPlane(const std::vector<glm::vec3>& polygon, const VectorMath::Plane& plane)
			{
				std::vector<glm::vec3> projection;
				for (const auto& vertice : polygon)
				{
					projection.push_back(GetProjectionPointOnPlane(vertice, plane.mPointInPlane, plane.mNormalToPlane));
				}
				return projection;
			}
		}

		namespace CollisionDetection
		{
			bool DoesPathCollide(const Polygon& target, const glm::vec3& destination, const Polygon& obstacle)
			{
				bool doesItCollide = false;

				using namespace Intersection2D::PolygonIntersection;
				if (VectorMath::IsPointCoplanarWithPolygon(destination, target.Vertices()))
				{
					doesItCollide = DoesItCollide2D(target, destination, obstacle);
				}
				else
				{
					doesItCollide = DoesItCollide3D(target, destination, obstacle);
				}

				return doesItCollide;
			}

			bool DoesItCollide2D(const Polygon& target, const glm::vec3& destination, const Polygon& obstacle)
			{
				using namespace PolygonIntersection;
				using namespace TravelPathBounding::Detail;

				const Polygon boundingPath = GetBoundingPath2D(target, destination);
				const bool doesItCollide = DoPolygonsIntersect3D(boundingPath.Vertices(), obstacle.Vertices());
				return doesItCollide;
			}

			bool DoesItCollide3D(const Polygon& target, const glm::vec3& destination, const Polygon& obstacle)
			{
				using namespace PolygonIntersection;

				const glm::vec3 directionVector = destination - target.Center();
				std::vector<glm::vec3> boundingPath;
				for (const auto& vertex : target.Vertices())
				{
					boundingPath.push_back(vertex + directionVector);
				}
				const Plane boundingPathPlane = VectorMath::GetPolygonPlane(boundingPath);
				const bool doesItCollide = DoPolygonsIntersect3D(boundingPath, obstacle.Vertices());
				return doesItCollide;
			}
		}

		namespace TravelPathBounding
		{
			std::vector<Polygon> GetBoundingPath(const Polygon& target, const glm::vec3& destination)
			{
				/*
					We have 2 cases here:
					1. If the targetCenter is coplanar with target
					we then apply the 2D bounding path algorithm.
					The bounding path is 1 Polygon.
					2. Else all the vertices and their displacements determine the
					bounding path.
					The bounding path is 1 Polyhedron (n Polygons).
				*/
				std::vector<Polygon> boundingPath;
				if (VectorMath::IsPointCoplanarWithPolygon(destination, target.Vertices()))
				{
					boundingPath.push_back(Detail::GetBoundingPath2D(target, destination));
				}
				else
				{
					boundingPath = Detail::GetBoundingPath3D(target, destination);
				}
				return boundingPath;
			}

			namespace Detail
			{
				Polygon GetBoundingPath2D(const Polygon& target, const glm::vec3& destination)
				{
					using namespace VectorMath;

					std::vector<glm::vec3> boundingVertices;

					const auto& targetVertices = target.Vertices();
					const glm::vec3 directionVector = destination - target.Center();
					float previousDotValue = GetDotWithLastSideOfPolygon(targetVertices, directionVector);
					
					const size_t verticesCount = targetVertices.size();
					for (size_t i = 0; i < verticesCount; ++i)
					{
						const auto& a = targetVertices[i];
						const auto& b = targetVertices[(i + 1) % verticesCount];
						float dotValue = glm::dot(b - a, directionVector);

						// We must be careful now to add the vertices
						// to the bounding in CCW order.
						const glm::vec3 aForward = a + directionVector;
						const glm::vec3 aBackward = a - directionVector;
						const bool isCoveredFromFront = DoesRayIntersectPolygon(a, aForward, targetVertices);
						const bool isCoveredFromBehind = DoesRayIntersectPolygon(a, aBackward, targetVertices);
						if ((isCoveredFromFront == false) && 
							(isCoveredFromBehind == false))
						{
							if (dotValue == 0)
							{
								dotValue = previousDotValue;
							}

							const bool isAngleAcute = (dotValue > 0);
							if (isAngleAcute)
							{
								boundingVertices.push_back(a);
								boundingVertices.push_back(aForward);
							}
							else
							{
								boundingVertices.push_back(aForward);
								boundingVertices.push_back(a);
							}
						}
						else if (isCoveredFromFront == false)
						{
							boundingVertices.push_back(aForward);
						}
						else if (isCoveredFromBehind == false)
						{
							boundingVertices.push_back(a);
						}

						previousDotValue = dotValue;
					}
					return Polygon{ std::forward<std::vector<glm::vec3>>(boundingVertices) };
				}

				float GetDotWithLastSideOfPolygon(const std::vector<glm::vec3>& polygon, const glm::vec3& direction)
				{
					const auto verticesCount = polygon.size();
					const glm::vec3& z = polygon[verticesCount - 1];
					const glm::vec3& a = polygon[0];
					const glm::vec3 lastSide = a - z;
					return glm::dot(lastSide, direction);
				}

				std::vector<Polygon> GetBoundingPath3D(const Polygon& target, const glm::vec3& destination)
				{
					const auto directionVector = destination - target.Center();

					std::vector<Polygon> boundingFaces;

					const auto& targetVertices = target.Vertices();
					boundingFaces.emplace_back(std::vector<glm::vec3>(targetVertices.rbegin(), targetVertices.rend()));

					Plane polygonPlane = VectorMath::GetPolygonPlane(targetVertices);
					const size_t verticesCount = targetVertices.size();
					for (size_t i = 0; i < verticesCount; ++i)
					{
						const glm::vec3& a = targetVertices[i];
						const glm::vec3& b = targetVertices[(i + 1) % verticesCount];
						boundingFaces.push_back(GetBoundingPolygonForLineSegment(a, b, 
							polygonPlane.mNormalToPlane, directionVector));
					}

					std::vector<glm::vec3> topFaceVertices;
					for (const auto& vertex : targetVertices)
					{
						topFaceVertices.push_back(vertex + directionVector);
					}
					boundingFaces.emplace_back(topFaceVertices);
					return boundingFaces;
				}

				Polygon GetBoundingPolygonForLineSegment(const glm::vec3& a, const glm::vec3& b, 
					const glm::vec3& upDirection, const glm::vec3& directionVector)
				{
					const glm::vec3 c = a + directionVector;
					const glm::vec3 d = b + directionVector;
					
					std::vector<glm::vec3> bounding;

					const float angleWithUpDirection = glm::dot(directionVector, upDirection);
					const bool isDirectionVectorToLeft = (angleWithUpDirection >= 0);
					if (isDirectionVectorToLeft)
					{
						bounding.push_back(a);
						bounding.push_back(b);
						bounding.push_back(d);
						bounding.push_back(c);
					}
					else
					{
						bounding.push_back(a);
						bounding.push_back(c);
						bounding.push_back(d);
						bounding.push_back(b);
					}

					return Polygon{ std::forward<std::vector<glm::vec3>>(bounding) };
				}

			}
		}

		namespace CollisionAvoidance
		{
			glm::vec3 GetPositionOnNearEdge(const Polygon& target, const glm::vec3& targetCenter,
				const Polygon& obstacle,
				const Avoidance avoidance)
			{
				CollisionAvoider avoider{ target.Vertices(), obstacle.Vertices(), target.Center(), targetCenter, avoidance };
				const glm::vec3 validCenter = avoider.GetValidCenter();
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
				using namespace VectorMath;

				const bool insidePointsOnly = (mAvoidance == Avoidance::INSIDE_OUT);

				std::vector<Collision> collisions;
				for (const auto& vertex : verticesR1)
				{
					MarginPoint<glm::vec3> a{ vertex };
					if (insidePointsOnly && (IsPointInsidePolygon(verticesR2, a) == false))
					{
						continue;
					}

					MarginPoint<glm::vec3> b{ vertex + directionVector, BoundingPointType::UNBOUNDED };

					const auto intersection = GetIntersectionBetweenLineAndPolygon(a, b, verticesR2);
					if (intersection.second)
					{
						collisions.emplace_back(vertex, intersection.first);
					}
				}
				return collisions;
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