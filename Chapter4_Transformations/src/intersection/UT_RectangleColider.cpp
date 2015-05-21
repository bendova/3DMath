#include "UT_RectangleColider.h"
#include "../framework/UT_Util.h"
#include "../framework/MathUtil.h"

namespace MyCode
{
	using namespace PolygonCollision;

	bool UT_RectangleColider::Validate()
	{
		RectangleCollisionTest collisionTest;
		BoundingPathTest boundingPathTest;
		ProjectionToAxesTest projectionToAxesTest;
		ValidPositionTest validPositionTest;
		MultipleObjectsAvoidanceTest objectsAvoidanceTest;
		SteppingOutOfCollisionTest stepOutOfCollisionTest;

		return projectionToAxesTest.Run()
			&& collisionTest.Run()
			&& boundingPathTest.Run()
			&& validPositionTest.Run()
			&& objectsAvoidanceTest.Run()
			&& stepOutOfCollisionTest.Run();
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
			glm::vec3{  3.0f, 0.0f,  1.0f }, glm::vec3{  10.0f, 0.0f, 1.0f },
			glm::vec3{  10.0f, 0.0f, -1.0f }, glm::vec3{  3.0f, 0.0f, -1.0f } };
		
		const bool collision = PolygonIntersection::DoPolygonsIntersect(rectangle1, rectangle2);
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

		const bool collision = PolygonIntersection::DoPolygonsIntersect(rectangle1, rectangle2);
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
		
		const bool collision = PolygonIntersection::DoPolygonsIntersect(rectangle1, rectangle2);
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

		const bool collision = PolygonIntersection::DoPolygonsIntersect(rectangle1, rectangle2);
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

		const bool collision = PolygonIntersection::DoPolygonsIntersect(rectangle1, rectangle2);
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

		const bool collision = PolygonIntersection::DoPolygonsIntersect(rectangle1, rectangle2);
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

		const bool collision = PolygonIntersection::DoPolygonsIntersect(rectangle1, rectangle2);
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

		const bool collision = PolygonIntersection::DoPolygonsIntersect(rectangle1, rectangle2);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
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

	std::pair<glm::vec3, glm::vec3> UT_RectangleColider::ProjectionToAxesTest::ProjectPointsToAxis(const std::vector<glm::vec3>& points, const glm::vec3& lineA, const glm::vec3& lineB)
	{
		const auto projectedFactors = PolygonIntersection::ProjectPolygonToAxis(points, lineA, lineB);
		return GetPointsOnLine(projectedFactors, lineA, lineB);
	}

	std::pair<glm::vec3, glm::vec3> UT_RectangleColider::ProjectionToAxesTest::GetPointsOnLine(std::pair<float, float> factors,
		const glm::vec3& lineA, const glm::vec3& lineB)
	{
		const glm::vec3 lineDirection = lineB - lineA;
		const glm::vec3 pointM = lineA + factors.first * lineDirection;
		const glm::vec3 pointN = lineA + factors.second * lineDirection;
		return std::make_pair(pointM, pointN);
	}

	bool UT_RectangleColider::ValidPositionTest::Run()
	{
		return NearEdgeIntersection()
			&& FarEdgeIntersection()
			&& PathIntersection()
			&& DiagonalPathIntersection()
			&& EdgeIntersection()
			&& CornerIntersection()
			&& CrossIntersection()
			&& CrossIntersection2()
			&& CrossIntersection3()
			&& CrossIntersection4()
			&& CrossIntersection5()
			&& CrossIntersection6()
			&& CrossIntersection7();
	}

