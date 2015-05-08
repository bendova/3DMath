#ifndef _MY_CODE_INTERSECTION_SQUARE_COLIDER_H_
#define _MY_CODE_INTERSECTION_SQUARE_COLIDER_H_

#include <glm/glm.hpp>
#include <vector>

namespace MyCode
{
	class Rectangle
	{
	public:
		Rectangle(const glm::vec3* center, const glm::vec3 vectorToA, 
			const glm::vec3 vectorToB, const glm::vec3 vectorToC,
			const glm::vec3 vectorToD)
			: mCenter(center)
			, mVectorToA(vectorToA)
			, mVectorToB(vectorToB)
			, mVectorToC(vectorToC)
			, mVectorToD(vectorToD)
		{}

	private:
		const glm::vec3* mCenter;
		const glm::vec3 mVectorToA;
		const glm::vec3 mVectorToB;
		const glm::vec3 mVectorToC;
		const glm::vec3 mVectorToD;
	};

	class RectangleColider
	{
	public:
		RectangleColider();
		~RectangleColider();
		
		void AddRectangle(const Rectangle& rectangle);
		glm::vec3 GetPositionThatAvoidCollisions(const Rectangle& rectangle, const glm::vec3& targetCenter) const;

	private:
		std::vector<Rectangle> mRectangle;
	};
}


#endif //_MY_CODE_INTERSECTION_SQUARE_COLIDER_H_