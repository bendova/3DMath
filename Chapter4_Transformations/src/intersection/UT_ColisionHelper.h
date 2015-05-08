#ifndef _MY_CODE_UT_COLISION_HELPER_H_
#define _MY_CODE_UT_COLISION_HELPER_H_

#include <glm/glm.hpp>
#include "ColisionHelper.h"
#include "../control/ControlHelper.h"

namespace MyCode
{
	class UT_ColisionHelper
	{
	public:
		UT_ColisionHelper();
		~UT_ColisionHelper();

		bool Validate();
	private:
		struct Setup
		{
			Setup(const glm::vec3& posA, const glm::vec3& posB)
				: mSideLength(1.0f)
				, mIntersecter()
				, mControlA{ posA, mSideLength, mIntersecter }
				, mControlB{ posB, mSideLength, mIntersecter }
			{
				mIntersecter.AddControlHelper(&mControlA);
				mIntersecter.AddControlHelper(&mControlB);
			}

			const float mSideLength;
			ColisionHelper mIntersecter;
			ControlHelper mControlA;
			ControlHelper mControlB;
		};
		
		bool NoIntersection();
		bool NearEdgeIntersection();
		bool FarEdgeIntersection();
	};
}


#endif //_MY_CODE_UT_COLISION_HELPER_H_