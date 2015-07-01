#include "RectangleColider.h"
#include "CollisionAvoidance.h"
#include "TravelPathBounding.h"
#include "PolygonIntersection.h"
#include "Logging.h"
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

	glm::vec3 RectangleColider::GetPositionThatAvoidsCollisions(const Rectangle& rectangle, glm::vec3 destination) const
	{
		using namespace Intersection2D;
		using namespace RectangleColiderHelpers;

		bool wasCollisionFound = false;
		std::vector<const Rectangle*> sortedRectangles = SortByDistanceFromPoint(mRectangles, rectangle.Center());
		for (const Rectangle* r : sortedRectangles)
		{
			const Rectangle& obstacle = *r;
			if (&rectangle != &obstacle)
			{
				const bool doesTravelPathCollide = TravelPathBounding::DoesTravelPathCollide(rectangle, destination, obstacle);
				if (doesTravelPathCollide)
				{
					wasCollisionFound = true;
					destination = GetValidCenter(rectangle, obstacle, destination);
					CollisionSanityCheck(rectangle, destination, obstacle);
				}
			}
		}

		if (wasCollisionFound == false)
		{
			/*Log("Starting from center = [%f, %f, %f]\n", rectangle.Center().x, rectangle.Center().y, rectangle.Center().z);
			Log("No collision was found for destination = [%f, %f, %f]\n", destination.x, destination.y, destination.z);*/
		}

		return destination;
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
			if (PolygonIntersection::DoPolygonsIntersect2D(verticesOfR1, verticesOfR2, VectorMath::PointType::OPEN_ENDED))
			{
				const glm::vec3 directionVector{ targetCenter - currentCenter };
				const glm::vec3 centersVector{ obstacle.Center() - currentCenter };
				const float dotValue = glm::dot(directionVector, centersVector);
				const bool isTargetDirectionCorrect = (dotValue <= 0.0f);
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
			const float minDistanceBetweenCenters = target.InscribedCircleRadius() + obstacle.InscribedCircleRadius();
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
}