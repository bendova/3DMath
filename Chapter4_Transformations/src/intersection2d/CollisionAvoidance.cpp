#include "CollisionAvoidance.h"
#include "../framework/VectorMath.h"
#include "../framework/Logging.h"

namespace MyCode
{
	namespace Intersection2D
	{
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
					if (insidePointsOnly && (VectorMath::IsPointInsidePolygon(verticesR2,
						VectorMath::MarginPoint<glm::vec3> { vertex }) == false))
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

				VectorMath::MarginPoint<glm::vec3> marginA{ a };
				VectorMath::MarginPoint<glm::vec3> marginB{ b, VectorMath::BoundingPointType::UNBOUNDED };

				const auto lineSegmentPoints = lineSegments.size();
				for (size_t i = 0; i < lineSegmentPoints; ++i)
				{
					const auto& c = lineSegments[i];
					const auto& d = lineSegments[(i + 1) % lineSegmentPoints];

					VectorMath::MarginPoint<glm::vec3> marginC{ c };
					VectorMath::MarginPoint<glm::vec3> marginD{ d };
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