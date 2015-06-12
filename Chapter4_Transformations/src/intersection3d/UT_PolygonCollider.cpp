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
		TravelPathBoundingTest2D travelPathBounding2D;
		//TravelPathBoundingTest3D travelPathBounding3D;

		return intersectionTest3D.Run()
			&& travelPathBounding2D.Run()
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

	bool UT_PolygonCollider::TravelPathBoundingTest2D::Run()
	{
		return BoundingPathForTriangle()
			&& BoundingPathForRectangle()
			&& BoundingPathForRotatedRectangle()
			&& BoundingPathForConvexIrregularPolygon();
	}

	bool UT_PolygonCollider::TravelPathBoundingTest2D::BoundingPathForTriangle()
	{
		const Polygon triangle
		{
			glm::vec3{ 1.0f, 0.0f, -1.0f }, 
			glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 2.0f, 0.0f, 0.0f }
		};
		const glm::vec3 directionVector{ 0.0f, 0.0f, 2.0f };
		const glm::vec3 destination{ triangle.Center() + directionVector };
		const std::vector<Polygon> bounding = TravelPathBounding::GetBoundingPath(triangle, destination);

		const Polygon expected
		{
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ 1.0f, 0.0f, 2.0f },
			glm::vec3{ 2.0f, 0.0f, 2.0f }, glm::vec3{ 2.0f, 0.0f, 0.0f }
		};

		return CHECK_IS_TRUE(bounding.size() == 1)
			&& CHECK_EQUALS(bounding[0], expected);
	}

	bool UT_PolygonCollider::TravelPathBoundingTest2D::BoundingPathForRectangle()
	{
		const Polygon polygonXZ
		{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } 
		};
		const glm::vec3 destination{0.0f, 0.0f, 2.0f};
		const std::vector<Polygon> bounding = TravelPathBounding::GetBoundingPath(polygonXZ, destination);

		const Polygon expected
		{
			glm::vec3{ -1.0f, 0.0f, 3.0f }, glm::vec3{ 1.0f, 0.0f, 3.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f }
		};

		return CHECK_IS_TRUE(bounding.size() == 1) 
			&& CHECK_EQUALS(bounding[0], expected);
	}

	bool UT_PolygonCollider::TravelPathBoundingTest2D::BoundingPathForRotatedRectangle()
	{
		const Polygon polygonXZ
		{
			glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f },
			glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, 0.0f }
		};
		const glm::vec3 destination{ 0.0f, 0.0f, 3.0f };
		const std::vector<Polygon> bounding = TravelPathBounding::GetBoundingPath(polygonXZ, destination);

		const Polygon expected
		{
			glm::vec3{ 0.0f, 0.0f, 4.0f }, glm::vec3{ 1.0f, 0.0f, 3.0f },
			glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, 
			glm::vec3{ -1.0f, 0.0f, 0.0f }, glm::vec3{ -1.0f, 0.0f, 3.0f }
		};

		return CHECK_IS_TRUE(bounding.size() == 1)
			&& CHECK_EQUALS(bounding[0], expected);
	}

	bool UT_PolygonCollider::TravelPathBoundingTest2D::BoundingPathForConvexIrregularPolygon()
	{
		const Polygon polygon
		{
			glm::vec3{ -3.0f, 0.0f, 4.0f }, glm::vec3{ 1.0f, 0.0f, 4.0f },
			glm::vec3{ 3.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, -1.0f },
			glm::vec3{ 2.0f, 0.0f, -3.0f }, glm::vec3{ -1.0f, 0.0f, -4.0f },
			glm::vec3{ -4.0f, 0.0f, -2.0f }, glm::vec3{ -4.0f, 0.0f, 0.0f },
		};
		const glm::vec3 directionVector{ 10.0f, 0.0f, 0.0f };
		const glm::vec3 destination{ polygon.Center() + directionVector };
		const std::vector<Polygon> bounding = TravelPathBounding::GetBoundingPath(polygon, destination);

		const Polygon expected
		{
			glm::vec3{ -3.0f, 0.0f, 4.0f }, glm::vec3{ 11.0f, 0.0f, 4.0f },
			glm::vec3{ 13.0f, 0.0f, 0.0f }, glm::vec3{ 13.0f, 0.0f, -1.0f },
			glm::vec3{ 12.0f, 0.0f, -3.0f }, glm::vec3{ 9.0f, 0.0f, -4.0f },
			glm::vec3{ -1.0f, 0.0f, -4.0f }, glm::vec3{ -4.0f, 0.0f, -2.0f }, 
			glm::vec3{ -4.0f, 0.0f, 0.0f },
		};

		return CHECK_IS_TRUE(bounding.size() == 1)
			&& CHECK_EQUALS(bounding[0], expected);
	}

	bool UT_PolygonCollider::CollisionDetectionTest::Run()
	{
		return NoCollision()
			&& NearSideCollision()
			&& FarSideCollision()
			&& NoCollisionForSlidingPolygons()
			&& PointCollision();
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
		const Polygon sourceYZ
		{
			glm::vec3{ 0.0f, -1.0f, -1.0f }, glm::vec3{ 0.0f, -1.0f, 1.0f },
			glm::vec3{ 0.0f, 1.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, -1.0f } 
		};
		const Polygon obstacleXZ
		{
			glm::vec3{ 4.0f, 0.0f, 1.0f }, glm::vec3{ 6.0f, 0.0f, 1.0f },
			glm::vec3{ 6.0f, 0.0f, -1.0f }, glm::vec3{ 4.0f, 0.0f, -1.0f } 
		};
		const glm::vec3 destination{ 5.0f, 0.0f, 0.0f };

		const bool collision = CollisionDetection::DoesPathCollide(sourceYZ, destination, obstacleXZ);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::CollisionDetectionTest::FarSideCollision()
	{
		const Polygon sourceYZ
		{
			glm::vec3{ 0.0f, -1.0f, -1.0f }, glm::vec3{ 0.0f, -1.0f, 1.0f },
			glm::vec3{ 0.0f, 1.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, -1.0f } };
		const Polygon obstacleXZ
		{
			glm::vec3{ 4.0f, 0.0f, 1.0f }, glm::vec3{ 6.0f, 0.0f, 1.0f },
			glm::vec3{ 6.0f, 0.0f, -1.0f }, glm::vec3{ 4.0f, 0.0f, -1.0f } 
		};
		const glm::vec3 destination{ 10.0f, 0.0f, 0.0f };

		const bool collision = CollisionDetection::DoesPathCollide(sourceYZ, destination, obstacleXZ);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::CollisionDetectionTest::NoCollisionForSlidingPolygons()
	{
		const Polygon sourceYZ
		{
			glm::vec3{ 0.0f, -1.0f, -2.0f }, glm::vec3{ 0.0f, -1.0f, 0.0f },
			glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, -2.0f } 
		};
		const Polygon obstacleXY
		{
			glm::vec3{ 4.0f, -1.0f, 0.0f }, glm::vec3{ 6.0f, -1.0f, 0.0f },
			glm::vec3{ 6.0f, 1.0f, 0.0f }, glm::vec3{ 4.0f, 1.0f, 0.0f }
		};
		const glm::vec3 destination{ 10.0f, 0.0f, 0.0f };

		const bool collision = CollisionDetection::DoesPathCollide(sourceYZ, destination, obstacleXY);
		const bool collisionExpected = false;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::CollisionDetectionTest::PointCollision()
	{
		const Polygon source
		{
			glm::vec3{ -1.0f, -1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f } 
		};
		const Polygon obstacleYZ
		{
			glm::vec3{ 6.0f, -1.0f, 1.0f }, glm::vec3{ 6.0f, 1.0f, 1.0f },
			glm::vec3{ 6.0f, 1.0f, -1.0f }, glm::vec3{ 6.0f, -1.0f, -1.0f }
		};
		const glm::vec3 destination{ -10.0f, 0.0f, 0.0f };

		const bool collision = CollisionDetection::DoesPathCollide(source, destination, obstacleYZ);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::CollisionAvoidanceTest::Run()
	{
		return ValidPositionForNoCollision()
			&& ValidPositionForPointCollision()
			&& ValidPositionForSideCollision()
			&& ValidPositionForTouchingSideCollision()
			&& MoveAwayOverlappingPolygons()
			&& MoveAwayFullyOverlappedPolygons()
			&& MoveAwayTouchingPolygons()
			&& SlideTouchingPolygons()
			&& SlideTouchingPolygons2();
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

	bool UT_PolygonCollider::CollisionAvoidanceTest::ValidPositionForTouchingSideCollision()
	{
		const glm::vec3 centerA{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 centerB{ 1.0f, 0.0f, 0.0f };
		Setup setup{ centerA, centerB };

		const glm::vec3 destination{ 6.0f, 0.0f, 0.0f };
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ setup[0].Center() };

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

	bool UT_PolygonCollider::CollisionAvoidanceTest::SlideTouchingPolygons()
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

		const glm::vec3 destination{ 0.0f, 0.0f, 2.0f };
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ destination };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedDestination, expectedDestination));
	}

	bool UT_PolygonCollider::CollisionAvoidanceTest::SlideTouchingPolygons2()
	{
		Polygon polygon1
		{
			glm::vec3{ 1.0f, -1.0f, -1.0f }, glm::vec3{ 1.0f, -1.0f, 1.0f },
			glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec3{ 1.0f, 1.0f, -1.0f }
		};
		Polygon polygon2
		{
			glm::vec3{ 1.0f, -1.0f, 1.0f }, glm::vec3{ 3.0f, -1.0f, 1.0f },
			glm::vec3{ 3.0f, 1.0f, 1.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f }
		};
		Setup setup{ polygon1, polygon2 };

		const glm::vec3 destination{ 1.0f, 0.0f, 2.0f };
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ destination };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedDestination, expectedDestination));
	}
}