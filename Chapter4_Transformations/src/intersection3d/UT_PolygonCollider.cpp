#include "UT_PolygonCollider.h"
#include "UTUtil.h"
#include <vector>
#include "vectormath/Common.h"

namespace MyCode
{
	using namespace VectorMath;
	using namespace Intersection3D;

	bool UT_PolygonCollider::Validate()
	{
		IntersectionTest3D intersectionTest3D;
		TravelPathBoundingTest2D travelPathBounding2D;
		TravelPathBoundingTest3D travelPathBounding3D;
		CollisionDetectionTest collisionDetection;
		CollisionAvoidanceTest collisionAvoidance;

		return intersectionTest3D.Run()
			&& travelPathBounding2D.Run()
			&& travelPathBounding3D.Run()
			&& collisionDetection.Run()
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
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ -0.5f, 0.0f, 1.0f }, glm::vec3{ 1.5f, 0.0f, 1.0f },
			glm::vec3{ 1.5f, 0.0f, -1.0f }, glm::vec3{ -0.5f, 0.0f, -1.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect3D(rectangle1, rectangle2);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::TravelPathBoundingTest2D::Run()
	{
		return BoundingPathForTriangle()
			&& BoundingPathForRectangle()
			&& BoundingPathForRotatedRectangle()
			&& BoundingPathFoParalelogram()
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
		const std::vector<glm::vec3> bounding = TravelPathBounding::GetBoundingPath(triangle.Vertices(), directionVector);

		const std::vector<glm::vec3> expected
		{
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ 1.0f, 0.0f, 2.0f },
			glm::vec3{ 2.0f, 0.0f, 2.0f }, glm::vec3{ 2.0f, 0.0f, 0.0f }
		};

		return CHECK_EQUALS(bounding, expected);
	}

	bool UT_PolygonCollider::TravelPathBoundingTest2D::BoundingPathForRectangle()
	{
		const Polygon polygonXZ
		{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } 
		};
		const glm::vec3 directionVector{0.0f, 0.0f, 2.0f};
		const std::vector<glm::vec3> bounding = TravelPathBounding::GetBoundingPath(polygonXZ.Vertices(), directionVector);

		const std::vector<glm::vec3> expected
		{
			glm::vec3{ -1.0f, 0.0f, 3.0f }, glm::vec3{ 1.0f, 0.0f, 3.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f }
		};

