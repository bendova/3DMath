#ifndef _MY_CODE_INTERSECTION_3D_UT_POLYGON_INTERSECTION_H_
#define _MY_CODE_INTERSECTION_3D_UT_POLYGON_INTERSECTION_H_

#include "PolygonCollider.h"

namespace MyCode
{
	class UT_PolygonCollider
	{
	public:
		bool Validate();
	private:
		class Setup
		{
		public:
			Setup(const std::initializer_list<Polygon>& polygons)
				: mPolygons()
				, mCollider()
			{
				mPolygons.insert(mPolygons.end(), polygons.begin(), polygons.end());
				AddRectanglesToCollider();
			}

			Setup(const std::initializer_list<glm::vec3> centers)
				: mPolygons()
				, mCollider()
			{
				BuildPolygons(centers);
				AddRectanglesToCollider();
			}

			const PolygonCollider& Collider() { return mCollider; }
			const std::vector<Polygon>& Rectangles() { return mPolygons; }

			const Polygon& operator[](int index)
			{
				return mPolygons[index];
			}

		private:

			void BuildPolygons(const std::initializer_list<glm::vec3> centers)
			{
				const glm::vec3 vectorToA{ -0.5f, 0.0f, 0.5f };
				const glm::vec3 vectorToB{  0.5f, 0.0f, 0.5f };
				const glm::vec3 vectorToC{  0.5f, 0.0f, -0.5f };
				const glm::vec3 vectorToD{ -0.5f, 0.0f, -0.5f };

				for (const auto& center : centers)
				{
					mPolygons.push_back(Polygon{ center + vectorToA, center + vectorToB,
						center + vectorToC, center + vectorToD });
				}
			}

			void AddRectanglesToCollider()
			{
				mCollider.AddPolygons(mPolygons);
			}

			std::vector<Polygon> mPolygons;
			PolygonCollider mCollider;
		};

		class IntersectionTest3D
		{
		public:
			bool Run();
		private:
			bool NoCollision();
			bool CrossCollision();
			bool SamePlaneCollision();
		};

		class CollisionDetectionTest
		{
		public:
			bool Run();
		private:
			bool NoCollision();
			bool NearSideCollision();
			bool FarSideCollision();
			bool NoCollisionForSlidingPolygons();
			bool PointCollision();
		};

		class TravelPathBoundingTest2D
		{
		public:
			bool Run();
		private:
			bool BoundingPathForTriangle();
			bool BoundingPathForRectangle();
			bool BoundingPathForRotatedRectangle();
			bool BoundingPathForConvexIrregularPolygon();
		};

		class TravelPathBoundingTest3D
		{
		public:
			bool Run();
		private:
			bool BoundingPathForRectangle();
			bool BoundingPathForRotatedRectangle();
		};

		class CollisionAvoidanceTest
		{
		public:
			bool Run();
		private:
			bool ValidPositionForNoCollision();
			bool ValidPositionForPointCollision();
			bool ValidPositionForSideCollision();
			bool ValidPositionForTouchingSideCollision();
			bool MoveAwayOverlappingPolygons();
			bool MoveAwayFullyOverlappedPolygons();
			bool MoveAwayTouchingPolygons();
			bool SlideTouchingPolygons();
			bool SlideTouchingPolygons2();
		};
	};
}

#endif //_MY_CODE_INTERSECTION_3D_UT_POLYGON_INTERSECTION_H_