#include "UnitTestRunner.h"

#include "intersection2d/UT_RectangleColider.h"
#include "intersection3d/UT_CubeCollider.h"
#include "intersection3d/UT_PolygonCollider.h"
#include "../framework/UT_VectorMath.h"

namespace MyCode
{
	namespace UnitTestRunner
	{
		void RunUnitTests()
		{
			using namespace Detail;
			ValidateVectorMath();
			ValidateColliderForPolygons2D();
			ValidateColliderForPolygons3D();
			ValidateCubeCollider();
		}

		namespace Detail
		{
			void ValidateVectorMath()
			{
				UT_VectorMath vectorMathUT;
				assert(vectorMathUT.Validate());
			}

			void ValidateColliderForPolygons2D()
			{
				UT_RectangleColider collider;
				assert(collider.Validate());
			}

			void ValidateColliderForPolygons3D()
			{
				UT_PolygonCollider collider;
				assert(collider.Validate());
			}

			void ValidateCubeCollider()
			{
				UT_CubeCollider collider;
				assert(collider.Validate());
			}
		}
	}
}