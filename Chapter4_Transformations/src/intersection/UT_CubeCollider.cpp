#include "UT_CubeCollider.h"
#include "../framework/UT_Util.h"
#include "../framework/MathUtil.h"

namespace MyCode
{
	bool UT_CubeCollider::Validate()
	{
		return CubeRepresentation()
			&& AvoidCollision();
	}

	bool UT_CubeCollider::CubeRepresentation()
	{
		const glm::vec3 bottomA{ -1.0f, -1.0f, 1.0f };
		const glm::vec3 bottomB{ 1.0f, -1.0f, 1.0f };
		const glm::vec3 bottomC{ 1.0f, -1.0f, -1.0f };
		const glm::vec3 bottomD{ -1.0f, -1.0f, -1.0f };
		const glm::vec3 topA{ -1.0f, 1.0f, 1.0f };
		const glm::vec3 topB{ 1.0f, 1.0f, 1.0f };
		const glm::vec3 topC{ 1.0f, 1.0f, -1.0f };
		const glm::vec3 topD{ -1.0f, 1.0f, -1.0f };

		const std::vector<glm::vec3> cubeVertices{
			bottomA, bottomB, bottomC, bottomD,
			topA, topB, topC, topD
		};
		const glm::vec3 cubeCenter{ 0.0f, 0.0f, 0.0f };
		Cube c{ cubeCenter, cubeVertices };

		const Rectangle expectedFrontFace{bottomA, bottomB, topB, topA};
		const bool isFrontFaceValid = CHECK_IS_TRUE(expectedFrontFace == c.FrontFace());

		const Rectangle expectedRightFace{ bottomB, bottomC, topC, topB };
		const bool isRightFaceValid = CHECK_IS_TRUE(expectedRightFace == c.RightFace());

		const Rectangle expectedBackFace{ bottomC, bottomD, topD, topC };
		const bool isBackFaceValid = CHECK_IS_TRUE(expectedBackFace == c.BackFace());

		const Rectangle expectedLeftFace{ bottomD, bottomA, topA, topD };
		const bool isLeftFaceValid = CHECK_IS_TRUE(expectedLeftFace == c.LeftFace());

		const Rectangle expectedBottomFace{ bottomA, bottomB, bottomC, bottomD };
		const bool isBottomFaceValid = CHECK_IS_TRUE(expectedBottomFace == c.BottomFace());

		const Rectangle expectedTopFace{ topA, topB, topC, topD };
		const bool isTopFaceValid = CHECK_IS_TRUE(expectedTopFace == c.TopFace());

		return (isFrontFaceValid && isRightFaceValid && isBackFaceValid 
			&& isLeftFaceValid && isBottomFaceValid && isTopFaceValid);
	}

	bool UT_CubeCollider::AvoidCollision()
	{
		const glm::vec3 cubeCenterA{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 cubeCenterB{ 5.0f, 0.0f, 0.0f };

		Setup setup{ cubeCenterA, cubeCenterB };
		const glm::vec3 destination{ 10.0f, 0.0f, 0.0f };
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ 3.0f, 0.0f, 0.0f };

		return CHECK_EQUALS(returnedDestination, expectedDestination);
	}
}