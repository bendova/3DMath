#ifndef _MY_CODE_UNIT_TEST_RUNNER_H_
#define _MY_CODE_UNIT_TEST_RUNNER_H_

namespace MyCode
{
	namespace UnitTestRunner
	{
		void RunUnitTests();

		namespace Detail
		{
			void ValidateVectorMath();
			void ValidateColliderForPolygons2D();
			void ValidateColliderForPolygons3D();
			void ValidateCubeCollider();
		}
	}
}

#endif //_MY_CODE_UNIT_TEST_RUNNER_H_