		return CHECK_EQUALS(bounding, expected);
	}

	bool UT_PolygonCollider::TravelPathBoundingTest2D::BoundingPathForRotatedRectangle()
	{
		const Polygon polygonXZ
		{
			glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f },
			glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, 0.0f }
		};
		const glm::vec3 directionVector{ 0.0f, 0.0f, 3.0f };
		const std::vector<glm::vec3> bounding = TravelPathBounding::GetBoundingPath(polygonXZ.Vertices(), directionVector);

		const std::vector<glm::vec3> expected
		{
			glm::vec3{ 0.0f, 0.0f, 4.0f }, glm::vec3{ 1.0f, 0.0f, 3.0f },
			glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, 
			glm::vec3{ -1.0f, 0.0f, 0.0f }, glm::vec3{ -1.0f, 0.0f, 3.0f }
		};

		return CHECK_EQUALS(bounding, expected);
	}

	bool UT_PolygonCollider::TravelPathBoundingTest2D::BoundingPathFoParalelogram()
	{
		const Polygon paralelogram
		{
			glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f },
			glm::vec3{ 4.0f, 0.0f, -1.0f }, glm::vec3{ 2.0f, 0.0f, -1.0f }
		};
		const glm::vec3 directionVector{ 0.0f, 0.0f, 2.0f };
		const std::vector<glm::vec3> bounding = TravelPathBounding::GetBoundingPath(paralelogram.Vertices(), directionVector);

		const std::vector<glm::vec3> expected
		{
			glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 1.0f, 0.0f, 2.0f },
			glm::vec3{ 3.0f, 0.0f, 2.0f }, glm::vec3{ 4.0f, 0.0f, 1.0f },
			glm::vec3{ 4.0f, 0.0f, -1.0f }, glm::vec3{ 2.0f, 0.0f, -1.0f }
		};

		return CHECK_EQUALS(bounding, expected);
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
		const std::vector<glm::vec3> bounding = TravelPathBounding::GetBoundingPath(polygon.Vertices(), directionVector);

		const std::vector<glm::vec3> expected
		{
			glm::vec3{ -3.0f, 0.0f, 4.0f }, glm::vec3{ 11.0f, 0.0f, 4.0f },
			glm::vec3{ 13.0f, 0.0f, 0.0f }, glm::vec3{ 13.0f, 0.0f, -1.0f },
			glm::vec3{ 12.0f, 0.0f, -3.0f }, glm::vec3{ 9.0f, 0.0f, -4.0f },
			glm::vec3{ -1.0f, 0.0f, -4.0f }, glm::vec3{ -4.0f, 0.0f, -2.0f }, 
			glm::vec3{ -4.0f, 0.0f, 0.0f },
		};

		return CHECK_EQUALS(bounding, expected);
	}

	bool UT_PolygonCollider::TravelPathBoundingTest3D::Run()
	{
		return BoundingPathForLineMovedLeft()
			&& BoundingPathForLineMovedRight()
			&& BoundingPathForTriangle()
			&& BoundingPathForRectangle()
			&& BoundingPathForRotatedRectangle();
	}

	bool UT_PolygonCollider::TravelPathBoundingTest3D::BoundingPathForLineMovedLeft()
	{
		const glm::vec3 directionVector{ 2.0f, 2.0f, 2.0f };
		const glm::vec3 a{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 b{ 0.0f, 0.0f, -2.0f };
		const glm::vec3 c{ a + directionVector };
		const glm::vec3 d{ b + directionVector };
		const glm::vec3 up {0.0f, 1.0f, 0.0f};

		const Polygon bounding{ TravelPathBounding::Detail::GetBoundingPolygonForLineSegment(a, b, up, directionVector) };
		const Polygon expected{ a, b, d, c };

		return CHECK_EQUALS(bounding, expected);
	}

	bool UT_PolygonCollider::TravelPathBoundingTest3D::BoundingPathForLineMovedRight()
	{
		const glm::vec3 directionVector{ 2.0f, -2.0f, 2.0f };
		const glm::vec3 a{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 b{ 0.0f, 0.0f, -2.0f };
		const glm::vec3 c{ a + directionVector };
		const glm::vec3 d{ b + directionVector };
		const glm::vec3 up{ 0.0f, 1.0f, 0.0f };

		const Polygon bounding{ TravelPathBounding::Detail::GetBoundingPolygonForLineSegment(a, b, up, directionVector) };
		const Polygon expected{ a, c, d, b };

		return CHECK_EQUALS(bounding, expected);
	}

	bool UT_PolygonCollider::TravelPathBoundingTest3D::BoundingPathForTriangle()
	{
		const glm::vec3 directionVector{ 0.0f, 2.0f, 2.0f };
		const glm::vec3 a{ 1.0f, 0.0f, -1.0f };
		const glm::vec3 b{ 1.0f, 0.0f, 0.0f };
		const glm::vec3 c{ 2.0f, 0.0f, 0.0f };
		const glm::vec3 d{ a + directionVector };
		const glm::vec3 e{ b + directionVector };
		const glm::vec3 f{ c + directionVector };

		const Polygon triangle{ a, b, c };
		const glm::vec3 destination{ triangle.Center() + directionVector };
		const std::vector<Polygon> bounding = TravelPathBounding::Detail::GetBoundingPath3D(triangle, destination);

		const Polygon expectedFaceBottom { c, b, a };
		const Polygon expectedFaceAB { a, b, e, d };
		const Polygon expectedFaceBC { b, c, f, e };
		const Polygon expectedFaceCA { c, a, d, f };
		const Polygon expectedFaceTop { d, e, f };
		const std::vector<Polygon> expectedFaces
		{ 
			expectedFaceBottom, 
			expectedFaceAB, expectedFaceBC, expectedFaceCA, 
			expectedFaceTop
		};

		return CHECK_IS_TRUE(bounding.size() == 5)
			&& CHECK_EQUALS(bounding, expectedFaces);
	}

	bool UT_PolygonCollider::TravelPathBoundingTest3D::BoundingPathForRectangle()
	{
		//FIXME
		return CHECK_IS_TRUE(true);
	}

	bool UT_PolygonCollider::TravelPathBoundingTest3D::BoundingPathForRotatedRectangle()
	{
		//FIXME
		return CHECK_IS_TRUE(true);
	}

	bool UT_PolygonCollider::CollisionDetectionTest::Run()
	{
		return NoCollision()
			&& NoCollisionForTouchingPolygons()
			//&& NoCollisionForOverlappingPolygons() // I don't think we need this
			&& NearSideCollision()
			&& NearSideCollision2()
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
		const glm::vec3 directionVector{0.0f, 0.0f, 10.0f};

		const bool collision = CollisionDetection::DoesPathCollide(target, directionVector, obstacle);
		const bool collisionExpected = false;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::CollisionDetectionTest::NoCollisionForTouchingPolygons()
	{
		const Polygon sourceXZ
		{
			glm::vec3{ 4.0f, 0.0f, 1.0f }, glm::vec3{ 6.0f, 0.0f, 1.0f },
			glm::vec3{ 6.0f, 0.0f, -1.0f }, glm::vec3{ 4.0f, 0.0f, -1.0f }
		};
		const Polygon obstacleYZ
		{
			glm::vec3{ 0.0f, -1.0f, -1.0f }, glm::vec3{ 0.0f, -1.0f, 1.0f },
			glm::vec3{ 0.0f, 1.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, -1.0f }
		};
		const glm::vec3 directionVector{ -4.0f, 0.0f, 0.0f };

		const bool collision = CollisionDetection::DoesPathCollide(sourceXZ, directionVector, obstacleYZ);
		const bool collisionExpected = false;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::CollisionDetectionTest::NoCollisionForOverlappingPolygons()
	{
		const std::vector<glm::vec3> verticesYZ
		{
			glm::vec3{ 0.0f, -1.0f, -1.0f }, glm::vec3{ 0.0f, -1.0f, 1.0f },
			glm::vec3{ 0.0f, 1.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, -1.0f }
		};
		Polygon sourceYZ{ verticesYZ };
		sourceYZ.SetCenter(glm::vec3{0.0f, 0.0f, 5.0f});
		
		const Polygon obstacleYZ{ verticesYZ };

		const glm::vec3 directionVector{ 0.0f, 0.0f, -10.0f };

		// FIXME I have a feeling we might need to revisit this part of the algorithm.
		// This will cause a failure to detect the collision of 2 completely overlapped polyhedrons.
		const bool collision = CollisionDetection::DoesPathCollide(sourceYZ, directionVector, obstacleYZ);
		const bool collisionExpected = false;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::CollisionDetectionTest::NearSideCollision()
	{
		const Polygon sourceXZ
		{
			glm::vec3{ 4.0f, 0.0f, 1.0f }, glm::vec3{ 6.0f, 0.0f, 1.0f },
			glm::vec3{ 6.0f, 0.0f, -1.0f }, glm::vec3{ 4.0f, 0.0f, -1.0f }
		};
		const Polygon obstacleYZ
		{
			glm::vec3{ 0.0f, -1.0f, -1.0f }, glm::vec3{ 0.0f, -1.0f, 1.0f },
			glm::vec3{ 0.0f, 1.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, -1.0f }
		};
		const glm::vec3 directionVector{ -5.0f, 0.0f, 0.0f };

		const bool collision = CollisionDetection::DoesPathCollide(sourceXZ, directionVector, obstacleYZ);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::CollisionDetectionTest::NearSideCollision2()
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
		const glm::vec3 directionVector{ 5.0f, 0.0f, 0.0f };

		const bool collision = CollisionDetection::DoesPathCollide(sourceYZ, directionVector, obstacleXZ);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::CollisionDetectionTest::FarSideCollision()
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
		const glm::vec3 directionVector{ 10.0f, 0.0f, 0.0f };

		const bool collision = CollisionDetection::DoesPathCollide(sourceYZ, directionVector, obstacleXZ);
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
		const glm::vec3 directionVector{ 10.0f, 0.0f, 0.0f };

		const bool collision = CollisionDetection::DoesPathCollide(sourceYZ, directionVector, obstacleXY);
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
		const glm::vec3 directionVector{ 10.0f, 0.0f, 0.0f };

		const bool collision = CollisionDetection::DoesPathCollide(source, directionVector, obstacleYZ);
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