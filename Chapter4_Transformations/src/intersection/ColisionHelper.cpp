#include "ColisionHelper.h"
#include "../control/ControlHelper.h"
#include "../framework/MathUtil.h"
#include <cmath>
#include <cassert>

namespace MyCode
{
	ColisionHelper::ColisionHelper()
		: mControlHelpers{}
	{}

	ColisionHelper::~ColisionHelper()
	{}

	glm::vec3 ColisionHelper::GetValidPosition(const ControlHelper& current, glm::vec3 desired) const
	{
		if (current.GetPosition() != desired)
		{
			for (const auto* other: mControlHelpers)
			{
				const float minDistance = current.GetRadius() + other->GetRadius();
				if ((other != &current) && (glm::length(desired - other->GetPosition()) < minDistance))
				{
					desired = GetPointOnEdge(current, desired, *other);
				}
			}
		}
		return desired;
	}

	glm::vec3 ColisionHelper::GetPointOnEdge(const ControlHelper& current, const glm::vec3& desired, const ControlHelper& other) const
	{
		assert(current.GetPosition() != desired);

		const auto& currentCenter = current.GetPosition();
		const auto& otherCenter = other.GetPosition();
		const glm::vec3 projection{ MathUtil::GetProjectionPoint
			(glm::vec4{ currentCenter, 1.0f }, glm::vec4{ desired, 1.0f }, glm::vec4{ otherCenter, 1.0f }) };
		
		const float minDistanceBetweenCenters = current.GetRadius() + other.GetRadius();
		const float distanceFromOtherCenterToProjection = glm::length(otherCenter - projection);
		const float distanceFromProjectionToValidPoint = std::sqrt(minDistanceBetweenCenters * minDistanceBetweenCenters - 
			distanceFromOtherCenterToProjection * distanceFromOtherCenterToProjection);
		
		const auto vectorBetweenCenterAndProjection = projection - currentCenter;
		const float distanceBetweenCenterAndProjection = glm::length(vectorBetweenCenterAndProjection);
		const float distanceFromCenterToValidPoint = distanceBetweenCenterAndProjection - distanceFromProjectionToValidPoint;
		const float factor = distanceFromCenterToValidPoint / distanceBetweenCenterAndProjection;
		const auto point = currentCenter + factor * vectorBetweenCenterAndProjection;

		return point;
	}
}