#include "UT_TriangleIntersection.h"
#include "TriangleIntersection.h"

#define CHECK_EQUALS(actual, expected) CheckEquals((actual), (expected), __FUNCTION__)
#define CHECK_EQUALS_MSG(actual, expected, msg) CheckEquals((actual), (expected), __FUNCTION__, (msg))

namespace MyCode
{
	UT_TriangleIntersection::UT_TriangleIntersection()
	{}

	UT_TriangleIntersection::~UT_TriangleIntersection()
	{}

	bool UT_TriangleIntersection::Run()
	{
		bool result = TestTriangleVerticesIntersection()
			&& TestTriangleAreaIntersection();
		
		return result;
	}

	bool UT_TriangleIntersection::TestTriangleVerticesIntersection()
	{
		return TriangleOverlapsWithItselfAtVertices()
			&& TriangleIntersectionIn0Vertices()
			&& TriangleIntersectionIn1Vertice()
			&& TriangleIntersectionIn2Vertices()
			&& TriangleIntersectionIn3Vertices()
			;
	}

	bool UT_TriangleIntersection::TriangleOverlapsWithItselfAtVertices()
	{
		Triangle a{ glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f) };
		const int count = TriangleIntersection::GetInsideVertices(a, a).size();

		return CHECK_EQUALS(count, Triangle::GetPointsCount());
	}

	bool UT_TriangleIntersection::TriangleIntersectionIn0Vertices()
	{
		Triangle a{ glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(-5.0f, 5.0f, 0.0f, 1.0f), glm::vec4(-4.0f, 5.0f, 0.0f, 1.0f), glm::vec4(-5.0f, 4.0f, 0.0f, 1.0f) };

		const int count = TriangleIntersection::GetInsideVertices(a, b).size();

		return CHECK_EQUALS(count, 0);
	}

	bool UT_TriangleIntersection::TriangleIntersectionIn1Vertice()
	{
		Triangle a{ glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -2.0f, 0.0f, 1.0f), glm::vec4(-2.0f, -1.0f, 0.0f, 1.0f) };

		const int count = TriangleIntersection::GetInsideVertices(a, b).size();

		return CHECK_EQUALS(count, 1);
	}

	bool UT_TriangleIntersection::TriangleIntersectionIn2Vertices()
	{
		Triangle a{ glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) };

		const int count = TriangleIntersection::GetInsideVertices(a, b).size();

		return CHECK_EQUALS(count, 2);
	}

	bool UT_TriangleIntersection::TriangleIntersectionIn3Vertices()
	{
		Triangle a{ glm::vec4(3.0f, 3.0f, 0.0f, 1.0f), glm::vec4(3.0f, -3.0f, 0.0f, 1.0f), glm::vec4(-3.0f, -3.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f) };

		const int count = TriangleIntersection::GetInsideVertices(a, b).size();

		return CHECK_EQUALS(count, 3);
	}

	bool UT_TriangleIntersection::TestTriangleAreaIntersection()
	{
		return TriangleOverlapsWithItselfInArea()
			&& TriangleIntersectionIn0Area()
			&& TriangleIntersectionIn1VerticeArea();
	}

	bool UT_TriangleIntersection::TriangleOverlapsWithItselfInArea()
	{
		Triangle a{ glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f) };

		const auto& verticesA = a.GetVertices();
		const TriangleIntersection::Area expectedArea{ verticesA.begin(), verticesA.end() };
		const TriangleIntersection::Area& area = TriangleIntersection::GetIntersectionArea(a, a);

		return CHECK_EQUALS(area, expectedArea);
	}

	bool UT_TriangleIntersection::TriangleNoIntersectionInArea()
	{
		Triangle a{ glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(0.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f) };

		const TriangleIntersection::Area expectedArea{};
		const TriangleIntersection::Area& area = TriangleIntersection::GetIntersectionArea(a, b);

		return CHECK_EQUALS(area, expectedArea);
	}

	bool UT_TriangleIntersection::TriangleIntersectionIn0Area()
	{
		Triangle a{ glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) };

		const TriangleIntersection::Area expectedArea{};
		const TriangleIntersection::Area& area = TriangleIntersection::GetIntersectionArea(a, b);

		return CHECK_EQUALS(area, expectedArea);
	}

	bool UT_TriangleIntersection::TriangleIntersectionIn1VerticeArea()
	{
		Triangle a{ glm::vec4(1.0f, 2.0f, 0.0f, 1.0f), glm::vec4(3.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, -2.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(0.0f, 2.0f, 0.0f, 1.0f), glm::vec4(2.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, -2.0f, 0.0f, 1.0f) };

		const TriangleIntersection::Area expectedArea{glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), 
			glm::vec4(2.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, -1.0f, 0.0f, 1.0f)};
		const TriangleIntersection::Area& area = TriangleIntersection::GetIntersectionArea(a, b);

		return CHECK_EQUALS(area, expectedArea);
	}
}