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
			bool DoesAnyVerticePathCollide(const std::vector<glm::vec3>& targetVertices, const glm::vec3& directionVector,
				const std::vector<glm::vec3>& obstacleVertices);
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
		}
	}
}

#endif //_MY_CODE_POLYGON_INTERSECTION_H_