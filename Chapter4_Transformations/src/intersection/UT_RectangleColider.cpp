#include "UT_RectangleColider.h"
#include "../framework/UT_Util.h"

namespace MyCode
{
	bool UT_RectangleColider::Validate()
	{
		RectangleCollisionTest collisionTest;
		ForwardPointsTest forwardPointsTest;
		BoundingPathTest boundingPathTest;
		ProjectionToAxesTest projectionToAxesTest;
		ValidPositionTest validPositionTest;

		return projectionToAxesTest.Run()
			&& collisionTest.Run()
			&& forwardPointsTest.Run()
			&& boundingPathTest.Run()
			&& validPositionTest.Run();
	}

	bool UT_RectangleColider::RectangleCollisionTest::Run()
	{
		return NoCollision()
			&& TouchingInOnePoint()
			&& TouchingOnOneSide()
			&& CollisionInOnePoint()
			&& CollisionOnOneSide()
			&& CompleteOverlap()
			&& NoCollisionOfRotatedRectangles()
			&& CollisionOfRotatedRectangles();
	}

	bool UT_RectangleColider::RectangleCollisionTest::NoCollision()
	{
		const std::vector<glm::vec3> rectangle1{ 
			glm::vec3{ -1.0f, 0.0f,  1.0f }, glm::vec3{  1.0f, 0.0f,  1.0f },
			glm::vec3{  1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{  3.0f, 0.0f,  1.0f }, glm::vec3{  5.0f, 0.0f, 1.0f },
			glm::vec3{  5.0f, 0.0f, -1.0f }, glm::vec3{  3.0f, 0.0f, -1.0f } };
		
		const bool collision = PolygonCollision::DoPolygonsIntersect(rectangle1, rectangle2);
		const bool collisionExpected = false;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_RectangleColider::RectangleCollisionTest::TouchingInOnePoint()
	{
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ 1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 2.0f },
			glm::vec3{ 2.0f, 0.0f, 2.0f }, glm::vec3{ 2.0f, 0.0f, 1.0f } };

		const bool collision = PolygonCollision::DoPolygonsIntersect(rectangle1, rectangle2);
		const bool collisionExpected = false;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_RectangleColider::RectangleCollisionTest::TouchingOnOneSide()
	{
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ -1.0f, 0.0f,  1.0f }, glm::vec3{  1.0f, 0.0f, 1.0f },
			glm::vec3{  1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ 1.0f, 0.0f, 1.0f }, glm::vec3{ 2.0f, 0.0f, 1.0f },
			glm::vec3{ 2.0f, 0.0f, -1.0f }, glm::vec3{ 1.0f, 0.0f, -1.0f } };
		
