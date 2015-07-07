#include "Intersection.h"

namespace MyCode
{
	namespace VectorMath
	{
		glm::vec3 ProjectPointOnPlane(const glm::vec3& pointToProject, const VectorMath::Plane& plane)
		{
			const float distance = GetDistanceBetweenPointAndPlane(pointToProject, plane.mPointInPlane, plane.mNormalToPlane);
			const float factor = (distance / glm::length(plane.mNormalToPlane));
			const glm::vec3 projectedPoint = pointToProject + factor * plane.mNormalToPlane;
			return projectedPoint;
		}
	}
}