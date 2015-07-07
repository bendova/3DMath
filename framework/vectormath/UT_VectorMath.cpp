#include "UT_VectorMath.h"
#include "UTUtil.h"
#include "vectormath/Common.h"
#include "vectormath/Intersection.h"
#include "vectormath/Projection.h"

namespace MyCode
{
	using namespace VectorMath;

	bool UT_VectorMath::Validate()
	{
		PointToPlaneProjectionTest pointToPlaneProjection;
		LinesIntersection linesIntersection;
		ColinearLineSegmentsIntersection collinearLinesIntersection;
		LineSegmentPolygonIntersection lineSegmentWithPolygonIntersection;
		CoplanarityTest coplanarityTest;

		return pointToPlaneProjection.Run()
			&& linesIntersection.Run()
			&& collinearLinesIntersection.Run()
			&& lineSegmentWithPolygonIntersection.Run()
			&& coplanarityTest.Run();
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
		const VectorMath::Plane plane{ pointInPlane, planeNormal };

		const glm::vec3 projectedPoint = ProjectPointOnPlane(pointToProject, plane);
		const glm::vec3 expectedPoint{ 1.0f, 1.0f, 0.0f };

		return CHECK_EQUALS(projectedPoint, expectedPoint);
	}

	bool UT_VectorMath::PointToPlaneProjectionTest::PointToPlaneProjection1()
	{
		const glm::vec3 pointToProject{ -1.0f, -1.0f, 0.0f };
		const glm::vec3 pointInPlane{ -1.0f, 0.0f, 1.0f };
		const glm::vec3 planeNormal{ 0.0f, 4.0f, 0.0f };
		const VectorMath::Plane plane{ pointInPlane, planeNormal };

		const glm::vec3 projectedPoint = ProjectPointOnPlane(pointToProject, plane);
		const glm::vec3 expectedPoint{ -1.0f, 0.0f, 0.0f };

		return CHECK_EQUALS(projectedPoint, expectedPoint);
	}

	bool UT_VectorMath::LinesIntersection::Run()
	{
		return NoIntersectionBetweenClosedLineSegments()
			&& NoIntersectionBetweenOpenLineSegments()
			&& IntersectionBetweenClosedLineSegments()
			&& IntersectionBetweenOpenLineSegments()
			&& IntersectionBetweenLineSegmentAndRay()
			&& IntersectionBetweenLineSegmentAndLine()
			&& NoIntersectionBetweenRays()
			&& IntersectionBetweenRays()
			&& NoIntersectionBetweenLines()
			&& IntersectionBetweenLines();
	}

