#include "UT_PolygonCollider.h"
#include "../framework/UTUtil.h"
#include "../framework/VectorMath.h"
#include <vector>

namespace MyCode
{
	using namespace VectorMath;
	using namespace Intersection3D;

	bool UT_PolygonCollider::Validate()
	{
		IntersectionTest3D intersectionTest3D;
		CollisionDetectionTest pathCollisionDetection;
		CollisionAvoidanceTest collisionAvoidance;

		return intersectionTest3D.Run()
			&& pathCollisionDetection.Run()
			&& collisionAvoidance.Run();
	}

	bool UT_PolygonCollider::IntersectionTest3D::Run()
	{
		return NoCollision()
			&& CrossCollision()
			&& SamePlaneCollision();
	}

	bool UT_PolygonCollider::IntersectionTest3D::NoCollision()
	{
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ -1.0f, 1.0f, 1.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f },
			glm::vec3{ 1.0f, 1.0f, -1.0f }, glm::vec3{ -1.0f, 1.0f, -1.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect3D(rectangle1, rectangle2);
		const bool collisionExpected = false;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::IntersectionTest3D::CrossCollision()
	{
		const std::vector<glm::vec3> rectangleInXZ{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangleInXY{
			glm::vec3{ -1.0f, -1.0f, 0.0f }, glm::vec3{ 1.0f, -1.0f, 0.0f },
			glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec3{ -1.0f, 1.0f, 0.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect3D(rectangleInXZ, rectangleInXY);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::IntersectionTest3D::SamePlaneCollision()
	{
		const std::vector<glm::vec3> rectangleInXZ{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangleInXY{
			glm::vec3{ -0.5f, 0.0f, 1.0f }, glm::vec3{ 1.5f, 0.0f, 1.0f },
			glm::vec3{ 1.5f, 0.0f, -1.0f }, glm::vec3{ -0.5f, 0.0f, -1.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect3D(rectangleInXZ, rectangleInXY);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::CollisionDetectionTest::Run()
	{
		return NoCollision()
			&& NearSideCollision()
			&& FarSideCollision();
	}

	bool UT_PolygonCollider::CollisionDetectionTest::NoCollision()
	{
		const Polygon target{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const Polygon obstacle{
			glm::vec3{ 4.0f, -1.0f, 0.0f }, glm::vec3{ 6.0f, -1.0f, 0.0f },
			glm::vec3{ 6.0f, 1.0f, 0.0f }, glm::vec3{ 4.0f, 1.0f, 0.0f } };
		const glm::vec3 destination{0.0f, 0.0f, 10.0f};

		const bool collision = CollisionDetection::DoesPathCollide(target, destination, obstacle);
		const bool collisionExpected = false;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::CollisionDetectionTest::NearSideCollision()
	{
		const Polygon sourceYZ{
			glm::vec3{ 0.0f, -1.0f, -1.0f }, glm::vec3{ 0.0f, -1.0f, 1.0f },
			glm::vec3{ 0.0f, 1.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, -1.0f } };
		const Polygon obstacleXZ{
			glm::vec3{ 4.0f, 0.0f, 1.0f }, glm::vec3{ 6.0f, 0.0f, 1.0f },
			glm::vec3{ 6.0f, 0.0f, -1.0f }, glm::vec3{ 4.0f, 0.0f, -1.0f } };
		const glm::vec3 destination{ 5.0f, 0.0f, 0.0f };

		const bool collision = CollisionDetection::DoesPathCollide(sourceYZ, destination, obstacleXZ);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::CollisionDetectionTest::FarSideCollision()
	{
		const Polygon sourceYZ{
			glm::vec3{ 0.0f, -1.0f, -1.0f }, glm::vec3{ 0.0f, -1.0f, 1.0f },
			glm::vec3{ 0.0f, 1.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, -1.0f } };
		const Polygon obstacleXZ{
			glm::vec3{ 4.0f, 0.0f, 1.0f }, glm::vec3{ 6.0f, 0.0f, 1.0f },
			glm::vec3{ 6.0f, 0.0f, -1.0f }, glm::vec3{ 4.0f, 0.0f, -1.0f } };
		const glm::vec3 destination{ 10.0f, 0.0f, 0.0f };

		const bool collision = CollisionDetection::DoesPathCollide(sourceYZ, destination, obstacleXZ);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::CollisionAvoidanceTest::Run()
	{
		return ValidPositionForNoCollision()
			&& ValidPositionForPointCollision()
			&& ValidPositionForSideCollision()
			&& MoveAwayOverlappingPolygons()
			&& MoveAwayTouchingPolygons()
			&& MoveAwayFullyOverlappedPolygons();
	}

	bool UT_PolygonCollider::CollisionAvoidanceTest::ValidPositionForNoCollision()
	{
		const glm::vec3 centerA{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 centerB{ 5.0f, 0.0f, 0.0f };
		Setup setup{ centerA, centerB};
		
		const glm::vec3 destination{3.0f, 0.0f, 0.0f};
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ destination };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedDestination, expectedDestination));
	}

	bool UT_PolygonCollider::CollisionAvoidanceTest::ValidPositionForPointCollision()
	{
		const Polygon polygonRotatedAroundZ{ 
			glm::vec3{ -1.0f, -1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, 
			glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f } };
		const Polygon polygonInYZ{ 
			glm::vec3{ 5.0f, -1.0f, -1.0f }, glm::vec3{ 5.0f, -1.0f, 1.0f },
			glm::vec3{ 5.0f, 1.0f, 1.0f }, glm::vec3{ 5.0f, 1.0f, -1.0f } };
		Setup setup{ polygonRotatedAroundZ, polygonInYZ };

		const glm::vec3 destination{ 6.0f, 0.0f, 0.0f };
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ 4.0f, 0.0f, 0.0f };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedDestination, expectedDestination));
	}

	bool UT_PolygonCollider::CollisionAvoidanceTest::ValidPositionForSideCollision()
	{
		const Polygon polygonRotatedAroundZ{
			glm::vec3{ -1.0f, -1.0f, -1.0f }, glm::vec3{ -1.0f, -1.0f, 1.0f },
			glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec3{ 1.0f, 1.0f, -1.0f } };
		const Polygon polygonInYZ{
			glm::vec3{ 5.0f, -1.0f, -1.0f }, glm::vec3{ 5.0f, -1.0f, 1.0f },
			glm::vec3{ 5.0f, 1.0f, 1.0f }, glm::vec3{ 5.0f, 1.0f, -1.0f } };
		Setup setup{ polygonRotatedAroundZ, polygonInYZ };

		const glm::vec3 destination{ 6.0f, 0.0f, 0.0f };
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ 4.0f, 0.0f, 0.0f };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedDestination, expectedDestination));
	}

	bool UT_PolygonCollider::CollisionAvoidanceTest::MoveAwayOverlappingPolygons()
	{
		const std::initializer_list<glm::vec3> vertices
		{ 
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } 
		};

		Polygon polygon1{ vertices };
		Polygon polygon2{ vertices };
		polygon1.SetCenter(glm::vec3{ 0.0f, 0.0f, 0.0f });
		polygon2.SetCenter(glm::vec3{ 0.5f, 0.0f, 0.0f });
		Setup setup{ polygon1, polygon2 };

		const glm::vec3 destination{ -6.0f, 0.0f, 0.0f };
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ destination };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedDestination, expectedDestination));
	}

	bool UT_PolygonCollider::CollisionAvoidanceTest::MoveAwayTouchingPolygons()
	{
		Polygon polygon1
		{
			glm::vec3{ -1.0f, -1.0f, 1.0f }, glm::vec3{ 1.0f, -1.0f, 1.0f },
			glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec3{ -1.0f, 1.0f, 1.0f }
		};
		Polygon polygon2
		{
			glm::vec3{ 1.0f, -1.0f, 1.0f }, glm::vec3{ 3.0f, -1.0f, 1.0f },
			glm::vec3{ 3.0f, 1.0f, 1.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f }
		};
		Setup setup{ polygon1, polygon2 };

		const glm::vec3 destination{ -10.0f, 0.0f, 1.0f };
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ destination };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedDestination, expectedDestination));
	}

	bool UT_PolygonCollider::CollisionAvoidanceTest::MoveAwayFullyOverlappedPolygons()
	{
		Polygon polygon1
		{
			glm::vec3{ 1.0f, -1.0f, 1.0f }, glm::vec3{ 1.0f, -1.0f, -1.0f },
			glm::vec3{ 1.0f, 1.0f, -1.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f }
		};
		Polygon polygon2
		{
			glm::vec3{ 1.0f, -1.0f, -1.0f }, glm::vec3{ 1.0f, -1.0f, 1.0f },
			glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec3{ 1.0f, 1.0f, -1.0f }
		};
		Setup setup{ polygon1, polygon2 };

		const glm::vec3 destination{ -9.0f, 0.0f, 0.0f };
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ destination };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedDestination, expectedDestination));
	}
}