#ifndef _MY_CODE_INTERSECTION_CUBE_COLIDER_H_
#define _MY_CODE_INTERSECTION_CUBE_COLIDER_H_

#include <vector>
#include <glm/glm.hpp>
#include "Cube.h"

namespace MyCode
{
	class CubeCollider
	{
	public:
		void AddCube(const Cube& c) { mCubes.push_back(&c); };
		glm::vec3 GetPositionThatAvoidCollisions(const Cube& cube, glm::vec3 targetCenter) const;

	private:
		std::vector<const Cube*> mCubes;
	};


	namespace CubeColliderHelpers
	{
		std::vector<const Cube*> SortByDistanceFromPoint(std::vector<const Cube*> rectangles, const glm::vec3& point);
		glm::vec3 GetValidPosition(const Cube& cube, const glm::vec3& targetCenter, const Cube& obstacle);
	}

	namespace TravelPathIntersection
	{
		bool DoesTravelPathCollide(const Cube& cube, const glm::vec3& targetCenter, const Cube& obstacle);
		bool DoesTravellingRectangleCollideCube(const Rectangle& target, const glm::vec3& targetCenter, const Cube& obstacle);
	}
}

#endif //_MY_CODE_INTERSECTION_CUBE_COLIDER_H_