	bool UT_VectorMath::LinesIntersection::NoIntersectionBetweenClosedLineSegments()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f)};
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f)};
		MarginPoint<glm::vec3> c{ glm::vec3(2.0f, 0.0f, 2.0f)};
		MarginPoint<glm::vec3> d{ glm::vec3(2.0f, 0.0f, 2.0f)};
		
		const auto intersection = GetLinesIntersection(a, b, c, d);
		const bool doesIntersectionPointExist = intersection.second;
		const bool intersectionPointExpected = false;

		return CHECK_EQUALS(intersectionPointExpected, doesIntersectionPointExist);
	}
	
	bool UT_VectorMath::LinesIntersection::NoIntersectionBetweenOpenLineSegments()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f)};
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f), BoundingPointType::BOUNDED, 
			PointType::OPEN_ENDED };
		MarginPoint<glm::vec3> c{ glm::vec3(1.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> d{ glm::vec3(1.0f, 0.0f, 4.0f) };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const bool doesIntersectionPointExist = intersection.second;
		const bool intersectionPointExpected = false;

		return CHECK_EQUALS(intersectionPointExpected, doesIntersectionPointExist);
	}

	bool UT_VectorMath::LinesIntersection::IntersectionBetweenClosedLineSegments()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f)};
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f)};
		MarginPoint<glm::vec3> c{ glm::vec3(1.0f, 0.0f, 0.0f)};
		MarginPoint<glm::vec3> d{ glm::vec3(1.0f, 0.0f, 4.0f)};

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const glm::vec3 intersectionPoint = intersection.first;
		const glm::vec3 expectedIntersectionPoint = b.mPoint;

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersectionPoint, expectedIntersectionPoint));
	}
	bool UT_VectorMath::LinesIntersection::IntersectionBetweenOpenLineSegments()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f,  0.0f), BoundingPointType::BOUNDED, PointType::OPEN_ENDED };
		MarginPoint<glm::vec3> b{ glm::vec3(2.0f, 0.0f,  0.0f), BoundingPointType::BOUNDED, PointType::OPEN_ENDED };
		MarginPoint<glm::vec3> c{ glm::vec3(1.0f, 0.0f, -2.0f), BoundingPointType::BOUNDED, PointType::OPEN_ENDED };
		MarginPoint<glm::vec3> d{ glm::vec3(1.0f, 0.0f,  4.0f), BoundingPointType::BOUNDED, PointType::OPEN_ENDED };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const glm::vec3 intersectionPoint = intersection.first;
		const glm::vec3 expectedIntersectionPoint{1.0f, 0.0f, 0.0f};

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersectionPoint, expectedIntersectionPoint));
	}

	bool UT_VectorMath::LinesIntersection::IntersectionBetweenLineSegmentAndRay()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f,  0.0f)};
		MarginPoint<glm::vec3> b{ glm::vec3(2.0f, 0.0f,  0.0f)};
		MarginPoint<glm::vec3> c{ glm::vec3(1.0f, 0.0f, 10.0f)};
		MarginPoint<glm::vec3> d{ glm::vec3(1.0f, 0.0f, 9.0f), BoundingPointType::UNBOUNDED };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const glm::vec3 intersectionPoint = intersection.first;
		const glm::vec3 expectedIntersectionPoint{1.0f, 0.0f, 0.0f};

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersectionPoint, expectedIntersectionPoint));
	}
	bool UT_VectorMath::LinesIntersection::IntersectionBetweenLineSegmentAndLine()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f)};
		MarginPoint<glm::vec3> b{ glm::vec3(2.0f, 0.0f, 0.0f)};
		MarginPoint<glm::vec3> c{ glm::vec3(1.0f, 0.0f, -10.0f), BoundingPointType::UNBOUNDED };
		MarginPoint<glm::vec3> d{ glm::vec3(1.0f, 0.0f, -9.0f), BoundingPointType::UNBOUNDED };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const glm::vec3 intersectionPoint = intersection.first;
		const glm::vec3 expectedIntersectionPoint{ 1.0f, 0.0f, 0.0f };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersectionPoint, expectedIntersectionPoint));
	}
	bool UT_VectorMath::LinesIntersection::NoIntersectionBetweenRays()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(2.0f, 0.0f, 0.0f)};
		MarginPoint<glm::vec3> b{ glm::vec3(0.0f, 0.0f, 0.0f), BoundingPointType::UNBOUNDED };
		MarginPoint<glm::vec3> c{ glm::vec3(3.0f, 0.0f, 0.0f)};
		MarginPoint<glm::vec3> d{ glm::vec3(5.0f, 0.0f, 0.0f), BoundingPointType::UNBOUNDED };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const bool doesIntersectionPointExist = intersection.second;
		const bool intersectionPointExpected = false;

		return CHECK_EQUALS(intersectionPointExpected, doesIntersectionPointExist);
	}
	bool UT_VectorMath::LinesIntersection::IntersectionBetweenRays()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f)};
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f), BoundingPointType::UNBOUNDED };
		MarginPoint<glm::vec3> c{ glm::vec3(5.0f, 0.0f, 10.0f) };
		MarginPoint<glm::vec3> d{ glm::vec3(5.0f, 0.0f, 9.0f), BoundingPointType::UNBOUNDED };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const glm::vec3 intersectionPoint = intersection.first;
		const glm::vec3 expectedIntersectionPoint{ 5.0f, 0.0f, 0.0f };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersectionPoint, expectedIntersectionPoint));
	}
	bool UT_VectorMath::LinesIntersection::NoIntersectionBetweenLines()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f), BoundingPointType::UNBOUNDED };
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f), BoundingPointType::UNBOUNDED };
		MarginPoint<glm::vec3> c{ glm::vec3(0.0f, 0.0f, 1.0f), BoundingPointType::UNBOUNDED };
		MarginPoint<glm::vec3> d{ glm::vec3(1.0f, 0.0f, 1.0f), BoundingPointType::UNBOUNDED };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const bool doesIntersectionPointExist = intersection.second;
		const bool intersectionPointExpected = false;

		return CHECK_EQUALS(intersectionPointExpected, doesIntersectionPointExist);
	}
	bool UT_VectorMath::LinesIntersection::IntersectionBetweenLines()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f), BoundingPointType::UNBOUNDED };
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f), BoundingPointType::UNBOUNDED };
		MarginPoint<glm::vec3> c{ glm::vec3(5.0f, 0.0f, 3.0f), BoundingPointType::UNBOUNDED };
		MarginPoint<glm::vec3> d{ glm::vec3(5.0f, 0.0f, 5.0f), BoundingPointType::UNBOUNDED };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const glm::vec3 intersectionPoint = intersection.first;
		const glm::vec3 expectedIntersectionPoint{ 5.0f, 0.0f, 0.0f };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersectionPoint, expectedIntersectionPoint));
	}

	bool UT_VectorMath::ColinearLineSegmentsIntersection::Run()
	{
		return NoIntersection()
			&& IntersectionInA()
			&& IntersectionInB()
			&& IntersectionInC()
			&& IntersectionInD()
			&& IntersectionWithMarginError()
			&& IntersectionOpenEnded()
			&& IntersectionOpenEndedOverlapped()
			&& IntersectionOpenEndedOverlapped2();
	}

	bool UT_VectorMath::ColinearLineSegmentsIntersection::NoIntersection()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> c{ glm::vec3(2.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> d{ glm::vec3(3.0f, 0.0f, 0.0f) };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const bool doesIntersectionPointExist = intersection.second;
		const bool intersectionPointExpected = false;

		return CHECK_EQUALS(intersectionPointExpected, doesIntersectionPointExist);
	}

	bool UT_VectorMath::ColinearLineSegmentsIntersection::IntersectionInA()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(1.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> b{ glm::vec3(2.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> c{ glm::vec3(0.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> d{ glm::vec3(3.0f, 0.0f, 0.0f) };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const std::pair<glm::vec3, bool> expected{ a.mPoint, true };

		return CHECK_EQUALS(intersection, expected);
	}

	bool UT_VectorMath::ColinearLineSegmentsIntersection::IntersectionInB()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(4.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> b{ glm::vec3(2.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> c{ glm::vec3(3.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> d{ glm::vec3(0.0f, 0.0f, 0.0f) };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const std::pair<glm::vec3, bool> expected{ c.mPoint, true };

		return CHECK_EQUALS(intersection, expected);
	}

	bool UT_VectorMath::ColinearLineSegmentsIntersection::IntersectionInC()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> b{ glm::vec3(3.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> c{ glm::vec3(1.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> d{ glm::vec3(2.0f, 0.0f, 0.0f) };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const std::pair<glm::vec3, bool> expected{ c.mPoint, true };

		return CHECK_EQUALS(intersection, expected);
	}

	bool UT_VectorMath::ColinearLineSegmentsIntersection::IntersectionInD()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(3.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> c{ glm::vec3(2.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> d{ glm::vec3(0.0f, 0.0f, 0.0f) };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const std::pair<glm::vec3, bool> expected{ c.mPoint, true };

		return CHECK_EQUALS(intersection, expected);
	}

	bool UT_VectorMath::ColinearLineSegmentsIntersection::IntersectionWithMarginError()
	{
		MarginPoint<glm::vec3> a{ glm::vec3(0.0f, 0.0f, 1.0f) };
		MarginPoint<glm::vec3> b{ glm::vec3(4.0f, 0.0f, -3.0f) };
		MarginPoint<glm::vec3> c{ glm::vec3(1.0f, 0.0f, 0.0f) };
		MarginPoint<glm::vec3> d{ glm::vec3(3.0f, 0.0f, -2.0f) };

		const auto intersection = GetLinesIntersection(a, b, c, d);
		const std::pair<glm::vec3, bool> expected{ c.mPoint, true };

		return CHECK_EQUALS(intersection, expected);
	}

	bool UT_VectorMath::ColinearLineSegmentsIntersection::IntersectionOpenEnded()
	{
		const BoundingPointType boundingType = BoundingPointType::BOUNDED;
		const PointType pointType = PointType::OPEN_ENDED;
		MarginPoint<glm::vec3> a{ glm::vec3(-1.0f, 0.0f, 0.0f), boundingType, pointType };
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f), boundingType, pointType };
		MarginPoint<glm::vec3> c{ glm::vec3(-0.5f, 0.0f, 0.0f), boundingType, pointType };
		MarginPoint<glm::vec3> d{ glm::vec3(1.5f, 0.0f, 0.0f), boundingType, pointType };

		const auto intersection = DoCollinearLineSegmentsIntersect(a, b, c, d);
		const bool expected{ true };

		return CHECK_EQUALS(intersection, expected);
	}
	
	bool UT_VectorMath::ColinearLineSegmentsIntersection::IntersectionOpenEndedOverlapped()
	{
		const BoundingPointType boundingType = BoundingPointType::BOUNDED;
		const PointType pointType = PointType::OPEN_ENDED;
		MarginPoint<glm::vec3> a{ glm::vec3(-1.0f, 0.0f, 0.0f), boundingType, pointType };
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f), boundingType, pointType };

		const auto intersection = DoCollinearLineSegmentsIntersect(a, b, a, b);
		const bool expected{ true };

		return CHECK_EQUALS(intersection, expected);
	}

	bool UT_VectorMath::ColinearLineSegmentsIntersection::IntersectionOpenEndedOverlapped2()
	{
		const BoundingPointType boundingType = BoundingPointType::BOUNDED;
		const PointType pointType = PointType::OPEN_ENDED;
		MarginPoint<glm::vec3> a{ glm::vec3(-1.0f, 0.0f, 0.0f), boundingType, pointType };
		MarginPoint<glm::vec3> b{ glm::vec3(1.0f, 0.0f, 0.0f), boundingType, pointType };

		const auto intersection = DoCollinearLineSegmentsIntersect(a, b, b, a);
		const bool expected{ true };

		return CHECK_EQUALS(intersection, expected);
	}

	bool UT_VectorMath::LineSegmentPolygonIntersection::Run()
	{
		return NoIntersection()
			&& SegmentCrossingThroughPolygon()
			&& SegmentIsInPolygon()
			&& SegmentIntersectsSideOfCoplanarPolygon()
			&& SegmentIntersectsSideOfPolygon()
			&& NoIntersectionForTouchingPolygon();
	}

	bool UT_VectorMath::LineSegmentPolygonIntersection::NoIntersection()
	{
		const MarginPoint<glm::vec3> a{ glm::vec3{ 1.0f, 0.0f, -1.0f } };
		const MarginPoint<glm::vec3> b{ glm::vec3{ 1.0f, 0.0f, 9.0f } };
		const std::vector<glm::vec3> obstacle{ glm::vec3{ 4.0f, -1.0f, 0.0f }, glm::vec3{ 6.0f, -1.0f, 0.0f },
			glm::vec3{ 6.0f, 1.0f, 0.0f }, glm::vec3{ 4.0f, 1.0f, 0.0f } };

		const auto intersection = GetIntersectionBetweenLineAndPolygon(a, b, obstacle);
		const bool expected = false;

		return CHECK_EQUALS(intersection.second, expected);
	}

	bool UT_VectorMath::LineSegmentPolygonIntersection::SegmentCrossingThroughPolygon()
	{
		const MarginPoint<glm::vec3> a{ glm::vec3{ 0.0f, 0.0f, -5.0f }};
		const MarginPoint<glm::vec3> b{ glm::vec3{ 0.0f, 0.0f, 5.0f }};
		const std::vector<glm::vec3> polygonInXY{ 
			glm::vec3{ -1.0f, -1.0f, 0.0f }, glm::vec3{ 1.0f, -1.0f, 0.0f },
			glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec3{ -1.0f, 1.0f, 0.0f } };

		const auto intersection = GetIntersectionBetweenLineAndPolygon(a, b, polygonInXY);
		const glm::vec3 expected{ 0.0f, 0.0f, 0.0f };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersection.first, expected));
	}

	bool UT_VectorMath::LineSegmentPolygonIntersection::SegmentIsInPolygon()
	{
		const MarginPoint<glm::vec3> a{ glm::vec3{ -1.0f, 0.0f, 0.0f }};
		const MarginPoint<glm::vec3> b{ glm::vec3{ 1.0f, 0.0f, 0.0f }};
		const std::vector<glm::vec3> polygonInXY{
			glm::vec3{ -2.0f, -1.0f, 0.0f }, glm::vec3{ 2.0f, -1.0f, 0.0f },
			glm::vec3{ 2.0f, 1.0f, 0.0f }, glm::vec3{ -2.0f, 1.0f, 0.0f } };

		const auto intersection = GetIntersectionBetweenLineAndPolygon(a, b, polygonInXY);
		const glm::vec3 expected{ a.mPoint };

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersection.first, expected));
	}
	bool UT_VectorMath::LineSegmentPolygonIntersection::SegmentIntersectsSideOfCoplanarPolygon()
	{
		const MarginPoint<glm::vec3> a{ glm::vec3{ -5.0f, 0.0f, 0.0f }};
		const MarginPoint<glm::vec3> b{ glm::vec3{ 1.0f, 0.0f, 0.0f }};
		const std::vector<glm::vec3> polygonInXY{
			glm::vec3{ -2.0f, -1.0f, 0.0f }, glm::vec3{ 2.0f, -1.0f, 0.0f },
			glm::vec3{ 2.0f, 1.0f, 0.0f }, glm::vec3{ -2.0f, 1.0f, 0.0f } };

		const auto intersection = GetIntersectionBetweenLineAndPolygon(a, b, polygonInXY);
		const glm::vec3 expected{-2.0f, 0.0f, 0.0f};

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersection.first, expected));
	}

	bool UT_VectorMath::LineSegmentPolygonIntersection::SegmentIntersectsSideOfPolygon()
	{
		const MarginPoint<glm::vec3> a{ glm::vec3{ 2.0f, 0.0f, -10.0f }};
		const MarginPoint<glm::vec3> b{ glm::vec3{ 2.0f, 0.0f, 10.0f }};
		const std::vector<glm::vec3> polygonInXY{
			glm::vec3{ -2.0f, -1.0f, 0.0f }, glm::vec3{ 2.0f, -1.0f, 0.0f },
			glm::vec3{ 2.0f, 1.0f, 0.0f }, glm::vec3{ -2.0f, 1.0f, 0.0f } };

		const auto intersection = GetIntersectionBetweenLineAndPolygon(a, b, polygonInXY);
		const glm::vec3 expected{2.0f, 0.0f, 0.0f};

		return CHECK_IS_TRUE(AreVectorsEqualWithinMargin(intersection.first, expected));
	}

	bool UT_VectorMath::LineSegmentPolygonIntersection::NoIntersectionForTouchingPolygon()
	{
		const MarginPoint<glm::vec3> a{ glm::vec3{ 1.0f, -1.0f, 1.0f }, BoundingPointType::BOUNDED,
			PointType::OPEN_ENDED };
		const MarginPoint<glm::vec3> b{ glm::vec3{ -9.0f, -1.0f, 1.0f }, BoundingPointType::BOUNDED,
			PointType::OPEN_ENDED };
		const std::vector<glm::vec3> polygon
		{
			glm::vec3{ 1.0f, -1.0f, 1.0f }, glm::vec3{ 3.0f, -1.0f, 1.0f },
			glm::vec3{ 3.0f, 1.0f, 1.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f } 
		};

		const auto intersection = GetIntersectionBetweenLineAndPolygon(a, b, polygon);
		const bool expected = false;

		return CHECK_EQUALS(intersection.second, expected);
	}

	bool UT_VectorMath::CoplanarityTest::Run()
	{
		return PlaneDescription()
			&& PointIsNotInPlane() 
			&& PointIsInPlane();
	}

	bool UT_VectorMath::CoplanarityTest::PlaneDescription()
	{
		const std::vector<glm::vec3> polygonXZ
		{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, 1.0f }
		};
		const Plane plane = VectorMath::GetPolygonPlane(polygonXZ);
		const Plane expected
		{
			glm::vec3{ -1.0f, 0.0f, 1.0f },
			glm::vec3{ 0.0f, 4.0f, 0.0f }
		};

		return CHECK_EQUALS(plane, expected);
	}

	bool UT_VectorMath::CoplanarityTest::PointIsNotInPlane()
	{
		const glm::vec3 point{1.0f, 0.0f, 0.0f};
		const std::vector<glm::vec3> polygon
		{
			glm::vec3{ -1.0f, -1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }
		};

		const bool isInPlane = VectorMath::IsPointCoplanarWithPolygon(point, polygon);
		const bool expected = false;

		return CHECK_EQUALS(isInPlane, expected);
	}

	bool UT_VectorMath::CoplanarityTest::PointIsInPlane()
	{
		const glm::vec3 point{ 0.0f, 0.0f, 0.0f };
		const std::vector<glm::vec3> polygon
		{
			glm::vec3{ -1.0f, -1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }
		};

		const bool isInPlane = VectorMath::IsPointCoplanarWithPolygon(point, polygon);
		const bool expected = true;

		return CHECK_EQUALS(isInPlane, expected);
	}
}