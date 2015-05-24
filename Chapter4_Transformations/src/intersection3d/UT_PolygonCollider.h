#ifndef _MY_CODE_INTERSECTION_3D_UT_POLYGON_INTERSECTION_H_
#define _MY_CODE_INTERSECTION_3D_UT_POLYGON_INTERSECTION_H_

namespace MyCode
{
	class UT_PolygonCollider
	{
	public:
		bool Validate();
	private:

		class CollisionTest3D
		{
		public:
			bool Run();
		private:
			bool PointToPlaneProjection();
			bool PointToPlaneProjection1();
			bool NoCollision();
			bool CrossCollision();
		};
	};
}

#endif //_MY_CODE_INTERSECTION_3D_UT_POLYGON_INTERSECTION_H_