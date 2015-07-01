#ifndef _MY_CODE_INTERSECTION_2D_TRAVEL_PATH_BOUNDING_H_
#define _MY_CODE_INTERSECTION_2D_TRAVEL_PATH_BOUNDING_H_

#include <vector>
#include <glm/glm.hpp>

namespace MyCode
{
	class Rectangle;

	namespace Intersection2D
	{
		namespace TravelPathBounding
		{
			bool DoesTravelPathCollide(const Rectangle& rectangle, const glm::vec3& targetCenter, const Rectangle& obstacle);
			bool DoesAnyVerticePathCollide(const std::vector<glm::vec3>& targetVertices, const glm::vec3& directionVector,
				const std::vector<glm::vec3>& obstacleVertices);

			std::vector<glm::vec3> GetTravelPathBounding(const Rectangle& rectangle, const glm::vec3& targetCenter);
			std::vector<int> GetSortedIndicesByDistanceFromPoint(const std::vector<glm::vec3>& vertices,
				const glm::vec3& targetPoint);
			bool IsBoundingPathRectangle(const std::vector<glm::vec3>& vertices, const glm::vec3& directionVector);
			std::vector<glm::vec3> GetBoundingPathAsRectangle(std::vector<glm::vec3> vertices,
				const std::vector<int> indices, const glm::vec3& directionVector);
			std::vector<glm::vec3> GetBoundingPathAsHexagon(const std::vector<glm::vec3>& vertices,
				const std::vector<int>& indices, const glm::vec3& directionVector);
		}
	}
}

#endif //_MY_CODE_INTERSECTION_2D_TRAVEL_PATH_BOUNDING_H_