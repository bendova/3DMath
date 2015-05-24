#ifndef _MY_CODE_UT_VECTOR_MATH_H_
#define _MY_CODE_UT_VECTOR_MATH_H_

namespace MyCode
{
	class UT_VectorMath
	{
	public:
		bool Validate();
	private:

		class PointToPlaneProjectionTest
		{
		public:
			bool Run();
		private:
			bool PointToPlaneProjection();
			bool PointToPlaneProjection1();
		};
	};
}

#endif //_MY_CODE_UT_VECTOR_MATH_H_