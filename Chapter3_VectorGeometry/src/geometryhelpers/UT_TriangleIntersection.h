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
		
	private:
		bool TestTriangleVerticesIntersection();
		bool TriangleOverlapsWithItselfAtVertices();
		bool TriangleIntersectionIn1Vertice();
		bool TriangleIntersectionIn2Vertices();
		bool TriangleIntersectionIn3Vertices();
		bool TriangleIntersectionIn0Vertices();

		bool TestTriangleAreaIntersection();
		bool TriangleOverlapsWithItselfInArea();
		bool TriangleNoIntersectionInArea();
		bool TriangleIntersectionIn0Area();
		bool TriangleIntersectionIn1VerticeArea();

		template<typename T, typename U>
		bool CheckEquals(T actual, U expected, const char* testName, const char* msg = nullptr)
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
	};
}

#endif //_MY_CODE_UT_TRIANGLE_INTERSECTION_H_