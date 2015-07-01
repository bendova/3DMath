#ifndef _MY_CODE_INTERSECTION_2D_COLLISION_AVOIDANCE_H_
#define _MY_CODE_INTERSECTION_2D_COLLISION_AVOIDANCE_H_

#include <vector>
#include <glm/glm.hpp>

namespace MyCode
{
	namespace Intersection2D
	{
		struct Collision
		{
			glm::vec3 mCollidingVertex;
			glm::vec3 mPointOfCollision;
		};

		namespace CollisionAvoidance
		{
			class CollisionAvoider
			{
			public:
				enum class Avoidance
				{
					OUTSIDE_IN = 0,
					INSIDE_OUT
				};

				CollisionAvoider(const std::vector<glm::vec3>& verticesR1, const std::vector<glm::vec3>& verticesR2,
					const glm::vec3& currentCenter, const glm::vec3& targetCenter,
					const Avoidance avoidance = Avoidance::OUTSIDE_IN);

				const glm::vec3& GetValidCenter() const { return mValidCenter; }
			private:
				void DetermineValidCenter();
				std::pair<Collision, bool> GetNearEdgeCollision();
				std::vector<Collision> GetCollisionsFromBothPolygons();

				std::vector<Collision> GetCollisions(const std::vector<glm::vec3>& forwardsR1, const std::vector<glm::vec3>& forwardsR2,
					const glm::vec3& directionVector) const;

				std::pair<glm::vec3, bool> GetClosestIntersectionPoint(const glm::vec3& a, const glm::vec3& b,
					const std::vector<glm::vec3>& lineSegments) const;
				std::pair<glm::vec3, bool> GetFarthestIntersectionPoint(const glm::vec3& a, const glm::vec3& b,
					const std::vector<glm::vec3>& lineSegments) const;

				void SortAscending(std::vector<Collision>& collisions, const glm::vec3& biasPoint) const;
				glm::vec3 GetCenterThatAvoidsCollision(const glm::vec3& currentCenter, const Collision& collision) const;

				const std::vector<glm::vec3>& mVerticesR1;
				const std::vector<glm::vec3>& mVerticesR2;
				const glm::vec3& mCurrentCenter;
				const glm::vec3& mTargetCenter;
				const glm::vec3 mDirectionVector;
				const Avoidance mAvoidance;
				glm::vec3 mValidCenter;
			};

			glm::vec3 GetPositionOnNearEdge(const std::vector<glm::vec3>& verticesOfR1, const glm::vec3& currentCenter,
				const glm::vec3& targetCenter, const std::vector<glm::vec3>& verticesOfR2,
				const CollisionAvoider::Avoidance avoidance = CollisionAvoider::Avoidance::OUTSIDE_IN);
		}
	}
}

#endif //_MY_CODE_INTERSECTION_2D_COLLISION_AVOIDANCE_H_