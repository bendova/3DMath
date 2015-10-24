#include "CubeCollider.h"
#include "../intersection2d/RectangleColider.h"
#include "../intersection2d/PolygonIntersection.h"
#include "../intersection3d/PolygonCollider.h"
#include "vectormath/DataTypes.h"
#include "vectormath/Projection.h"
#include "PolygonCollider.h"

#include <algorithm>

namespace MyCode
{
	glm::vec3 CubeCollider::GetPositionThatAvoidsCollisions(const Cube& cube, glm::vec3 targetCenter) const
	{
		using namespace CubeColliderHelpers;
		using namespace TravelPathIntersection;

		glm::vec3 directionVector = targetCenter - cube.Center();

		std::vector<const Cube*> sortedCubes = SortByDistanceFromPoint(mCubes, cube.Center());
		for (const Cube* c : sortedCubes)
		{
			const Cube& obstacle = *c;
			if (&obstacle != &cube)
			{
				const bool doCubesCollide = DoesTravelPathCollide(cube, directionVector, obstacle);
				if (doCubesCollide)
				{
					directionVector = GetValidPosition(cube, directionVector, obstacle);
				}
			}
		}

		targetCenter = cube.Center() + directionVector;
		return targetCenter;
	}

	namespace CubeColliderHelpers
	{
		std::vector<const Cube*> SortByDistanceFromPoint(std::vector<const Cube*> rectangles, const glm::vec3& point)
		{
			std::sort(rectangles.begin(), rectangles.end(),
				[&point](const Cube* a, const Cube* b)
			{
				const auto distanceFromA_toPoint = glm::length(point - a->Center());
				const auto distanceFromB_toPoint = glm::length(point - b->Center());
				return distanceFromA_toPoint < distanceFromB_toPoint;
			});

			return rectangles;
		}

		glm::vec3 GetValidPosition(const Cube& cube, glm::vec3 directionVector, const Cube& obstacle)
		{
			using namespace TravelPathIntersection;

			static const glm::vec3 ZERO_VECTOR{ 0.0f, 0.0f, 0.0f };

			PolygonCollider polygonCollider;
			polygonCollider.AddPolygons(obstacle.GetFaces());

			for (int i = 0; i < cube.FacesCount(); ++i)
			{
				const Polygon& face = cube[i];
				const glm::vec3 faceTargetCenter = face.Center() + directionVector;
				const glm::vec3 validTargetCenter = polygonCollider.GetPositionThatAvoidCollisions(face, faceTargetCenter);
				if (validTargetCenter != faceTargetCenter)
				{
					directionVector = validTargetCenter - face.Center();
					if (directionVector == ZERO_VECTOR)
					{
						break;
					}
				}
			}
			return directionVector;
		}
	}

	namespace TravelPathIntersection
	{
		bool DoesTravelPathCollide(const Cube& cube, const glm::vec3& directionVector, const Cube& obstacle)
		{
			// FIXME The implementation of this method would fail
			// the cubes were rotated. 

			using namespace VectorMath;
			
			typedef std::pair<Plane, int> PlaneFaceIndex;

			static const glm::vec3 origin{ 0.0f, 0.0f, 0.0f };
			static const Plane xyPlane{ origin, glm::vec3{ 0.0f, 0.0f, 1.0f } };
			static const Plane xzPlane{ origin, glm::vec3{ 0.0f, 1.0f, 0.0f } };
			static const Plane yzPlane{ origin, glm::vec3{ 1.0f, 0.0f, 0.0f } };
			static const std::vector<PlaneFaceIndex> planeFaces
			{ 
				{ xyPlane, Cube::FRONT_FACE_INDEX },
				{ xzPlane, Cube::BOTTOM_FACE_INDEX },
				{ yzPlane, Cube::LEFT_FACE_INDEX }
			};

			bool doesItCollide = true;
			for (const PlaneFaceIndex& planeFace : planeFaces)
			{
				const auto& plane = planeFace.first;
				const auto& cubeFace = cube[planeFace.second];
				const auto& obstacleFace = obstacle[planeFace.second];
				std::vector<glm::vec3> sourceProjection = Intersection3D::PolygonIntersection::GetPolygonProjectionToPlane(cubeFace.Vertices(), plane);
				if (Intersection2D::PolygonIntersection::Detail::IsLineSegment(sourceProjection))
				{
					const auto lineSegment = Intersection2D::PolygonIntersection::Detail::GetLineSegmentFromCollinearPoints(sourceProjection);
					sourceProjection.clear();
					sourceProjection.push_back(lineSegment.first);
					sourceProjection.push_back(lineSegment.second);
				}
				const glm::vec3 directionVectorProjection = ProjectPointOnPlane(directionVector, plane);
				const std::vector<glm::vec3> boundingPath = Intersection3D::TravelPathBounding::GetBoundingPath(sourceProjection, directionVectorProjection);
				const auto& obstacleProjection = Intersection3D::PolygonIntersection::GetPolygonProjectionToPlane(obstacleFace.Vertices(), plane);
				doesItCollide = Intersection2D::PolygonIntersection::DoPolygonsIntersect2D(boundingPath,
					obstacleProjection, VectorMath::PointType::OPEN_ENDED);
				if (doesItCollide == false)
				{
					break;
				}
			}
			return doesItCollide;
		}
	}
}