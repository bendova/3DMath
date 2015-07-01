#include "TravelPathBounding.h"
#include "Rectangle.h"
#include "PolygonIntersection.h"

namespace MyCode
{
	namespace Intersection2D
	{
		namespace TravelPathBounding
		{
			bool DoesTravelPathCollide(const Rectangle& rectangle, const glm::vec3& targetCenter, const Rectangle& obstacle)
			{
				const auto travelPathPolygon = TravelPathBounding::GetTravelPathBounding(rectangle, targetCenter);
				const std::vector<glm::vec3> obstacleVertices{ obstacle.A(), obstacle.B(), obstacle.C(), obstacle.D() };
				const bool doTheyOverlap = PolygonIntersection::DoPolygonsIntersect2D(travelPathPolygon, obstacleVertices);
				return doTheyOverlap;
			}

			std::vector<glm::vec3> GetTravelPathBounding(const Rectangle& rectangle, const glm::vec3& targetCenter)
			{
				const auto& currentCenter = rectangle.Center();
				std::vector<glm::vec3> vertices{ rectangle.A(), rectangle.B(), rectangle.C(), rectangle.D() };
				auto indices = GetSortedIndicesByDistanceFromPoint(vertices, targetCenter);

				const glm::vec3 directionVector = targetCenter - currentCenter;

				std::vector<glm::vec3> travelPathBoundingRectangle;
				bool isBoundingPathRectangle = IsBoundingPathRectangle(vertices, directionVector);
				if (isBoundingPathRectangle)
				{
					travelPathBoundingRectangle = GetBoundingPathAsRectangle(vertices, indices, directionVector);
				}
				else
				{
					travelPathBoundingRectangle = GetBoundingPathAsHexagon(vertices, indices, directionVector);
				}

				return travelPathBoundingRectangle;
			}

			std::vector<int> GetSortedIndicesByDistanceFromPoint(const std::vector<glm::vec3>& vertices,
				const glm::vec3& targetPoint)
			{
				std::vector<int> indices{ 0, 1, 2, 3 };
				std::sort(indices.begin(), indices.end(),
					[&vertices, &targetPoint](const int i, const int j)
				{
					return glm::length(vertices[i] - targetPoint) < glm::length(vertices[j] - targetPoint);
				});
				return indices;
			}

			bool IsBoundingPathRectangle(const std::vector<glm::vec3>& vertices,
				const glm::vec3& directionVector)
			{
				const glm::vec3 ab = vertices[1] - vertices[0];
				const glm::vec3 bc = vertices[2] - vertices[1];

				const float dotAbDirection = glm::dot(ab, directionVector);
				const float dotBcDirection = glm::dot(bc, directionVector);
				const bool isDirectionNormalToAB = (dotAbDirection == 0.0f);
				const bool isDirectionNormalToBC = (dotBcDirection == 0.0f);

				return (isDirectionNormalToAB || isDirectionNormalToBC);
			}

			std::vector<glm::vec3> GetBoundingPathAsRectangle(std::vector<glm::vec3> vertices,
				const std::vector<int> indices, const glm::vec3& directionVector)
			{
				vertices[indices[0]] += directionVector;
				vertices[indices[1]] += directionVector;

				const std::vector<glm::vec3> travelPathBoundingRectangle{ vertices.begin(), vertices.end() };
				return travelPathBoundingRectangle;
			}

			std::vector<glm::vec3> GetBoundingPathAsHexagon(const std::vector<glm::vec3>& vertices,
				const std::vector<int>& indices, const glm::vec3& directionVector)
			{
				const int hexagonVerticesCount = 6;
				std::vector<glm::vec3> travelPathBoundingRectangle{ static_cast<unsigned>(hexagonVerticesCount) };

				const int rectangleVerticesCount = 4;
				const int triangleVerticesCount = 3;
				int j = 0;
				for (int i = indices[0] + 1; j < triangleVerticesCount; ++j)
				{
					i %= rectangleVerticesCount;
					travelPathBoundingRectangle[j] = vertices[i];
					++i;
				}

				for (int i = indices[3] + 1; j < hexagonVerticesCount; ++j)
				{
					i %= rectangleVerticesCount;
					travelPathBoundingRectangle[j] = vertices[i] + directionVector;
					++i;
				}

				return travelPathBoundingRectangle;
			}
		}
	}
}
