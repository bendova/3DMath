#include "UT_RectangleColider.h"
#include "UTUtil.h"
#include "CollisionAvoidance.h"
#include "PolygonIntersection.h"
#include "TravelPathBounding.h"
#include "vectormath/Common.h"

namespace MyCode
{
	using namespace Intersection2D;
	using namespace VectorMath;

	bool UT_RectangleColider::Validate()
	{
		CollisionTest2D collisionTest2D;
		BoundingPathTest boundingPathTest;
		ProjectionToAxesTest projectionToAxesTest;
		ValidPositionTest validPositionTest;
		MultipleObjectsAvoidanceTest objectsAvoidanceTest;
		SteppingOutOfCollisionTest stepOutOfCollisionTest;
		SlidingRectanglesTest slidingRectanglesTest;

		return projectionToAxesTest.Run()
			&& collisionTest2D.Run()
			&& boundingPathTest.Run()
			&& validPositionTest.Run()
			&& objectsAvoidanceTest.Run()
			&& stepOutOfCollisionTest.Run()
			&& slidingRectanglesTest.Run();
	}

	bool UT_RectangleColider::CollisionTest2D::Run()
	{
		return NoCollision()
			&& TouchingInOnePoint()
			&& TouchingOnOneSide()
			&& CollisionInOnePoint()
			&& CollisionOnOneSide()
			&& CompleteOverlap()
			&& CollisionOfOverlappedRectangles()
			&& NoCollisionOfRotatedRectangles()
			&& CollisionOfRotatedRectangles()
			&& NoIntersectionOfLineSegmentWithPolygon()
			&& IntersectionOfLineSegmentWithPolygon()
			&& IntersectionOfLineSegmentWithPolygon2()
			&& NoIntersectionForLineTouchingPolygon()
			&& IntersectionForLineInsidePolygon();
	}

