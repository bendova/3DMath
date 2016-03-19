#ifndef _MY_CODE_UT_TRIANGLE_INTERSECTION_H_
#define _MY_CODE_UT_TRIANGLE_INTERSECTION_H_

#include "UT_TriangleIntersection.h"

namespace MyCode
{
	class UT_TriangleIntersection
	{
	public:
		UT_TriangleIntersection();
		~UT_TriangleIntersection();

		bool Run();
	};

	class TriangleAreaIntersectionTest
	{
	public:
		TriangleAreaIntersectionTest();
		~TriangleAreaIntersectionTest();

		bool Run();
	private:
		bool TriangleOverlapsWithItself();
		bool IntersectionIn_NoArea();
		bool TriangleIntersectionWithContainedTriangle();
		bool IntersectionIn_PointArea();
		bool IntersectionIn_LineArea();
		bool IntersectionIn_TriangleArea();
		bool IntersectionIn_RombusArea();
		bool IntersectionIn_TrapezeArea();
		bool IntersectionIn_PentagonArea();
		bool IntersectionIn_HexagonArea();
	};

	template<typename T, typename U>
	bool CheckEquals(const T& actual, const U& expected, const char* testName, const char* msg = nullptr)
	{
		bool equals = (actual == expected);
		if (!equals)
		{
			if (msg)
			{
				printf("%s() Failed: %s!\n", testName, msg);
			}
			else
			{
				printf("%s() Failed!\n", testName);
			}
		}

		return equals;
	}
}

#endif //_MY_CODE_UT_TRIANGLE_INTERSECTION_H_