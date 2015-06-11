#ifndef _MY_CODE_POLYGON_INTERSECTION_H_
#define _MY_CODE_POLYGON_INTERSECTION_H_

#include <glm/glm.hpp>
#include <vector>
#include "Polygon.h"

namespace MyCode
{
	class PolygonCollider
	{
	public:
		void AddPolygon(const Polygon& polygon);
		void AddPolygons(const std::vector<Polygon>& polygons);
		glm::vec3 GetPositionThatAvoidCollisions(const Polygon& polygon, glm::vec3 targetCenter) const;

	private:
		std::vector<const Polygon*> mPolygons;
	};

	namespace PolygonColliderHelpers
	{
		std::vector<const Polygon*> SortByDistanceFromPoint(std::vector<const Polygon*> rectangles, const glm::vec3& point);
		glm::vec3 GetValidCenter(const Polygon& rectangle, const Polygon& obstacle, const glm::vec3& targetCenter);

		void CollisionSanityCheck(const Polygon& target, const glm::vec3& newTargetCenter, const Polygon& obstacle);
	}

	namespace Intersection3D
	{
		namespace PolygonIntersection
		{
			struct Plane
			{
				Plane(const glm::vec3& point, const glm::vec3& normal)
					: mPoint(point)
					, mNormal(normal)
				{}

				glm::vec3 mPoint;
				glm::vec3 mNormal;
			};

			bool DoPolygonsIntersect3D(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b);
			bool DoPolygonProjectionsIntersect(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b);
			std::vector<Plane> GetCoordinatePlanesRelativeToPlane(const std::vector<glm::vec3>& a);
			std::vector<glm::vec3> GetPolygonProjectionToPlane(const std::vector<glm::vec3>& polygon, const Plane& plane);
		}

		namespace CollisionDetection
		{
			bool DoesPathCollide(const Polygon& target, const glm::vec3& targetCenter, const Polygon& obstacle);
			bool DoesAnyVerticePathCollide(const Polygon& target, const glm::vec3& directionVector,
				const Polygon& obstacle);
		}

		namespace TravelPathBounding
		{
			std::vector<Polygon> GetBoundingPath(const Polygon& target, const glm::vec3& destination);

			namespace Detail
			{
				Polygon GetBoundingPath2D(const Polygon& target, const glm::vec3& destination);
				std::vector<Polygon> GetBoundingPath3D(const Polygon& target, const glm::vec3& destination);
			}
		}

		namespace CollisionAvoidance
		{
			enum class Avoidance
			{
				OUTSIDE_IN = 0,
				INSIDE_OUT
			};

			glm::vec3 GetPositionOnNearEdge(const Polygon& target, const glm::vec3& targetCenter,
				const Polygon& obstacle,
				const Avoidance avoidance = Avoidance::OUTSIDE_IN);

			struct Collision
			{
				Collision(const glm::vec3& collidingVertex, const glm::vec3& pointOfCollision)
					: mCollidingVertex(collidingVertex)
					, mPointOfCollision(pointOfCollision)
				{}

				glm::vec3 mCollidingVertex;
				glm::vec3 mPointOfCollision;
			};

			class CollisionAvoider
			{
			public:
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
		}
	}
}

#endif //_MY_CODE_POLYGON_INTERSECTION_H_