	bool UT_RectangleColider::ValidPositionTest::NoCollision()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 1.0f, 0.0f, 0.0f };
		const glm::vec3 validTarget = setup.Collider().GetPositionThatAvoidCollisions(setup[0], targetOfR1);
		
		return CHECK_EQUALS(targetOfR1, validTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::NearEdgeIntersection()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 3.0f, 0.0f, 0.0f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ 2.0f, 0.0f, 0.0f };

		return CHECK_EQUALS(expectedTargetOfR1, returnedTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::FarEdgeIntersection()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 4.5f, 0.0f, 0.0f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ 2.0f, 0.0f, 0.0f };

		return CHECK_EQUALS(expectedTargetOfR1, returnedTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::EdgeIntersection()
	{
		const glm::vec3 centerOfR1{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 centerOfR2{ 1.0f, 0.0f, 0.0f };
		Setup setup{ centerOfR1, centerOfR2 };

		const glm::vec3 targetOfR1{ -1.0f, 0.0f, -1.0f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ targetOfR1 };

		return CHECK_EQUALS(expectedTargetOfR1, returnedTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::CornerIntersection()
	{
		const glm::vec3 centerOfR1{ 2.3f, 0.0f, 1.0f };
		const glm::vec3 centerOfR2{ 1.0f, 0.0f, 0.0f };
		Setup setup{ centerOfR1, centerOfR2 };

		const glm::vec3 targetOfR1{ 2.3f, 0.0f, 0.9f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ targetOfR1 };

		return CHECK_EQUALS(expectedTargetOfR1, returnedTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::PathIntersection()
	{
		Setup setup{ glm::vec3{ -1.0f, 0.0f, -4.0f }, glm::vec3{ -1.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ -1.0f, 0.0f, 0.0f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ -1.0f, 0.0f, -1.0f };

		return CHECK_EQUALS(expectedTargetOfR1, returnedTarget);
	}

	bool UT_RectangleColider::ValidPositionTest::DiagonalPathIntersection()
	{
		Setup setup{ glm::vec3{ -3.0f, 0.0f, -3.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ -1.0f, 0.0f, -1.0f };

		const double errorMargin = 1e-6;
		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedTarget, expectedTargetOfR1, errorMargin));
	}

	bool UT_RectangleColider::ValidPositionTest::CrossIntersection()
	{
		Setup setup{ glm::vec3{ -1.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 1.0f, 0.0f, 0.0f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ -1.0f, 0.0f, -1.0f };

		return CHECK_EQUALS(returnedTarget, expectedTargetOfR1);
	}

	bool UT_RectangleColider::ValidPositionTest::CrossIntersection2()
	{
		Setup setup{ glm::vec3{ -0.8f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 0.2f, 0.0f, -0.45f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ setup[0].Center() };

		return CHECK_EQUALS(returnedTarget, expectedTargetOfR1);
	}

	bool UT_RectangleColider::ValidPositionTest::CrossIntersection3()
	{
		Setup setup{ glm::vec3{ -1.0f, 0.0f, -0.5f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ -0.5f, 0.0f, 0.5f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ setup[0].Center() };

		return CHECK_EQUALS(returnedTarget, expectedTargetOfR1);
	}

	bool UT_RectangleColider::ValidPositionTest::CrossIntersection4()
	{
		Setup setup{ glm::vec3{ -1.71264f, 0.0f, -1.6331f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ -0.947925f, 0.0f, -0.924394f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidCollisions(setup[0], targetOfR1);
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
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ targetOfR1 };

		const double errorMargin = 1e-6;
		const bool areEqual = CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedTarget, expectedTargetOfR1, errorMargin));
		return areEqual;
	}

	bool UT_RectangleColider::ValidPositionTest::CrossIntersection6()
	{
		Setup setup{ glm::vec3{ -1.0f, 0.0f, -0.2f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ -0.9f, 0.0f, -0.2f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidCollisions(setup[0], targetOfR1);
		const glm::vec3 expectedTargetOfR1{ setup[0].Center() };

		const double errorMargin = 1e-6;
		const bool areEqual = CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedTarget, expectedTargetOfR1, errorMargin));
		return areEqual;
	}

	bool UT_RectangleColider::ValidPositionTest::CrossIntersection7()
	{
		Setup setup{ glm::vec3{ 0.0f, 0.0f, 0.999999940f }, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const glm::vec3 targetOfR1{ 0.0f, 0.0f, 0.899999917f };
		const glm::vec3 returnedTarget = setup.Collider().GetPositionThatAvoidCollisions(setup[0], targetOfR1);
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
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ 2.0f, 0.0f, 0.0f };

		return CHECK_EQUALS(returnedDestination, expectedDestination);
	}

	bool UT_RectangleColider::SteppingOutOfCollisionTest::Run()
	{
		return StepOutOfCollision()
			&& StepOutOfCollision2()
			&& StepOutOfCollision3()
			&& StepOutOfCollision4();
	}

	bool UT_RectangleColider::SteppingOutOfCollisionTest::StepOutOfCollision4()
	{
		const glm::vec3 targetCenter{ 0.0f, 0.0f, 0.3f };
		const glm::vec3 nearObstacleCenter{ 0.5f, 0.0f, 0.0f };

		Setup setup{ targetCenter, nearObstacleCenter };

		const glm::vec3 destination{ -6.0f, 0.0f, 0.0f };
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ destination };

		return CHECK_EQUALS(returnedDestination, expectedDestination);
	}

	bool UT_RectangleColider::SteppingOutOfCollisionTest::StepOutOfCollision()
	{
		Setup setup{ glm::vec3{ -2.5f, 0.0f, 2.5f }, glm::vec3{ -1.52f, 0.0f, 2.5f } };

		const glm::vec3 destination{ -2.52f, 0.0f, 2.413f };
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidCollisions(setup[0], destination);
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
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidCollisions(setup[0], destination);
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
		const glm::vec3 returnedDestination = setup.Collider().GetPositionThatAvoidCollisions(setup[0], destination);
		const glm::vec3 expectedDestination{ -0.5f, 1.0f, 0.325f };

		const double errorMargin = 1e-6;
		const bool areEqual = CHECK_IS_TRUE(AreVectorsEqualWithinMargin(returnedDestination, expectedDestination, errorMargin));
		return areEqual;
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