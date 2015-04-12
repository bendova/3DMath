#include "TriangleIntersection.h"
#include <float.h>

namespace MyCode
{
	TriangleIntersection::TriangleIntersection()
	{}

	TriangleIntersection::~TriangleIntersection()
	{}

	/*TriangleIntersection::Area 
	TriangleIntersection::GetIntersectionArea(const Triangle& triangleA, const Triangle& triangleB)
	{
		const auto overlapBA = GetOverlapArea(triangleA, triangleB);
		const auto overlapAB = GetOverlapArea(triangleB, triangleA);
		Area area = GetSumOfAreas(overlapAB, overlapBA);
		return area;
	}
*/

	TriangleIntersection::Area
	TriangleIntersection::GetIntersectionArea(const Triangle& triangleA, const Triangle& triangleB)
	{
		Area area{};

		const auto& verticesA = triangleA.GetVertices();
		const int VERTICES_COUNT = Triangle::GetPointsCount();
		for (int i = 0; i < VERTICES_COUNT; ++i)
		{
			const int sideStartIndex = i;
			const int sideEndIndex = (i + 1) % VERTICES_COUNT;
			const auto& sideStartPoint = verticesA[sideStartIndex];
			const auto& sideEndPoint = verticesA[sideEndIndex];
			if (Contains(triangleA, sideStartPoint))
			{
				area.push_back(sideStartPoint);
			}
			const auto sideIntersectionPoints = GetIntersectionPoints(triangleB, sideStartPoint, sideEndPoint);
			area.insert(area.end(), sideIntersectionPoints.begin(), sideIntersectionPoints.end());
		}

		const auto& verticesB = triangleB.GetVertices();
		const int lastIndex = verticesB.size() - 1;
		int index = GetIndexOfNearestVertice(triangleB, verticesB[lastIndex]);
		for (int i = 0; i < VERTICES_COUNT; ++i)
		{
			index = (index + i) % VERTICES_COUNT;
			const auto& sideStartPoint = verticesB[index];
			if (Contains(triangleB, sideStartPoint))
			{
				area.push_back(sideStartPoint);
			}
		}

		return area;
	}

	TriangleIntersection::PointsList
	TriangleIntersection::GetTriangleSidesIntersectionPoints(const Triangle& triangleA, const Triangle& triangleB)
	{
		PointsList intersectionPoints{};

		const auto& verticesA = triangleA.GetVertices();
		const auto& verticesB = triangleB.GetVertices();
		const int VERTICES_COUNT = Triangle::GetPointsCount();
		for (int i = 0; i < VERTICES_COUNT; ++i)
		{
			const int sideStartIndex = i;
			const int sideEndIndex = (i + 1) % VERTICES_COUNT;
			const glm::vec4 sideOfA = verticesA[sideEndIndex] - verticesA[sideStartIndex];

			const auto points = GetIntersectionPoints(triangleB, verticesA[sideStartIndex], verticesA[sideEndIndex]);
			intersectionPoints.insert(intersectionPoints.end(), points.begin(), points.end());
		}

		return intersectionPoints;
	}

	int 
	TriangleIntersection::GetIndexOfNearestVertice(const Triangle& triangle, const Point& p)
	{
		int index = 0;
		float minDistance = FLT_MAX_EXP;
		const auto& vertices = triangle.GetVertices();
		for (unsigned i = 0; i < vertices.size(); ++i)
		{
			const float distance = glm::distance(vertices[i], p);
			if (distance < minDistance)
			{
				minDistance = distance;
				index = i;
			}
		}
		return index;
	}

	TriangleIntersection::Area
	TriangleIntersection::GetOverlapArea(const Triangle& triangleA, const Triangle& triangleB)
	{
		Area area{};

		const PointsList verticesOfBInsideA = GetInsideVertices(triangleA, triangleB);
		const auto verticesCount = verticesOfBInsideA.size();
		if (verticesCount == 1)
		{
			const auto& verticeOfAInsideB = verticesOfBInsideA[0];
			const auto& intersectionPoints = GetTriangleSidesIntersectionPoints(triangleA, triangleB);
			if (intersectionPoints.size() == 2)
			{
				area.push_back(intersectionPoints[0]);
				area.push_back(verticeOfAInsideB);
				area.push_back(intersectionPoints[1]);
			}
		}
		else if (verticesCount == 2)
		{
			const auto& verticeOfAInsideB = verticesOfBInsideA[0];
			const auto& intersectionPoints = GetTriangleSidesIntersectionPoints(triangleA, triangleB);
			if (intersectionPoints.size() == 2)
			{
				// triangle 1
				area.push_back(verticesOfBInsideA[0]);
				area.push_back(intersectionPoints[0]);
				area.push_back(intersectionPoints[1]);

				// triangle 2
				area.push_back(intersectionPoints[1]);
				area.push_back(verticesOfBInsideA[1]);
				area.push_back(verticesOfBInsideA[0]);
			}
		}
		else if (verticesCount == 3)
		{
			area.assign(verticesOfBInsideA.begin(), verticesOfBInsideA.end());
		}
		else if (verticesCount == 0)
		{
			// no intersection
		}

		return area;
	}

	TriangleIntersection::PointsList
	TriangleIntersection::GetInsideVertices(const Triangle& triangleA, const Triangle& triangleB)
	{
		TriangleIntersection::PointsList insidePoints;

		const auto& verticesB = triangleB.GetVertices();
		auto itEnd = verticesB.end();
		for (auto it = verticesB.begin(); it != itEnd; ++it)
		{
			const auto& point = *it;
			if (Contains(triangleA, point))
			{
				insidePoints.push_back(point);
			}
		}

		return insidePoints;
	}

	TriangleIntersection::PointsList
	TriangleIntersection::GetIntersectionPoints(const Triangle& triangle, const glm::vec4& segmentStart, const glm::vec4& segmentEnd)
	{
		PointsList result;

		const auto& vertices = triangle.GetVertices();
		const int VERTICES_COUNT = Triangle::GetPointsCount();
		for (int i = 0; i < VERTICES_COUNT; ++i)
		{
			const int sideStartIndex = i;
			const int sideEndIndex = (i + 1) % VERTICES_COUNT;
			const auto point = MathUtil::GetIntersectionPoint(vertices[sideStartIndex], vertices[sideEndIndex], segmentStart, segmentEnd);
			if (point.w != 0.0f)
			{
				result.push_back(point);
			}
		}
		//FIXME The result points must be sorted ascending based on the distance from the segmentStart point.

		return result;
	}

	TriangleIntersection::Area
	TriangleIntersection::GetSumOfAreas(const Area& a, const Area& b)
	{
		Area sum;
		sum.assign(a.begin(), a.end());
		sum.insert(sum.end(), b.begin(), b.end());
		return sum;
	}
}