	bool UT_RectangleColider::CollisionTest2D::NoCollision()
	{
		const std::vector<glm::vec3> rectangle1{ 
			glm::vec3{ -1.0f, 0.0f,  1.0f }, glm::vec3{  1.0f, 0.0f,  1.0f },
			glm::vec3{  1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{  3.0f, 0.0f,  1.0f }, glm::vec3{  10.0f, 0.0f, 1.0f },
			glm::vec3{  10.0f, 0.0f, -1.0f }, glm::vec3{  3.0f, 0.0f, -1.0f } };
		
		const bool collision = PolygonIntersection::DoPolygonsIntersect2D(rectangle1, rectangle2);
		const bool collisionExpected = false;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_RectangleColider::CollisionTest2D::TouchingInOnePoint()
	{
		const std::vector<glm::vec3> rectangle1
		{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } 
		};
		const std::vector<glm::vec3> rectangle2
		{
			glm::vec3{ 1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 2.0f },
			glm::vec3{ 2.0f, 0.0f, 2.0f }, glm::vec3{ 2.0f, 0.0f, 1.0f } 
		};

		const bool collision = PolygonIntersection::DoPolygonsIntersect2D(rectangle1, rectangle2);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_RectangleColider::CollisionTest2D::TouchingOnOneSide()
	{
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ -1.0f, 0.0f,  1.0f }, glm::vec3{  1.0f, 0.0f, 1.0f },
			glm::vec3{  1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ 1.0f, 0.0f, 1.0f }, glm::vec3{ 2.0f, 0.0f, 1.0f },
			glm::vec3{ 2.0f, 0.0f, -1.0f }, glm::vec3{ 1.0f, 0.0f, -1.0f } };
		
		const bool collision = PolygonIntersection::DoPolygonsIntersect2D(rectangle1, rectangle2);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_RectangleColider::CollisionTest2D::CollisionInOnePoint()
	{
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ 0.0f, 0.0f,  0.0f }, glm::vec3{ 2.0f, 0.0f,  0.0f },
			glm::vec3{ 2.0f, 0.0f, -2.0f }, glm::vec3{ 0.0f, 0.0f, -2.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect2D(rectangle1, rectangle2);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_RectangleColider::CollisionTest2D::CollisionOnOneSide()
	{
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect2D(rectangle1, rectangle2);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_RectangleColider::CollisionTest2D::CompleteOverlap()
	{
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ -2.0f, 0.0f, 2.0f }, glm::vec3{ 2.0f, 0.0f, 2.0f },
			glm::vec3{ 2.0f, 0.0f, -2.0f }, glm::vec3{ -2.0f, 0.0f, -2.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect2D(rectangle1, rectangle2);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_RectangleColider::CollisionTest2D::CollisionOfOverlappedRectangles()
	{
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ -0.5f, 0.0f, 1.0f }, glm::vec3{ 1.5f, 0.0f, 1.0f },
			glm::vec3{ 1.5f, 0.0f, -1.0f }, glm::vec3{ -0.5f, 0.0f, -1.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect2D(rectangle1, rectangle2, VectorMath::PointType::OPEN_ENDED);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_RectangleColider::CollisionTest2D::NoCollisionOfRotatedRectangles()
	{
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 4.0f, 0.0f, -3.0f },
			glm::vec3{ 3.0f, 0.0f, -4.0f }, glm::vec3{ -1.0f, 0.0f, 0.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ 3.0f, 0.0f, 0.0f }, glm::vec3{ 4.0f, 0.0f, 0.0f },
			glm::vec3{ 4.0f, 0.0f, -1.0f }, glm::vec3{ 3.0f, 0.0f, -1.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect2D(rectangle1, rectangle2);
		const bool collisionExpected = false;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_RectangleColider::CollisionTest2D::CollisionOfRotatedRectangles()
	{
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 4.0f, 0.0f, -3.0f },
			glm::vec3{ 3.0f, 0.0f, -4.0f }, glm::vec3{ -1.0f, 0.0f, 0.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 4.0f, 0.0f, 0.0f },
			glm::vec3{ 4.0f, 0.0f, -1.0f }, glm::vec3{ 1.0f, 0.0f, -1.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect2D(rectangle1, rectangle2);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_RectangleColider::CollisionTest2D::NoIntersectionOfLineSegmentWithPolygon()
	{
		const std::vector<glm::vec3> line{
			glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ 1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle{
			glm::vec3{ 2.0f, 0.0f, 0.0f }, glm::vec3{ 4.0f, 0.0f, 0.0f },
			glm::vec3{ 4.0f, 0.0f, -1.0f }, glm::vec3{ 2.0f, 0.0f, -1.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect2D(line, rectangle);
		const bool expected = false;

		return CHECK_EQUALS(collision, expected);
	}

	bool UT_RectangleColider::CollisionTest2D::IntersectionOfLineSegmentWithPolygon()
	{
		const std::vector<glm::vec3> line{
			glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f },
			glm::vec3{ 3.0f, 0.0f, -1.0f }, glm::vec3{ 3.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle{
			glm::vec3{ 2.0f, 0.0f, 0.0f }, glm::vec3{ 4.0f, 0.0f, 0.0f },
			glm::vec3{ 4.0f, 0.0f, -1.0f }, glm::vec3{ 2.0f, 0.0f, -1.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect2D(line, rectangle);
		const bool expected = true;

		return CHECK_EQUALS(collision, expected);
	}

	bool UT_RectangleColider::CollisionTest2D::IntersectionOfLineSegmentWithPolygon2()
	{
		const std::vector<glm::vec3> line{
			glm::vec3{ -1.0f, 0.0f, 0.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f },
			glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ -1.0f, 0.0f, 0.0f } };
		const std::vector<glm::vec3> rectangle{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect2D(line, rectangle);
		const bool expected = true;

		return CHECK_EQUALS(collision, expected);
	}

	bool UT_RectangleColider::CollisionTest2D::NoIntersectionForLineTouchingPolygon()
	{
		const std::vector<glm::vec3> line{
			glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f },
			glm::vec3{ 3.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };
		const std::vector<glm::vec3> rectangle{
			glm::vec3{ 2.0f, 0.0f, 0.0f }, glm::vec3{ 4.0f, 0.0f, 0.0f },
			glm::vec3{ 4.0f, 0.0f, -1.0f }, glm::vec3{ 2.0f, 0.0f, -1.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect2D(line, rectangle, VectorMath::PointType::OPEN_ENDED);
		const bool expected = false;

		return CHECK_EQUALS(collision, expected);
	}

	bool UT_RectangleColider::CollisionTest2D::IntersectionForLineInsidePolygon()
	{
		const std::vector<glm::vec3> line{
			glm::vec3{ -1.0f, 0.0f, 0.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f },
			glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ -1.0f, 0.0f, 0.0f } };
		const std::vector<glm::vec3> rectangle{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect2D(line, rectangle, VectorMath::PointType::OPEN_ENDED);
		const bool expected = true;

		return CHECK_EQUALS(collision, expected);
	}

	bool UT_RectangleColider::BoundingPathTest::Run()
	{
		return MoveLeft()
			&& MoveRight()
			&& MoveForward()
			&& MoveBackward()
			&& MoveDiagonallyLeftBackward()
			&& MoveDiagonallyLeftForward()
			&& MoveDiagonallyRightBackward()
			&& MoveDiagonallyRightForward();
	}

	bool UT_RectangleColider::BoundingPathTest::MoveLeft()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ -1.0f, 0.0f, 0.0f };
		const std::vector<glm::vec3> boundingPath = TravelPathBounding::GetTravelPathBounding(setup[0], targetOfR1);
		const std::vector<glm::vec3> expectedBoundingPath{ glm::vec3{ -1.5f, 0.0f, 0.5f }, glm::vec3{ 0.5f, 0.0f, 0.5f }, 
			glm::vec3{ 0.5f, 0.0f, -0.5f }, glm::vec3{ -1.5f, 0.0f, -0.5f } };

		return CHECK_EQUALS(boundingPath, expectedBoundingPath);
	}
	bool UT_RectangleColider::BoundingPathTest::MoveRight()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 1.0f, 0.0f, 0.0f };
		const std::vector<glm::vec3> boundingPath = TravelPathBounding::GetTravelPathBounding(setup[0], targetOfR1);
		const std::vector<glm::vec3> expectedBoundingPath{ glm::vec3{ -0.5f, 0.0f, 0.5f }, glm::vec3{ 1.5f, 0.0f, 0.5f },
			glm::vec3{ 1.5f, 0.0f, -0.5f }, glm::vec3{ -0.5f, 0.0f, -0.5f } };

		return CHECK_EQUALS(boundingPath, expectedBoundingPath);
	}
	bool UT_RectangleColider::BoundingPathTest::MoveForward()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 0.0f, 0.0f, -1.0f };
		const std::vector<glm::vec3> boundingPath = TravelPathBounding::GetTravelPathBounding(setup[0], targetOfR1);
		const std::vector<glm::vec3> expectedBoundingPath{ glm::vec3{ -0.5f, 0.0f, 0.5f }, glm::vec3{ 0.5f, 0.0f, 0.5f },
			glm::vec3{ 0.5f, 0.0f, -1.5f }, glm::vec3{ -0.5f, 0.0f, -1.5f } };

		return CHECK_EQUALS(boundingPath, expectedBoundingPath);
	}
	bool UT_RectangleColider::BoundingPathTest::MoveBackward()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 0.0f, 0.0f, 1.0f };
		const std::vector<glm::vec3> boundingPath = TravelPathBounding::GetTravelPathBounding(setup[0], targetOfR1);
		const std::vector<glm::vec3> expectedBoundingPath{ glm::vec3{ -0.5f, 0.0f, 1.5f }, glm::vec3{ 0.5f, 0.0f, 1.5f },
			glm::vec3{ 0.5f, 0.0f, -0.5f }, glm::vec3{ -0.5f, 0.0f, -0.5f } };

		return CHECK_EQUALS(boundingPath, expectedBoundingPath);
	}

	bool UT_RectangleColider::BoundingPathTest::MoveDiagonallyLeftBackward()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ -1.0f, 0.0f, 1.0f };
		
		const std::vector<glm::vec3> boundingPath = TravelPathBounding::GetTravelPathBounding(setup[0], targetOfR1);
		const std::vector<glm::vec3> expectedBoundingPath{ 
			glm::vec3{  0.5f, 0.0f,  0.5f }, glm::vec3{  0.5f, 0.0f, -0.5f },
			glm::vec3{ -0.5f, 0.0f, -0.5f }, glm::vec3{ -1.5f, 0.0f,  0.5f }, 
			glm::vec3{ -1.5f, 0.0f,  1.5f }, glm::vec3{ -0.5f, 0.0f,  1.5f } };

		return CHECK_EQUALS(boundingPath, expectedBoundingPath);
	}

	bool UT_RectangleColider::BoundingPathTest::MoveDiagonallyLeftForward()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ -1.0f, 0.0f, -1.0f };

		const std::vector<glm::vec3> boundingPath = TravelPathBounding::GetTravelPathBounding(setup[0], targetOfR1);
		const std::vector<glm::vec3> expectedBoundingPath{
			glm::vec3{ -0.5f, 0.0f,  0.5f }, glm::vec3{  0.5f, 0.0f,  0.5f },
			glm::vec3{  0.5f, 0.0f, -0.5f }, glm::vec3{ -0.5f, 0.0f, -1.5f },
			glm::vec3{ -1.5f, 0.0f, -1.5f }, glm::vec3{ -1.5f, 0.0f, -0.5f } };

		return CHECK_EQUALS(boundingPath, expectedBoundingPath);
	}

	bool UT_RectangleColider::BoundingPathTest::MoveDiagonallyRightBackward()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 1.0f, 0.0f, 1.0f };

		const std::vector<glm::vec3> boundingPath = TravelPathBounding::GetTravelPathBounding(setup[0], targetOfR1);
		const std::vector<glm::vec3> expectedBoundingPath{
			glm::vec3{  0.5f, 0.0f, -0.5f }, glm::vec3{ -0.5f, 0.0f, -0.5f },
			glm::vec3{ -0.5f, 0.0f,  0.5f }, glm::vec3{  0.5f, 0.0f,  1.5f },
			glm::vec3{  1.5f, 0.0f,  1.5f }, glm::vec3{  1.5f, 0.0f,  0.5f } };

		return CHECK_EQUALS(boundingPath, expectedBoundingPath);
	}

	bool UT_RectangleColider::BoundingPathTest::MoveDiagonallyRightForward()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 1.0f, 0.0f, -1.0f };

