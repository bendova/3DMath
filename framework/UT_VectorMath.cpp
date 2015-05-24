#include "UT_VectorMath.h"
#include "VectorMath.h"
#include "UTUtil.h"

namespace MyCode
{
	using namespace VectorMath;

	bool UT_VectorMath::Validate()
	{
		PointToPlaneProjectionTest pointToPlaneProjection;
		return pointToPlaneProjection.Run();
	}

	bool UT_VectorMath::PointToPlaneProjectionTest::Run()
	{
		return PointToPlaneProjection()
			&& PointToPlaneProjection1();
	}

	bool UT_VectorMath::PointToPlaneProjectionTest::PointToPlaneProjection()
	{
		const glm::vec3 pointToProject{ 1.0f, 1.0f, 1.0f };
		const glm::vec3 pointInPlane{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 planeNormal{ 0.0f, 0.0f, 1.0f };

		const glm::vec3 projectedPoint = GetProjectionPointOnPlane(pointToProject, pointInPlane, planeNormal);
		const glm::vec3 expectedPoint{ 1.0f, 1.0f, 0.0f };

		return CHECK_EQUALS(projectedPoint, expectedPoint);
	}

	bool UT_VectorMath::PointToPlaneProjectionTest::PointToPlaneProjection1()
	{
		const glm::vec3 pointToProject{ -1.0f, -1.0f, 0.0f };
		const glm::vec3 pointInPlane{ -1.0f, 0.0f, 1.0f };
		const glm::vec3 planeNormal{ 0.0f, 4.0f, 0.0f };

		const glm::vec3 projectedPoint = GetProjectionPointOnPlane(pointToProject, pointInPlane, planeNormal);
		const glm::vec3 expectedPoint{ -1.0f, 0.0f, 0.0f };

		return CHECK_EQUALS(projectedPoint, expectedPoint);
	}
}