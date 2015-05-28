#include "UT_VectorMath.h"
#include "VectorMath.h"
#include "UTUtil.h"

namespace MyCode
{
	using namespace VectorMath;

	bool UT_VectorMath::Validate()
	{
		PointToPlaneProjectionTest pointToPlaneProjection;
		LinesIntersection linesIntersection;
		LineSegmentPolygonIntersection lineSegmentWithPolygonIntersection;
		return pointToPlaneProjection.Run()
			&& linesIntersection.Run()
			&& lineSegmentWithPolygonIntersection.Run();
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

	bool UT_VectorMath::LinesIntersection::Run()
	{
		return NoIntersectionBetweenClosedLineSegments()
			&& NoIntersectionBetweenOpenLineSegments()
			&& IntersectionBetweenClosedLineSegments()
			&& IntersectionBetweenOpenLineSegments()
			&& NoIntersectionBetweenColinearLineSegments()
			&& IntersectionBetweenLineSegmentAndRay()
			&& IntersectionBetweenLineSegmentAndLine()
			&& NoIntersectionBetweenRays()
			&& IntersectionBetweenRays()
			&& NoIntersectionBetweenLines()
			&& IntersectionBetweenLines();
	}

	bool UT_VectorMath::LinesIntersection::NoIntersectionBetweenClosedLineSegments()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f), true, true };
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f), true, true };
		MarginPoint<glm::vec3> c{ glm::vec3(2.0f, 0.0f, 2.0f), true, true };
		MarginPoint<glm::vec3> d{ glm::vec3(2.0f, 0.0f, 2.0f), true, true };
		
		const auto intersection = GetLinesIntersection(a, b, c, d);
		const bool doesIntersectionPointExist = intersection.second;
		const bool intersectionPointExpected = false;

		return CHECK_EQUALS(intersectionPointExpected, doesIntersectionPointExist);
	}
	
	bool UT_VectorMath::LinesIntersection::NoIntersectionBetweenOpenLineSegments()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f), true, true };
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f), true, false };
		MarginPoint<glm::vec3> c{ glm::vec3(1.0f, 0.0f, 0.0f), true, true };
		MarginPoint<glm::vec3> d{ glm::vec3(1.0f, 0.0f, 4.0f), true, true };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const bool doesIntersectionPointExist = intersection.second;
		const bool intersectionPointExpected = false;

		return CHECK_EQUALS(intersectionPointExpected, doesIntersectionPointExist);
	}

	bool UT_VectorMath::LinesIntersection::IntersectionBetweenClosedLineSegments()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f), true, true };
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f), true, true };
		MarginPoint<glm::vec3> c{ glm::vec3(1.0f, 0.0f, 0.0f), true, true };
		MarginPoint<glm::vec3> d{ glm::vec3(1.0f, 0.0f, 4.0f), true, true };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const glm::vec3 intersectionPoint = intersection.first;
		const glm::vec3 expectedIntersectionPoint = b.mPoint;

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersectionPoint, expectedIntersectionPoint));
	}
	bool UT_VectorMath::LinesIntersection::IntersectionBetweenOpenLineSegments()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f,  0.0f), true, false };
		MarginPoint<glm::vec3> b{ glm::vec3(2.0f, 0.0f,  0.0f), true, false };
		MarginPoint<glm::vec3> c{ glm::vec3(1.0f, 0.0f, -2.0f), true, false };
		MarginPoint<glm::vec3> d{ glm::vec3(1.0f, 0.0f,  4.0f), true, false };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const glm::vec3 intersectionPoint = intersection.first;
		const glm::vec3 expectedIntersectionPoint{1.0f, 0.0f, 0.0f};

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersectionPoint, expectedIntersectionPoint));
	}

	bool UT_VectorMath::LinesIntersection::NoIntersectionBetweenColinearLineSegments()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f), true, true };
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f), true, false };
		MarginPoint<glm::vec3> c{ glm::vec3(2.0f, 0.0f, 0.0f), true, true };
		MarginPoint<glm::vec3> d{ glm::vec3(3.0f, 0.0f, 0.0f), true, true };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const bool doesIntersectionPointExist = intersection.second;
		const bool intersectionPointExpected = false;

		return CHECK_EQUALS(intersectionPointExpected, doesIntersectionPointExist);
	}

	bool UT_VectorMath::LinesIntersection::IntersectionBetweenLineSegmentAndRay()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f,  0.0f), true, true };
		MarginPoint<glm::vec3> b{ glm::vec3(2.0f, 0.0f,  0.0f), true, true };
		MarginPoint<glm::vec3> c{ glm::vec3(1.0f, 0.0f, 10.0f), true, true };
		MarginPoint<glm::vec3> d{ glm::vec3(1.0f, 0.0f,  9.0f), false, true };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const glm::vec3 intersectionPoint = intersection.first;
		const glm::vec3 expectedIntersectionPoint{1.0f, 0.0f, 0.0f};

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersectionPoint, expectedIntersectionPoint));
	}
	bool UT_VectorMath::LinesIntersection::IntersectionBetweenLineSegmentAndLine()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f), true, true };
		MarginPoint<glm::vec3> b{ glm::vec3(2.0f, 0.0f, 0.0f), true, true };
		MarginPoint<glm::vec3> c{ glm::vec3(1.0f, 0.0f, -10.0f), false, true };
		MarginPoint<glm::vec3> d{ glm::vec3(1.0f, 0.0f, -9.0f), false, true };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const glm::vec3 intersectionPoint = intersection.first;
		const glm::vec3 expectedIntersectionPoint{ 1.0f, 0.0f, 0.0f };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersectionPoint, expectedIntersectionPoint));
	}
	bool UT_VectorMath::LinesIntersection::NoIntersectionBetweenRays()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(2.0f, 0.0f, 0.0f), true, true };
		MarginPoint<glm::vec3> b{ glm::vec3(0.0f, 0.0f, 0.0f), false, true };
		MarginPoint<glm::vec3> c{ glm::vec3(3.0f, 0.0f, 0.0f), true, true };
		MarginPoint<glm::vec3> d{ glm::vec3(5.0f, 0.0f, 0.0f), false, true };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const bool doesIntersectionPointExist = intersection.second;
		const bool intersectionPointExpected = false;

		return CHECK_EQUALS(intersectionPointExpected, doesIntersectionPointExist);
	}
	bool UT_VectorMath::LinesIntersection::IntersectionBetweenRays()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f), true, true };
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f), false, true };
		MarginPoint<glm::vec3> c{ glm::vec3(5.0f, 0.0f, 10.0f), true, true };
		MarginPoint<glm::vec3> d{ glm::vec3(5.0f, 0.0f, 9.0f), false, true };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const glm::vec3 intersectionPoint = intersection.first;
		const glm::vec3 expectedIntersectionPoint{ 5.0f, 0.0f, 0.0f };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersectionPoint, expectedIntersectionPoint));
	}
	bool UT_VectorMath::LinesIntersection::NoIntersectionBetweenLines()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f), false, true };
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f), false, true };
		MarginPoint<glm::vec3> c{ glm::vec3(0.0f, 0.0f, 1.0f), false, true };
		MarginPoint<glm::vec3> d{ glm::vec3(1.0f, 0.0f, 1.0f), false, true };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const bool doesIntersectionPointExist = intersection.second;
		const bool intersectionPointExpected = false;

		return CHECK_EQUALS(intersectionPointExpected, doesIntersectionPointExist);
	}
	bool UT_VectorMath::LinesIntersection::IntersectionBetweenLines()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f), false, true };
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f), false, true };
		MarginPoint<glm::vec3> c{ glm::vec3(5.0f, 0.0f, 3.0f), false, true };
		MarginPoint<glm::vec3> d{ glm::vec3(5.0f, 0.0f, 5.0f), false, true };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const glm::vec3 intersectionPoint = intersection.first;
		const glm::vec3 expectedIntersectionPoint{ 5.0f, 0.0f, 0.0f };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersectionPoint, expectedIntersectionPoint));
	}

	bool UT_VectorMath::LineSegmentPolygonIntersection::Run()
	{
		return NoIntersection()
			&& SegmentCrossingThroughPolygon()
			&& SegmentIsInPolygon()
			&& SegmentIntersectsSideOfCoplanarPolygon()
			&& SegmentIntersectsSideOfPolygon();
	}

	bool UT_VectorMath::LineSegmentPolygonIntersection::NoIntersection()
	{
		const glm::vec3 a{ 1.0f, 0.0f, -1.0f };
		const glm::vec3 b{ 1.0f, 0.0f, 9.0f };
		const std::vector<glm::vec3> obstacle{ glm::vec3{ 4.0f, -1.0f, 0.0f }, glm::vec3{ 6.0f, -1.0f, 0.0f },
			glm::vec3{ 6.0f, 1.0f, 0.0f }, glm::vec3{ 4.0f, 1.0f, 0.0f } };

		const auto intersection = GetIntersectionBetweenLineAndPolygon(a, b, obstacle);
		const bool expected = false;

		return CHECK_EQUALS(intersection.second, expected);
	}

	bool UT_VectorMath::LineSegmentPolygonIntersection::SegmentCrossingThroughPolygon()
	{
		const glm::vec3 a{ 0.0f, 0.0f, -5.0f };
		const glm::vec3 b{ 0.0f, 0.0f,  5.0f };
		const std::vector<glm::vec3> polygonInXY{ 
			glm::vec3{ -1.0f, -1.0f, 0.0f }, glm::vec3{ 1.0f, -1.0f, 0.0f },
			glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec3{ -1.0f, 1.0f, 0.0f } };

		const auto intersection = GetIntersectionBetweenLineAndPolygon(a, b, polygonInXY);
		const glm::vec3 expected{ 0.0f, 0.0f, 0.0f };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersection.first, expected));
	}

	bool UT_VectorMath::LineSegmentPolygonIntersection::SegmentIsInPolygon()
	{
		const glm::vec3 a{ -1.0f, 0.0f, 0.0f };
		const glm::vec3 b{ 1.0f, 0.0f, 0.0f };
		const std::vector<glm::vec3> polygonInXY{
			glm::vec3{ -2.0f, -1.0f, 0.0f }, glm::vec3{ 2.0f, -1.0f, 0.0f },
			glm::vec3{ 2.0f, 1.0f, 0.0f }, glm::vec3{ -2.0f, 1.0f, 0.0f } };

		const auto intersection = GetIntersectionBetweenLineAndPolygon(a, b, polygonInXY);
		const glm::vec3 expected{ a };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersection.first, expected));
	}
	bool UT_VectorMath::LineSegmentPolygonIntersection::SegmentIntersectsSideOfCoplanarPolygon()
	{
		const glm::vec3 a{ -5.0f, 0.0f, 0.0f };
		const glm::vec3 b{ 1.0f, 0.0f, 0.0f };
		const std::vector<glm::vec3> polygonInXY{
			glm::vec3{ -2.0f, -1.0f, 0.0f }, glm::vec3{ 2.0f, -1.0f, 0.0f },
			glm::vec3{ 2.0f, 1.0f, 0.0f }, glm::vec3{ -2.0f, 1.0f, 0.0f } };

		const auto intersection = GetIntersectionBetweenLineAndPolygon(a, b, polygonInXY);
		const glm::vec3 expected{-2.0f, 0.0f, 0.0f};

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersection.first, expected));
	}

	bool UT_VectorMath::LineSegmentPolygonIntersection::SegmentIntersectsSideOfPolygon()
	{
		const glm::vec3 a{ 2.0f, 0.0f, -10.0f };
		const glm::vec3 b{ 2.0f, 0.0f, 10.0f };
		const std::vector<glm::vec3> polygonInXY{
			glm::vec3{ -2.0f, -1.0f, 0.0f }, glm::vec3{ 2.0f, -1.0f, 0.0f },
			glm::vec3{ 2.0f, 1.0f, 0.0f }, glm::vec3{ -2.0f, 1.0f, 0.0f } };

		const auto intersection = GetIntersectionBetweenLineAndPolygon(a, b, polygonInXY);
		const glm::vec3 expected{2.0f, 0.0f, 0.0f};

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersection.first, expected));
	}
}