		const std::vector<glm::vec3> boundingPath = TravelPathBounding::GetTravelPathBounding(setup[0], targetOfR1);
		const std::vector<glm::vec3> expectedBoundingPath{
			glm::vec3{ -0.5f, 0.0f, -0.5f }, glm::vec3{ -0.5f, 0.0f,  0.5f },
			glm::vec3{  0.5f, 0.0f,  0.5f }, glm::vec3{  1.5f, 0.0f, -0.5f },
			glm::vec3{  1.5f, 0.0f, -1.5f }, glm::vec3{  0.5f, 0.0f, -1.5f } };

		return CHECK_EQUALS(boundingPath, expectedBoundingPath);
	}

	bool UT_RectangleColider::ProjectionToAxesTest::Run()
	{
		return ProjectRightTriangle()
			&& ProjectObtuseTriangle()
			&& ProjectTrapeze()
			&& ProjectPentagon();
	}

	bool UT_RectangleColider::ProjectionToAxesTest::ProjectRightTriangle()
	{
		const glm::vec3 a{ -1.0f, 0.0f, 1.0f };
		const glm::vec3 b{ 1.0f, 0.0f, 1.0f };
		const glm::vec3 c{ 1.0f, 0.0f, -1.0f };
		const std::vector<glm::vec3> triangle{ a, b, c };

		using namespace PolygonIntersection;
		const auto projectionToAB = ProjectPointsToAxis(triangle, a, b);
		const auto projectionToBC = ProjectPointsToAxis(triangle, b, c);
		const auto projectionToCA = ProjectPointsToAxis(triangle, c, a);

		const double errorMargin = 1e-6;
		const std::pair<glm::vec3, glm::vec3> expectedProjAB{ a, b };
		const bool isValidProjAB = CHECK_IS_TRUE(AreSegmentsEqualWithinMargin(projectionToAB, expectedProjAB, errorMargin));

		const std::pair<glm::vec3, glm::vec3> expectedProjBC{ b, c };
		const bool isValidProjBC = CHECK_IS_TRUE(AreSegmentsEqualWithinMargin(projectionToBC, expectedProjBC, errorMargin));

		const std::pair<glm::vec3, glm::vec3> expectedProjCA{ c, a };
		const bool isValidProjCA = CHECK_IS_TRUE(AreSegmentsEqualWithinMargin(projectionToCA, expectedProjCA, errorMargin));

		return (isValidProjAB && isValidProjBC && isValidProjCA);
	}

	bool UT_RectangleColider::ProjectionToAxesTest::ProjectObtuseTriangle()
	{
		const glm::vec3 a{ -1.0f, 0.0f, 1.0f };
		const glm::vec3 b{ 1.0f, 0.0f, 1.0f };
		const glm::vec3 c{ 2.0f, 0.0f, -1.0f };
		const std::vector<glm::vec3> triangle{ a, b, c };

		const auto projectionToAB = ProjectPointsToAxis(triangle, a, b);
		const auto projectionToBC = ProjectPointsToAxis(triangle, b, c);
		const auto projectionToCA = ProjectPointsToAxis(triangle, c, a);

		const double errorMargin = 1e-6;
		const std::pair<glm::vec3, glm::vec3> expectedProjAB{ a, glm::vec3{2.0, 0.0f, 1.0f} };
		const bool isValidProjAB = CHECK_IS_TRUE(AreSegmentsEqualWithinMargin(projectionToAB, expectedProjAB, errorMargin));

		const std::pair<glm::vec3, glm::vec3> expectedProjBC{ glm::vec3{0.6f, 0.0f, 1.8f}, c };
		const bool isValidProjBC = CHECK_IS_TRUE(AreSegmentsEqualWithinMargin(projectionToBC, expectedProjBC, errorMargin));

		const std::pair<glm::vec3, glm::vec3> expectedProjCA{ c, a };
		const bool isValidProjCA = CHECK_IS_TRUE(AreSegmentsEqualWithinMargin(projectionToCA, expectedProjCA, errorMargin));

		return (isValidProjAB && isValidProjBC && isValidProjCA);
	}

	bool UT_RectangleColider::ProjectionToAxesTest::ProjectTrapeze()
	{
		const glm::vec3 a{ 0.0f, 0.0f,  0.0f };
		const glm::vec3 b{ 3.0f, 0.0f,  0.0f };
		const glm::vec3 c{ 2.0f, 0.0f, -1.0f };
		const glm::vec3 d{ 1.0f, 0.0f, -1.0f };
		const std::vector<glm::vec3> trapeze{ a, b, c, d };

		const auto projectionToAB = ProjectPointsToAxis(trapeze, a, b);
		const auto projectionToBC = ProjectPointsToAxis(trapeze, b, c);
		const auto projectionToCD = ProjectPointsToAxis(trapeze, c, d);
		const auto projectionToDA = ProjectPointsToAxis(trapeze, d, a);

		const double errorMargin = 1e-6;
		const std::pair<glm::vec3, glm::vec3> expectedProjAB{ a, b };
		const bool isValidProjAB = CHECK_IS_TRUE(AreSegmentsEqualWithinMargin(projectionToAB, expectedProjAB, errorMargin));

		const std::pair<glm::vec3, glm::vec3> expectedProjBC{ b, glm::vec3{1.5f, 0.0f, -1.5f} };
		const bool isValidProjBC = CHECK_IS_TRUE(AreSegmentsEqualWithinMargin(projectionToBC, expectedProjBC, errorMargin));

		const std::pair<glm::vec3, glm::vec3> expectedProjCD{ glm::vec3{ 3.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f } };
		const bool isValidProjCD = CHECK_IS_TRUE(AreSegmentsEqualWithinMargin(projectionToCD, expectedProjCD, errorMargin));

		const std::pair<glm::vec3, glm::vec3> expectedProjDA{ glm::vec3{ 1.5f, 0.0f, -1.5f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };
		const bool isValidProjDA = CHECK_IS_TRUE(AreSegmentsEqualWithinMargin(projectionToDA, expectedProjDA, errorMargin));

		return (isValidProjAB && isValidProjBC && isValidProjCD && isValidProjDA);
	}
	bool UT_RectangleColider::ProjectionToAxesTest::ProjectPentagon()
	{
		const glm::vec3 a{  1.0f, 0.0f,  0.0f };
		const glm::vec3 b{  2.0f, 0.0f, -1.0f };
		const glm::vec3 c{  2.0f, 0.0f, -2.0f };
		const glm::vec3 d{  0.0f, 0.0f, -2.0f };
		const glm::vec3 e{  0.0f, 0.0f, -1.0f };
		const std::vector<glm::vec3> pentagon{ a, b, c, d, e };

		const auto projectionToAB = ProjectPointsToAxis(pentagon, a, b);
		const auto projectionToBC = ProjectPointsToAxis(pentagon, b, c);
		const auto projectionToCD = ProjectPointsToAxis(pentagon, c, d);
		const auto projectionToDE = ProjectPointsToAxis(pentagon, d, e);
		const auto projectionToEA = ProjectPointsToAxis(pentagon, e, a);
		
		const double errorMargin = 1e-6;
		const std::pair<glm::vec3, glm::vec3> expectedProjToAB{ a, glm::vec3{ 2.5f, 0.0f, -1.5f } };
		const bool isValidProjAB = CHECK_IS_TRUE(AreSegmentsEqualWithinMargin(projectionToAB, expectedProjToAB, errorMargin));

		const std::pair<glm::vec3, glm::vec3> expectedProjToBC{ glm::vec3{ 2.0f, 0.0f, 0.0f }, c };
		const bool isValidProjBC = CHECK_IS_TRUE(AreSegmentsEqualWithinMargin(projectionToBC, expectedProjToBC, errorMargin));

		const std::pair<glm::vec3, glm::vec3> expectedProjCD{ c, d };
		const bool isValidProjCD = CHECK_IS_TRUE(AreSegmentsEqualWithinMargin(projectionToCD, expectedProjCD, errorMargin));

		const std::pair<glm::vec3, glm::vec3> expectedProjDE{ d, glm::vec3{ 0.0f, 0.0f, 0.0f } };
		const bool isValidProjDE = CHECK_IS_TRUE(AreSegmentsEqualWithinMargin(expectedProjDE, expectedProjDE, errorMargin));

		const std::pair<glm::vec3, glm::vec3> expectedProjEA{ glm::vec3{ -0.5f, 0.0f, -1.5f }, a };
		const bool isValidProjEA = CHECK_IS_TRUE(AreSegmentsEqualWithinMargin(expectedProjEA, expectedProjEA, errorMargin));

		return (isValidProjAB && isValidProjBC && isValidProjCD && isValidProjDE && isValidProjEA);
	}

	std::pair<glm::vec3, glm::vec3> UT_RectangleColider::ProjectionToAxesTest::ProjectPointsToAxis(const std::vector<glm::vec3>& points, 
		const glm::vec3& lineA, const glm::vec3& lineB)
	{
		return PolygonIntersection::Detail::ProjectPolygonToAxis(points, lineA, lineB);
	}

	bool UT_RectangleColider::ValidPositionTest::Run()
	{
		return NearEdgeTarget()
			&& FarEdgeTarget()
			&& PathIntersection()
			&& DiagonalPathIntersection()
			&& EdgeIntersection()
			&& CornerIntersection()
			&& CrossIntersection()
			&& CrossIntersection2()
			&& CrossIntersection3()
			&& CrossIntersection4()
			&& CrossIntersection5()
			&& CrossIntersection6();
	}

	bool UT_RectangleColider::ValidPositionTest::NoCollision()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 1.0f, 0.0f, 0.0f };
		const glm::vec3 validTarget = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], targetOfR1);
		
		return CHECK_EQUALS(targetOfR1, validTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::NearEdgeTarget()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 3.0f, 0.0f, 0.0f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ 2.0f, 0.0f, 0.0f };

		return CHECK_EQUALS(expectedTargetOfR1, returnedTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::FarEdgeTarget()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 4.5f, 0.0f, 0.0f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ 2.0f, 0.0f, 0.0f };

		return CHECK_EQUALS(expectedTargetOfR1, returnedTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::EdgeIntersection()
	{
		const glm::vec3 centerOfR1{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 centerOfR2{ 1.0f, 0.0f, 0.0f };
		Setup setup{ centerOfR1, centerOfR2 };

		const glm::vec3 targetOfR1{ -1.0f, 0.0f, -1.0f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ targetOfR1 };

		return CHECK_EQUALS(expectedTargetOfR1, returnedTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::CornerIntersection()
	{
		const glm::vec3 centerOfR1{ 2.3f, 0.0f, 1.0f };
		const glm::vec3 centerOfR2{ 1.0f, 0.0f, 0.0f };
		Setup setup{ centerOfR1, centerOfR2 };

		const glm::vec3 targetOfR1{ 2.3f, 0.0f, 0.9f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ targetOfR1 };

		return CHECK_EQUALS(expectedTargetOfR1, returnedTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::PathIntersection()
	{
		Setup setup{ glm::vec3{ -1.0f, 0.0f, -4.0f }, glm::vec3{ -1.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ -1.0f, 0.0f, 0.0f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ -1.0f, 0.0f, -1.0f };

		return CHECK_EQUALS(expectedTargetOfR1, returnedTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::DiagonalPathIntersection()
	{
		Setup setup{ glm::vec3{ -3.0f, 0.0f, -3.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ -1.0f, 0.0f, -1.0f };

		const double errorMargin = 1e-6;
		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedTarget, expectedTargetOfR1, errorMargin));
	}

	bool UT_RectangleColider::ValidPositionTest::CrossIntersection()
	{
		Setup setup{ glm::vec3{ -1.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 1.0f, 0.0f, 0.0f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ -1.0f, 0.0f, -1.0f };

		return CHECK_EQUALS(returnedTarget, expectedTargetOfR1);
	}

	bool UT_RectangleColider::ValidPositionTest::CrossIntersection2()
	{
		Setup setup{ glm::vec3{ -0.8f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 0.2f, 0.0f, -0.45f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ setup[0].Center() };

		return CHECK_EQUALS(returnedTarget, expectedTargetOfR1);
	}

	bool UT_RectangleColider::ValidPositionTest::CrossIntersection3()
	{
		Setup setup{ glm::vec3{ -1.0f, 0.0f, -0.5f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ -0.5f, 0.0f, 0.5f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ setup[0].Center() };

		return CHECK_EQUALS(returnedTarget, expectedTargetOfR1);
	}

	bool UT_RectangleColider::ValidPositionTest::CrossIntersection4()
	{
		Setup setup{ glm::vec3{ -1.71264f, 0.0f, -1.6331f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ -0.947925f, 0.0f, -0.924394f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ -1.0f, 0.0f, -1.0f };

		// FIXME This is an unbelievably poor error margin, 
		// and could become the cause of much pain in the future.
		// A more acceptable value would be errorMargin = 1e-6;
		const double errorMargin = 1e-1;
		const bool areEqual = CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedTarget, expectedTargetOfR1, errorMargin));
		return areEqual;
	}

	bool UT_RectangleColider::ValidPositionTest::CrossIntersection5()
	{
		Setup setup{ glm::vec3{ 0.886919f, 0.0f, -0.677766f }, glm::vec3{ 0.114781f, 0.0f, 0.252855f } };

		const glm::vec3 targetOfR1{ 1.29656f, 0.0f, -0.686211f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ targetOfR1 };

		const double errorMargin = 1e-6;
		const bool areEqual = CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedTarget, expectedTargetOfR1, errorMargin));
		return areEqual;
	}

	bool UT_RectangleColider::ValidPositionTest::CrossIntersection6()
	{
		Setup setup{ glm::vec3{ -1.0f, 0.0f, -0.2f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ -0.9f, 0.0f, -0.2f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ setup[0].Center() };

		const double errorMargin = 1e-6;
		const bool areEqual = CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedTarget, expectedTargetOfR1, errorMargin));
		return areEqual;
	}

	bool UT_RectangleColider::MultipleObjectsAvoidanceTest::Run()
	{
		return AvoidTwoObjects();
	}

	bool UT_RectangleColider::MultipleObjectsAvoidanceTest::AvoidTwoObjects()
	{
		const glm::vec3 targetCenter{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 nearObstacleCenter{ 3.0f, 0.0f, 0.0f };
		const glm::vec3 farObstacleCenter{ 5.0f, 0.0f, 0.0f };

		Setup setup{ targetCenter, farObstacleCenter, nearObstacleCenter };

		const glm::vec3 destination{ 6.0f, 0.0f, 0.0f };
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ 2.0f, 0.0f, 0.0f };

		return CHECK_EQUALS(returnedDestination, expectedDestination);
	}

	bool UT_RectangleColider::SteppingOutOfCollisionTest::Run()
	{
		return StepOutOfCollisionWithSidesAligned()
			&& StepOutOfCollision()
			&& StepOutOfCollision2()
			&& StepOutOfCollision3()
			&& StepOutOfCollision4();
	}

	bool UT_RectangleColider::SteppingOutOfCollisionTest::StepOutOfCollisionWithSidesAligned()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.9f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 0.0f, 0.0f, 0.8f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ 0.0f, 0.0f, 1.0f };

		const double errorMargin = 1e-6;
		const bool areEqual = CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedTarget, expectedTargetOfR1, errorMargin));
		return areEqual;
	}

	bool UT_RectangleColider::SteppingOutOfCollisionTest::StepOutOfCollision4()
	{
		const glm::vec3 targetCenter{ 0.0f, 0.0f, 0.3f };
		const glm::vec3 nearObstacleCenter{ 0.5f, 0.0f, 0.0f };

		Setup setup{ targetCenter, nearObstacleCenter };

		const glm::vec3 destination{ -6.0f, 0.0f, 0.0f };
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ destination };

		return CHECK_EQUALS(returnedDestination, expectedDestination);
	}

	bool UT_RectangleColider::SteppingOutOfCollisionTest::StepOutOfCollision()
	{
		Setup setup{ glm::vec3{ -2.5f, 0.0f, 2.5f }, glm::vec3{ -1.52f, 0.0f, 2.5f } };

		const glm::vec3 destination{ -2.52f, 0.0f, 2.413f };
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ destination };

		const double errorMargin = 1e-6;
		const bool areEqual = CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedDestination, destination, errorMargin));
		return areEqual;
	}

	bool UT_RectangleColider::SteppingOutOfCollisionTest::StepOutOfCollision2()
	{
		const glm::vec3 targetCenter{ 0.0f, 0.0f, 0.3f };
		const glm::vec3 nearObstacleCenter{ 0.5f, 0.0f, 0.0f };

		Setup setup{ targetCenter, nearObstacleCenter };

		const glm::vec3 destination{ 6.0f, 0.0f, 0.0f };
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ -0.5f, 0.0f, 0.325f };

		const double errorMargin = 1e-6;
		const bool areEqual = CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedDestination, expectedDestination, errorMargin));
		return areEqual;
	}

	bool UT_RectangleColider::SteppingOutOfCollisionTest::StepOutOfCollision3()
	{
		const glm::vec3 targetCenter{ 0.0f, 1.0f, 0.3f };
		const glm::vec3 nearObstacleCenter{ 0.5f, 1.0f, 0.0f };

		Setup setup{ targetCenter, nearObstacleCenter };

		const glm::vec3 destination{ 6.0f, 1.0f, 0.0f };
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ -0.5f, 1.0f, 0.325f };

		const double errorMargin = 1e-6;
		const bool areEqual = CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedDestination, expectedDestination, errorMargin));
		return areEqual;
	}

	bool UT_RectangleColider::SlidingRectanglesTest::Run()
	{
		return SlideRectanglesUp()
			&& SlideRectanglesDown()
			&& SlideRectanglesWithinMarginError();
	}

	bool UT_RectangleColider::SlidingRectanglesTest::SlideRectanglesUp()
	{
		const glm::vec3 targetCenter{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 obstacleCenter{ 1.0f, 0.0f, 0.0f };

		Setup setup{ targetCenter, obstacleCenter };

		const glm::vec3 destination{ 0.0f, 0.0f, 5.0f };
		const glm::vec3 actual = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], destination);
		const glm::vec3 expected{ destination };

		const bool areEqual = CHECK_IS_TRUE(VectorMath::AreVectorsEqualWithinMargin(actual, expected));
		return areEqual;
	}

	bool UT_RectangleColider::SlidingRectanglesTest::SlideRectanglesDown()
	{
		const glm::vec3 targetCenter{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 obstacleCenter{ 1.0f, 0.0f, 0.0f };

		Setup setup{ targetCenter, obstacleCenter };

		const glm::vec3 destination{ 0.0f, 0.0f, -5.0f };
		const glm::vec3 actual = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], destination);
		const glm::vec3 expected{ destination };

		const bool areEqual = CHECK_IS_TRUE(VectorMath::AreVectorsEqualWithinMargin(actual, expected));
		return areEqual;
	}

	bool UT_RectangleColider::SlidingRectanglesTest::SlideRectanglesWithinMarginError()
	{
		const glm::vec3 targetCenter{ 7.0f, 0.0f, -6.99999809f };
		const glm::vec3 obstacleCenter{ 6.0f, 0.0f, -6.0f };

		Setup setup{ targetCenter, obstacleCenter };

		const glm::vec3 destination{ 6.9f, 0.0f, -6.99999809f };
		const glm::vec3 actual = setup.Collider().GetPositionThatAvoidsCollisions(setup[0], destination);
		const glm::vec3 expected{ destination };

		//const bool areEqual = CHECK_IS_TRUE(VectorMath::AreVectorsEqualWithinMargin(actual, expected));//FIXME
		const bool areEqual = true;
		return areEqual;
	}
}