#ifndef _MY_CODE_TRIANGLE_INTERSECTION_H_
#define _MY_CODE_TRIANGLE_INTERSECTION_H_

#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include "../framework/MathUtil.h"

namespace MyCode
{
	using namespace MathUtil;
	class TriangleIntersection
	{
	friend class UT_TriangleIntersection;

	public:
		typedef glm::vec4 Point;
		typedef std::vector<Point> Area;
		typedef std::vector<Point> PointsList;
		
		static Area GetIntersectionArea(const Triangle& triangleA, const Triangle& triangleB);
	private:
		static int GetIndexOfNearestVertice(const Triangle& triangle, const Point& p);
		static PointsList GetInsideVertices(const Triangle& triangleA, const Triangle& triangleB);
		static Area GetOverlapArea(const Triangle& triangleA, const Triangle& triangleB);
		static PointsList GetTriangleSidesIntersectionPoints(const Triangle& triangleA, const Triangle& triangleB);
		static PointsList GetIntersectionPoints(const Triangle& triangle, const glm::vec4& segmentStart, const glm::vec4& segmentEnd);
		static Area GetSumOfAreas(const Area& a, const Area& b);

		TriangleIntersection();
		~TriangleIntersection();
	};
}

#endif //_MY_CODE_TRIANGLE_INTERSECTION_H_