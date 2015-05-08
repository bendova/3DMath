#include "UT_RectangleColider.h"
#include "RectangleColider.h"
#include "../framework/UT_Util.h"

namespace MyCode
{
	UT_RectangleColider::UT_RectangleColider()
	{}

	UT_RectangleColider::~UT_RectangleColider()
	{}

	bool UT_RectangleColider::Validate()
	{
		return NoIntersection()
			&& NearEdgeIntersection()
			&& FarEdgeIntersection();
	}

	bool UT_RectangleColider::NoIntersection()
	{
		const glm::vec3 centerR1{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 centerR2{ 3.0f, 0.0f, 0.0f };
		const glm::vec3 vectorToA{ -0.5f, 0.0f, 0.5f };
		const glm::vec3 vectorToB{ 0.5f, 0.0f, 0.5f };
		const glm::vec3 vectorToC{ 0.5f, 0.0f, -0.5f };
		const glm::vec3 vectorToD{ -0.5f, 0.0f, -0.5f };
		Rectangle r1{ &centerR1, vectorToA, vectorToB, vectorToC, vectorToD };
		Rectangle r2{ &centerR2, vectorToA, vectorToB, vectorToC, vectorToD };
		RectangleColider colider;
		colider.AddRectangle(r1);
		colider.AddRectangle(r2);

		const glm::vec3 targetOfR1{ 1.0f, 0.0f, 0.0f };
		const glm::vec3 validTarget = colider.GetPositionThatAvoidCollisions(r1, targetOfR1);
		
		return CHECK_EQUALS(targetOfR1, validTarget);
	}

	bool UT_RectangleColider::NearEdgeIntersection()
	{
		return CHECK_IS_TRUE(true == false);
	}

	bool UT_RectangleColider::FarEdgeIntersection()
	{
		return CHECK_IS_TRUE(true == false);
	}
}