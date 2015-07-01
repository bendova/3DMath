#ifndef _MY_CODE_INTERSECTION_SQUARE_COLIDER_H_
#define _MY_CODE_INTERSECTION_SQUARE_COLIDER_H_

#include <glm/glm.hpp>
#include <vector>
#include <utility>
#include "Rectangle.h"

namespace MyCode
{
	class RectangleColider
	{
	public:
		void AddRectangle(const Rectangle& rectangle);
		void AddRectangles(const std::vector<Rectangle>& rectangles);
		glm::vec3 GetPositionThatAvoidsCollisions(const Rectangle& rectangle, glm::vec3 targetCenter) const;

	private:
		std::vector<const Rectangle*> mRectangles;
	};

	namespace RectangleColiderHelpers
	{
		std::vector<const Rectangle*> SortByDistanceFromPoint(std::vector<const Rectangle*> rectangles, const glm::vec3& point);
		glm::vec3 GetValidCenter(const Rectangle& rectangle, const Rectangle& obstacle, const glm::vec3& targetCenter);

		void CollisionSanityCheck(const Rectangle& target, const glm::vec3& newTargetCenter, const Rectangle& obstacle);
	}
}


#endif //_MY_CODE_INTERSECTION_SQUARE_COLIDER_H_