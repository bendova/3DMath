#include "TriangleIntersection.h"
#include <float.h>
#include <unordered_set>
#include "../framework/VectorMath.h"

namespace MyCode
{
	TriangleIntersection::TriangleIntersection()
	{}

	TriangleIntersection::~TriangleIntersection()
	{}

	TriangleIntersection::Area
	TriangleIntersection::GetIntersectionArea(const Triangle& triangleA, const Triangle& triangleB)
	{
		UnorderedSet<Point, std::hash<Point>> perimeterPoints;

		const auto& verticesA = triangleA.GetVertices();
		const int VERTICES_COUNT = Triangle::GetPointsCount();
		for (int i = 0; i < VERTICES_COUNT; ++i)
		{
			const int sideStartIndex = i;
			const int sideEndIndex = (i + 1) % VERTICES_COUNT;
			const auto& sideStartPoint = verticesA[sideStartIndex];
			const auto& sideEndPoint = verticesA[sideEndIndex];
			if (Contains(triangleB, sideStartPoint))
			{
				perimeterPoints.insert(sideStartPoint);
			}
			const auto sideIntersectionPoints = GetIntersectionPoints(triangleB, sideStartPoint, sideEndPoint);
			perimeterPoints.insert(sideIntersectionPoints.begin(), sideIntersectionPoints.end());
		}

		const auto& verticesB = triangleB.GetVertices();
		for (int i = 0; i < VERTICES_COUNT; ++i)
		{
			const auto& sideStartPoint = verticesB[i];
			if (Contains(triangleA, sideStartPoint))
			{
				perimeterPoints.insert(sideStartPoint);
			}
		}
		
		Area area;
		auto it = perimeterPoints.begin();
		auto itEnd = perimeterPoints.end();
		while (it != itEnd)
		{
			area.push_back(*(*it++));
		}
		return area;
	}

	TriangleIntersection::PointsList
	TriangleIntersection::GetIntersectionPoints(const Triangle& triangle, const glm::vec4& segmentStart, const glm::vec4& segmentEnd)
	{
		PointsList intersectionPoints;

		const auto& vertices = triangle.GetVertices();
		const int VERTICES_COUNT = Triangle::GetPointsCount();
		for (int i = 0; i < VERTICES_COUNT; ++i)
		{
			const int sideStartIndex = i;
			const int sideEndIndex = (i + 1) % VERTICES_COUNT;
			const auto point = GetIntersectionPointBetweenSegments(vertices[sideStartIndex], vertices[sideEndIndex], segmentStart, segmentEnd);
			if (point.w != 0.0f)
			{
				intersectionPoints.push_back(point);
			}
		}
		std::sort(intersectionPoints.begin(), intersectionPoints.end(), [&segmentStart](Point &a, Point &b)
		{
			return glm::length(a - segmentStart) < glm::length(b - segmentStart);
		});

		return intersectionPoints;
	}
}