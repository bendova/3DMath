#include "UT_IntersectionHelper.h"
#include "../framework/UT_Util.h"
#include "../framework/MathUtil.h"
#include <cmath>

namespace MyCode
{
	UT_IntersectionHelper::UT_IntersectionHelper()
	{}

	UT_IntersectionHelper::~UT_IntersectionHelper()
	{}

	bool UT_IntersectionHelper::Validate()
	{
		return NoIntersection()
			&& NearEdgeIntersection()
			&& FarEdgeIntersection();
	}

	bool UT_IntersectionHelper::NoIntersection()
	{
		Setup setup{ glm::vec3{ 0.0f }, glm::vec3{3.0f, 0.0f, 0.0f} };

		const glm::vec3 target{ 1.0f, 0.0f, 0.0f };
		const glm::vec3 validTarget = setup.mIntersecter.GetValidPosition(setup.mControlA, target);
		return CHECK_EQUALS(target, validTarget);
	}

	bool UT_IntersectionHelper::NearEdgeIntersection()
	{
		Setup setup{ glm::vec3{ 0.0f }, glm::vec3{ 4.0f, 0.0f, 0.0f } };

		const glm::vec3 target{ 3.0f, 0.0f, 0.0f };
		const float expectedValidTargetX = setup.mControlB.GetPosition().x - setup.mControlB.GetRadius() - setup.mControlA.GetRadius();
		glm::vec3 expectedValidTarget { expectedValidTargetX, 0.0f, 0.0f };
		MathUtil::Floor(expectedValidTarget);
		glm::vec3 validTarget = setup.mIntersecter.GetValidPosition(setup.mControlA, target);
		MathUtil::Floor(validTarget);
		return CHECK_EQUALS(expectedValidTarget, validTarget);
	}

	bool UT_IntersectionHelper::FarEdgeIntersection()
	{
		Setup setup{ glm::vec3{ 0.0f }, glm::vec3{ 4.0f, 0.0f, 0.0f } };

		const glm::vec3 target{ 5.0f, 0.0f, 0.0f };
		const float expectedValidTargetX = setup.mControlB.GetPosition().x - setup.mControlB.GetRadius() - setup.mControlA.GetRadius();
		glm::vec3 expectedValidTarget{ expectedValidTargetX, 0.0f, 0.0f };
		MathUtil::Floor(expectedValidTarget);
		glm::vec3 validTarget = setup.mIntersecter.GetValidPosition(setup.mControlA, target);
		MathUtil::Floor(validTarget);
		return CHECK_EQUALS(expectedValidTarget, validTarget);
	}
}