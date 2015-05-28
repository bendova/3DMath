#include "CubeCollider.h"
#include "../intersection2d/RectangleColider.h"
#include <algorithm>
#include "PolygonCollider.h"

namespace MyCode
{
	glm::vec3 CubeCollider::GetPositionThatAvoidCollisions(const Cube& cube, glm::vec3 targetCenter) const
	{
		using namespace CubeColliderHelpers;
		using namespace TravelPathIntersection;
		std::vector<const Cube*> sortedCubes = SortByDistanceFromPoint(mCubes, cube.GetCenter());
		for (const Cube* const c : sortedCubes)
		{
			const Cube& obstacle = *c;
			if (&obstacle != &cube)
			{
				const bool doCubesCollide = DoesTravelPathCollide(cube, targetCenter, obstacle);
				if (doCubesCollide)
				{
					targetCenter = GetValidPosition(cube, targetCenter, obstacle);
				}
			}
		}

		return targetCenter;
	}

	namespace CubeColliderHelpers
	{
		std::vector<const Cube*> SortByDistanceFromPoint(std::vector<const Cube*> rectangles, const glm::vec3& point)
		{
			std::sort(rectangles.begin(), rectangles.end(),
				[&point](const Cube* a, const Cube* b)
			{
				const auto distanceFromA_toPoint = glm::length(point - a->GetCenter());
				const auto distanceFromB_toPoint = glm::length(point - b->GetCenter());
				return distanceFromA_toPoint < distanceFromB_toPoint;
			});

			return rectangles;
		}

		glm::vec3 GetValidPosition(const Cube& cube, const glm::vec3& targetCenter, const Cube& obstacle)
		{
			using namespace TravelPathIntersection;

			PolygonCollider polygonCollider;
			polygonCollider.AddPolygons(obstacle.GetFaces());

			glm::vec3 directionVector = targetCenter - cube.GetCenter();
			for (int i = 0; i < cube.FacesCount(); ++i)
			{
				const Polygon& face = cube[i];
				const glm::vec3 faceTargetCenter = face.Center() + directionVector;
				const glm::vec3 validTargetCenter = polygonCollider.GetPositionThatAvoidCollisions(face, faceTargetCenter);
				if (validTargetCenter != faceTargetCenter)
				{
					directionVector = validTargetCenter - face.Center();
				}
			}
			return cube.GetCenter() + directionVector;
		}
	}

	namespace TravelPathIntersection
	{
		bool DoesTravelPathCollide(const Cube& cube, const glm::vec3& targetCenter, const Cube& obstacle)
		{
			bool doesItCollide = false;
			const glm::vec3 directionVector = targetCenter - cube.GetCenter();
			
			// FIXME Doing this is horribly expensive!
			// We should apply the same logic used for 2D polygon intersection - 
			// project the 3D objects and then check their shadows for intersection
			for (int i = 0; i < cube.FacesCount(); ++i)
			{
				const Polygon& face = cube[i];
				const glm::vec3 faceTargetCenter = face.Center() + directionVector;
				doesItCollide = DoesTravelPathCollideCube(face, faceTargetCenter, obstacle);
				if (doesItCollide)
				{
					break;
				}
			}
			return doesItCollide;
		}

		bool DoesTravelPathCollideCube(const Polygon& target, const glm::vec3& targetCenter, const Cube& obstacle)
		{
			using namespace Intersection3D;
			bool doesItCollide = false;
			for (int i = 0; i < obstacle.FacesCount(); ++i)
			{
				const Polygon& obstacleFace = obstacle[i];
				doesItCollide = CollisionDetection::DoesPathCollide(target, targetCenter, obstacleFace);
				if (doesItCollide)
				{
					break;
				}
			}
			return doesItCollide;
		}
	}
}