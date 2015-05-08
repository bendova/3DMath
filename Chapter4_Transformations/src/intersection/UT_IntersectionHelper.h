#ifndef _MY_CODE_UT_INTERSECTION_HELPER_H_
#define _MY_CODE_UT_INTERSECTION_HELPER_H_

#include <glm/glm.hpp>
#include "IntersectionHelper.h"
#include "../control/ControlHelper.h"

namespace MyCode
{
	class UT_IntersectionHelper
	{
	public:
		UT_IntersectionHelper();
		~UT_IntersectionHelper();

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
			IntersectionHelper mIntersecter;
			ControlHelper mControlA;
			ControlHelper mControlB;
		};
		
		bool NoIntersection();
		bool NearEdgeIntersection();
		bool FarEdgeIntersection();
	};
}


#endif //_MY_CODE_UT_INTERSECTION_HELPER_H_