		const bool collision = PolygonCollision::DoPolygonsIntersect(rectangle1, rectangle2);
		const bool collisionExpected = false;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_RectangleColider::RectangleCollisionTest::CollisionInOnePoint()
	{
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ 0.0f, 0.0f,  0.0f }, glm::vec3{ 2.0f, 0.0f,  0.0f },
			glm::vec3{ 2.0f, 0.0f, -2.0f }, glm::vec3{ 0.0f, 0.0f, -2.0f } };

		const bool collision = PolygonCollision::DoPolygonsIntersect(rectangle1, rectangle2);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_RectangleColider::RectangleCollisionTest::CollisionOnOneSide()
	{
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f } };

		const bool collision = PolygonCollision::DoPolygonsIntersect(rectangle1, rectangle2);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}


	bool UT_RectangleColider::RectangleCollisionTest::CompleteOverlap()
	{
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ -2.0f, 0.0f, 2.0f }, glm::vec3{ 2.0f, 0.0f, 2.0f },
			glm::vec3{ 2.0f, 0.0f, -2.0f }, glm::vec3{ -2.0f, 0.0f, -2.0f } };

		const bool collision = PolygonCollision::DoPolygonsIntersect(rectangle1, rectangle2);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_RectangleColider::RectangleCollisionTest::NoCollisionOfRotatedRectangles()
	{
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 4.0f, 0.0f, -3.0f },
			glm::vec3{ 3.0f, 0.0f, -4.0f }, glm::vec3{ -1.0f, 0.0f, 0.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ 3.0f, 0.0f, 0.0f }, glm::vec3{ 4.0f, 0.0f, 0.0f },
			glm::vec3{ 4.0f, 0.0f, -1.0f }, glm::vec3{ 3.0f, 0.0f, -1.0f } };

		const bool collision = PolygonCollision::DoPolygonsIntersect(rectangle1, rectangle2);
		const bool collisionExpected = false;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_RectangleColider::RectangleCollisionTest::CollisionOfRotatedRectangles()
	{
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 4.0f, 0.0f, -3.0f },
			glm::vec3{ 3.0f, 0.0f, -4.0f }, glm::vec3{ -1.0f, 0.0f, 0.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 4.0f, 0.0f, 0.0f },
			glm::vec3{ 4.0f, 0.0f, -1.0f }, glm::vec3{ 1.0f, 0.0f, -1.0f } };

		const bool collision = PolygonCollision::DoPolygonsIntersect(rectangle1, rectangle2);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_RectangleColider::ForwardPointsTest::Run()
	{
		return ForwardPointsForMovingLeft()
			&& ForwardPointsForMovingRight()
			&& ForwardPointsForMovingForward()
			&& ForwardPointsForMovingBackward();
	}

	bool UT_RectangleColider::ForwardPointsTest::ForwardPointsForMovingLeft()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.5f, 0.0f, 0.0f } };

		const auto& rectangle = setup.mRectangle1;
		const glm::vec3 moveLeft{ -1.0f, 0.0f, 0.0f };
		const auto forwardPoints = PolygonCollision::GetNearestPoints(rectangle, moveLeft);
		const std::pair<glm::vec3, glm::vec3> expectedPoints{ rectangle.A(), rectangle.D() };

		return CHECK_EQUALS(forwardPoints, expectedPoints);
	}

	bool UT_RectangleColider::ForwardPointsTest::ForwardPointsForMovingRight()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.5f, 0.0f, 0.0f } };

		const auto& rectangle = setup.mRectangle1;
		const glm::vec3 moveRight{ 1.0f, 0.0f, 0.0f };
		const auto forwardPoints = PolygonCollision::GetNearestPoints(rectangle, moveRight);
		const std::pair<glm::vec3, glm::vec3> expectedPoints{ rectangle.B(), rectangle.C() };

		return CHECK_EQUALS(forwardPoints, expectedPoints);
	}

	bool UT_RectangleColider::ForwardPointsTest::ForwardPointsForMovingForward()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.5f, 0.0f, 0.0f } };

		const auto& rectangle = setup.mRectangle1;
		const glm::vec3 moveForward{ 0.0f, 0.0f, -1.0f };
		const auto forwardPoints = PolygonCollision::GetNearestPoints(rectangle, moveForward);
		const std::pair<glm::vec3, glm::vec3> expectedPoints{ rectangle.C(), rectangle.D() };

		return CHECK_EQUALS(forwardPoints, expectedPoints);
	}

	bool UT_RectangleColider::ForwardPointsTest::ForwardPointsForMovingBackward()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.5f, 0.0f, 0.0f } };

		const auto& rectangle = setup.mRectangle1;
		const glm::vec3 moveBackward{ 0.0f, 0.0f, 1.0f };
		const auto forwardPoints = PolygonCollision::GetNearestPoints(rectangle, moveBackward);
		const std::pair<glm::vec3, glm::vec3> expectedPoints{ rectangle.A(), rectangle.B() };

		return CHECK_EQUALS(forwardPoints, expectedPoints);
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
		const std::vector<glm::vec3> boundingPath = PolygonCollision::GetTravelPathBounding(setup.mRectangle1, targetOfR1);
		const std::vector<glm::vec3> expectedBoundingPath{ glm::vec3{ -1.5f, 0.0f, 0.5f }, glm::vec3{ 0.5f, 0.0f, 0.5f }, 
			glm::vec3{ 0.5f, 0.0f, -0.5f }, glm::vec3{ -1.5f, 0.0f, -0.5f } };

		return CHECK_EQUALS(boundingPath, expectedBoundingPath);
	}
	bool UT_RectangleColider::BoundingPathTest::MoveRight()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 1.0f, 0.0f, 0.0f };
		const std::vector<glm::vec3> boundingPath = PolygonCollision::GetTravelPathBounding(setup.mRectangle1, targetOfR1);
		const std::vector<glm::vec3> expectedBoundingPath{ glm::vec3{ -0.5f, 0.0f, 0.5f }, glm::vec3{ 1.5f, 0.0f, 0.5f },
			glm::vec3{ 1.5f, 0.0f, -0.5f }, glm::vec3{ -0.5f, 0.0f, -0.5f } };

		return CHECK_EQUALS(boundingPath, expectedBoundingPath);
	}
	bool UT_RectangleColider::BoundingPathTest::MoveForward()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 0.0f, 0.0f, -1.0f };
		const std::vector<glm::vec3> boundingPath = PolygonCollision::GetTravelPathBounding(setup.mRectangle1, targetOfR1);
		const std::vector<glm::vec3> expectedBoundingPath{ glm::vec3{ -0.5f, 0.0f, 0.5f }, glm::vec3{ 0.5f, 0.0f, 0.5f },
			glm::vec3{ 0.5f, 0.0f, -1.5f }, glm::vec3{ -0.5f, 0.0f, -1.5f } };

		return CHECK_EQUALS(boundingPath, expectedBoundingPath);
	}
	bool UT_RectangleColider::BoundingPathTest::MoveBackward()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 0.0f, 0.0f, 1.0f };
		const std::vector<glm::vec3> boundingPath = PolygonCollision::GetTravelPathBounding(setup.mRectangle1, targetOfR1);
		const std::vector<glm::vec3> expectedBoundingPath{ glm::vec3{ -0.5f, 0.0f, 1.5f }, glm::vec3{ 0.5f, 0.0f, 1.5f },
			glm::vec3{ 0.5f, 0.0f, -0.5f }, glm::vec3{ -0.5f, 0.0f, -0.5f } };

		return CHECK_EQUALS(boundingPath, expectedBoundingPath);
	}

	bool UT_RectangleColider::BoundingPathTest::MoveDiagonallyLeftBackward()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ -1.0f, 0.0f, 1.0f };
		
		const std::vector<glm::vec3> boundingPath = PolygonCollision::GetTravelPathBounding(setup.mRectangle1, targetOfR1);
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

		const std::vector<glm::vec3> boundingPath = PolygonCollision::GetTravelPathBounding(setup.mRectangle1, targetOfR1);
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

		const std::vector<glm::vec3> boundingPath = PolygonCollision::GetTravelPathBounding(setup.mRectangle1, targetOfR1);
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

		const std::vector<glm::vec3> boundingPath = PolygonCollision::GetTravelPathBounding(setup.mRectangle1, targetOfR1);
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

		using namespace PolygonCollision;
		const auto projectionToAB = ProjectPolygonToAxis(triangle, a, b);
		const auto projectionToBC = ProjectPolygonToAxis(triangle, b, c);
		const auto projectionToCA = ProjectPolygonToAxis(triangle, c, a);

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

		using namespace PolygonCollision;
		const auto projectionToAB = ProjectPolygonToAxis(triangle, a, b);
		const auto projectionToBC = ProjectPolygonToAxis(triangle, b, c);
		const auto projectionToCA = ProjectPolygonToAxis(triangle, c, a);

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

		using namespace PolygonCollision;
		const auto projectionToAB = ProjectPolygonToAxis(trapeze, a, b);
		const auto projectionToBC = ProjectPolygonToAxis(trapeze, b, c);
		const auto projectionToCD = ProjectPolygonToAxis(trapeze, c, d);
		const auto projectionToDA = ProjectPolygonToAxis(trapeze, d, a);

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

		using namespace PolygonCollision;
		const auto projectionToAB = ProjectPolygonToAxis(pentagon, a, b);
		const auto projectionToBC = ProjectPolygonToAxis(pentagon, b, c);
		const auto projectionToCD = ProjectPolygonToAxis(pentagon, c, d);
		const auto projectionToDE = ProjectPolygonToAxis(pentagon, d, e);
		const auto projectionToEA = ProjectPolygonToAxis(pentagon, e, a);
		
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

	bool UT_RectangleColider::ValidPositionTest::Run()
	{
		return ValidPositionForNearEdgeIntersection()
			&& ValidPositionForFarEdgeIntersection()
			&& ValidPositionForPathIntersection()
			&& ValidPositionForDiagonalPathIntersection()
			&& ValidPositionForEdgeIntersection()
			&& ValidPositionForCornerIntersection()
			&& ValidPositionForCrossIntersection()
			&& ValidPositionForCrossIntersection2();
	}

	bool UT_RectangleColider::ValidPositionTest::ValidPositionForNoCollision()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 1.0f, 0.0f, 0.0f };
		const glm::vec3 validTarget = setup.mColider.GetPositionThatAvoidCollisions(setup.mRectangle1, targetOfR1);
		
		return CHECK_EQUALS(targetOfR1, validTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::ValidPositionForNearEdgeIntersection()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 2.5f, 0.0f, 0.0f };
		const glm::vec3 returnedTarget = setup.mColider.GetPositionThatAvoidCollisions(setup.mRectangle1, targetOfR1);
		const glm::vec3 expectedTargetOfR1{ 2.0f, 0.0f, 0.0f };

		return CHECK_EQUALS(expectedTargetOfR1, returnedTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::ValidPositionForFarEdgeIntersection()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 3.5f, 0.0f, 0.0f };
		const glm::vec3 returnedTarget = setup.mColider.GetPositionThatAvoidCollisions(setup.mRectangle1, targetOfR1);
		const glm::vec3 expectedTargetOfR1{ 2.0f, 0.0f, 0.0f };

		return CHECK_EQUALS(expectedTargetOfR1, returnedTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::ValidPositionForEdgeIntersection()
	{
		const glm::vec3 centerOfR1{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 centerOfR2{ 1.0f, 0.0f, 0.0f };
		Setup setup{ centerOfR1, centerOfR2 };

		const glm::vec3 targetOfR1{ -1.0f, 0.0f, -1.0f };
		const glm::vec3 returnedTarget = setup.mColider.GetPositionThatAvoidCollisions(setup.mRectangle1, targetOfR1);
		const glm::vec3 expectedTargetOfR1{ targetOfR1 };

		return CHECK_EQUALS(expectedTargetOfR1, returnedTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::ValidPositionForCornerIntersection()
	{
		const glm::vec3 centerOfR1{ 2.3f, 0.0f, 1.0f };
		const glm::vec3 centerOfR2{ 1.0f, 0.0f, 0.0f };
		Setup setup{ centerOfR1, centerOfR2 };

		const glm::vec3 targetOfR1{ 2.3f, 0.0f, 0.9f };
		const glm::vec3 returnedTarget = setup.mColider.GetPositionThatAvoidCollisions(setup.mRectangle1, targetOfR1);
		const glm::vec3 expectedTargetOfR1{ targetOfR1 };

		return CHECK_EQUALS(expectedTargetOfR1, returnedTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::ValidPositionForPathIntersection()
	{
		Setup setup{ glm::vec3{ -1.0f, 0.0f, -4.0f }, glm::vec3{ -1.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ -1.0f, 0.0f, 0.0f };
		const glm::vec3 returnedTarget = setup.mColider.GetPositionThatAvoidCollisions(setup.mRectangle1, targetOfR1);
		const glm::vec3 expectedTargetOfR1{ -1.0f, 0.0f, -1.0f };

		return CHECK_EQUALS(expectedTargetOfR1, returnedTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::ValidPositionForDiagonalPathIntersection()
	{
		Setup setup{ glm::vec3{ -3.0f, 0.0f, -3.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 returnedTarget = setup.mColider.GetPositionThatAvoidCollisions(setup.mRectangle1, targetOfR1);
		const glm::vec3 expectedTargetOfR1{ -1.0f, 0.0f, -1.0f };

		const double errorMargin = 1e-6;
		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedTarget, expectedTargetOfR1, errorMargin));
	}

	bool UT_RectangleColider::ValidPositionTest::ValidPositionForCrossIntersection()
	{
		Setup setup{ glm::vec3{ -1.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 1.0f, 0.0f, 0.0f };
		const glm::vec3 returnedTarget = setup.mColider.GetPositionThatAvoidCollisions(setup.mRectangle1, targetOfR1);
		const glm::vec3 expectedTargetOfR1{ -1.0f, 0.0f, -1.0f };

		return CHECK_EQUALS(returnedTarget, expectedTargetOfR1);
	}

	bool UT_RectangleColider::ValidPositionTest::ValidPositionForCrossIntersection2()
	{
		Setup setup{ glm::vec3{ -0.8f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 0.2f, 0.0f, -0.45f };
		const glm::vec3 returnedTarget = setup.mColider.GetPositionThatAvoidCollisions(setup.mRectangle1, targetOfR1);
		const glm::vec3 expectedTargetOfR1{ setup.mRectangle1.Center() };

		return CHECK_EQUALS(returnedTarget, expectedTargetOfR1);
	}

	bool AreSegmentsEqualWithinMargin(const std::pair<glm::vec3, glm::vec3>& ab, const std::pair<glm::vec3, glm::vec3>& cd, 
		const double margin)
	{
		return (AreVectorsEqualWithinMargin(ab.first, cd.first, margin) 
			&& AreVectorsEqualWithinMargin(ab.second, cd.second, margin));
	}

	bool AreVectorsEqualWithinMargin(const glm::vec3& a, const glm::vec3& b, const double margin)
	{
		return ((std::abs(a.x - b.x) <= margin)
			&& (std::abs(a.y - b.y) <= margin)
			&& (std::abs(a.z - b.z) <= margin));
	}
}