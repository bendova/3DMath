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
		TriangleAreaIntersectionTest areaTest;
		bool result = areaTest.Run();
		
		return result;
	}


	TriangleAreaIntersectionTest::TriangleAreaIntersectionTest()
	{}

	TriangleAreaIntersectionTest::~TriangleAreaIntersectionTest()
	{}

	bool TriangleAreaIntersectionTest::Run()
	{
		return TriangleOverlapsWithItself()
			&& TriangleIntersectionWithContainedTriangle()
			&& IntersectionIn_NoArea()
			&& IntersectionIn_PointArea()
			&& IntersectionIn_LineArea()
			&& IntersectionIn_TriangleArea()
			&& IntersectionIn_RombusArea()
			&& IntersectionIn_TrapezeArea()
			&& IntersectionIn_PentagonArea()
			&& IntersectionIn_HexagonArea()
			;
	}

	bool TriangleAreaIntersectionTest::TriangleOverlapsWithItself()
	{
		Triangle a{ glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f) };

		const auto& verticesA = a.GetVertices();
		const TriangleIntersection::Area expectedArea{ verticesA.begin(), verticesA.end() };
		const TriangleIntersection::Area area = TriangleIntersection::GetIntersectionArea(a, a);

		return CHECK_EQUALS(area, expectedArea);
	}

	bool TriangleAreaIntersectionTest::TriangleIntersectionWithContainedTriangle()
	{
		Triangle a{ glm::vec4(0.0f, 3.0f, 0.0f, 1.0f), glm::vec4(3.0f, -3.0f, 0.0f, 1.0f), glm::vec4(-3.0f, -3.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f) };

		const auto& verticesB = b.GetVertices();
		const TriangleIntersection::Area expectedArea{ verticesB.begin(), verticesB.end() };
		const TriangleIntersection::Area area = TriangleIntersection::GetIntersectionArea(a, b);

		return CHECK_EQUALS(area, expectedArea);
	}

	bool TriangleAreaIntersectionTest::IntersectionIn_NoArea()
	{
		Triangle a{ glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(0.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f) };

		const TriangleIntersection::Area expectedArea{};
		const TriangleIntersection::Area& area = TriangleIntersection::GetIntersectionArea(a, b);

		return CHECK_EQUALS(area, expectedArea);
	}

	bool TriangleAreaIntersectionTest::IntersectionIn_PointArea()
	{
		Triangle a{ glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f) };

		const TriangleIntersection::Area expectedArea{ glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) };
		const TriangleIntersection::Area& area = TriangleIntersection::GetIntersectionArea(a, b);

		return CHECK_EQUALS(area, expectedArea);
	}

	bool TriangleAreaIntersectionTest::IntersectionIn_LineArea()
	{
		Triangle a{ glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) };

		const TriangleIntersection::Area expectedArea{ glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f) };
		const TriangleIntersection::Area& area = TriangleIntersection::GetIntersectionArea(a, b);

		return CHECK_EQUALS(area, expectedArea);
	}
	
	bool TriangleAreaIntersectionTest::IntersectionIn_TriangleArea()
	{
		Triangle a{ glm::vec4(1.0f, 2.0f, 0.0f, 1.0f), glm::vec4(3.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, -2.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(0.0f, 2.0f, 0.0f, 1.0f), glm::vec4(2.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, -2.0f, 0.0f, 1.0f) };

		const TriangleIntersection::Area expectedArea{ glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(2.0f, 0.0f, 0.0f, 1.0f)};
		const TriangleIntersection::Area area = TriangleIntersection::GetIntersectionArea(a, b);

		return CHECK_EQUALS(area, expectedArea);
	}

	bool TriangleAreaIntersectionTest::IntersectionIn_RombusArea()
	{
		Triangle a{ glm::vec4(0.0f, 2.0f, 0.0f, 1.0f), glm::vec4(2.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, -2.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(3.0f, 3.0f, 0.0f, 1.0f), glm::vec4(3.0f, -3.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) };

		const TriangleIntersection::Area expectedArea{ glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(2.0f, 0.0f, 0.0f, 1.0f), 
			glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) };
		const TriangleIntersection::Area area = TriangleIntersection::GetIntersectionArea(a, b);

		return CHECK_EQUALS(area, expectedArea);
	}

	bool TriangleAreaIntersectionTest::IntersectionIn_TrapezeArea()
	{
		Triangle a{ glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(4.0f, 4.0f, 0.0f, 1.0f), glm::vec4(8.0f, 0.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(2.0f, 1.0f, 0.0f, 1.0f), glm::vec4(6.0f, 1.0f, 0.0f, 1.0f), glm::vec4(4.0f, -1.0f, 0.0f, 1.0f) };

		const TriangleIntersection::Area expectedArea
		{ 
			glm::vec4(5.0f, 0.0f, 0.0f, 1.0f), glm::vec4(3.0f, 0.0f, 0.0f, 1.0f), 
			glm::vec4(2.0f, 1.0f, 0.0f, 1.0f), glm::vec4(6.0f, 1.0f, 0.0f, 1.0f) 
		};
		const TriangleIntersection::Area area = TriangleIntersection::GetIntersectionArea(a, b);

		return CHECK_EQUALS(area, expectedArea);
	}

	bool TriangleAreaIntersectionTest::IntersectionIn_PentagonArea()
	{
		Triangle a{ glm::vec4(-3.0f, 5.0f, 0.0f, 1.0f), glm::vec4(2.0f, 0.0f, 0.0f, 1.0f), glm::vec4(-3.0f, -5.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(1.0f, 3.0f, 0.0f, 1.0f), glm::vec4(1.0f, -3.0f, 0.0f, 1.0f), glm::vec4(-2.0f, 0.0f, 0.0f, 1.0f) };

		const TriangleIntersection::Area expectedArea
		{
			glm::vec4(0.0f, 2.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(0.0f, -2.0f, 0.0f, 1.0f),
			glm::vec4(-2.0f, 0.0f, 0.0f, 1.0f)
		};
		const TriangleIntersection::Area area = TriangleIntersection::GetIntersectionArea(a, b);

		return CHECK_EQUALS(area, expectedArea);
	}

	bool TriangleAreaIntersectionTest::IntersectionIn_HexagonArea()
	{
		Triangle a{ glm::vec4(-3.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 3.0f, 0.0f, 1.0f), glm::vec4(3.0f, 0.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(-3.0f, 2.0f, 0.0f, 1.0f), glm::vec4(3.0f, 2.0f, 0.0f, 1.0f), glm::vec4(0.0f, -1.0f, 0.0f, 1.0f) };

		const TriangleIntersection::Area expectedArea
		{   
			glm::vec4(-2.0f, 1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, 2.0f, 0.0f, 1.0f), 
			glm::vec4(1.0f, 2.0f, 0.0f, 1.0f), glm::vec4( 2.0f, 1.0f, 0.0f, 1.0f), 
			glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f)
		};
		const TriangleIntersection::Area area = TriangleIntersection::GetIntersectionArea(a, b);

		return CHECK_EQUALS(area, expectedArea);
	}
}