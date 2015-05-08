#ifndef _MY_CODE_UT_SQUARE_COLIDER_H_
#define _MY_CODE_UT_SQUARE_COLIDER_H_

#include <glm/glm.hpp>

namespace MyCode
{
	class UT_RectangleColider
	{
	public:
		UT_RectangleColider();
		~UT_RectangleColider();

		bool Validate();
	private:

		bool NoIntersection();
		bool NearEdgeIntersection();
		bool FarEdgeIntersection();
	};
}


#endif //_MY_CODE_UT_SQUARE_COLIDER_H_