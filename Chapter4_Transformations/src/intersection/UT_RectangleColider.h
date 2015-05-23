#ifndef _MY_CODE_UT_SQUARE_COLIDER_H_
#define _MY_CODE_UT_SQUARE_COLIDER_H_

#include <glm/glm.hpp>
#include <initializer_list>
#include "RectangleColider.h"

namespace MyCode
{
	class UT_RectangleColider
	{
	public:
		bool Validate();
	private:
		class Setup
		{
		public:
			Setup(const std::initializer_list<glm::vec3> centers)
				: mRectangles()
				, mColider()
			{
				BuildRectangles(centers);
				AddRectanglesToCollider();
			}

			const RectangleColider& Collider() { return mColider; }
			const std::vector<Rectangle>& Rectangles() { return mRectangles; }

			const Rectangle& operator[](int index)
			{
				return mRectangles[index];
			}

		private:
			
			void BuildRectangles(const std::initializer_list<glm::vec3> centers)
			{
				const glm::vec3 vectorToA{ -0.5f, 0.0f, 0.5f };
				const glm::vec3 vectorToB{ 0.5f, 0.0f, 0.5f };
				const glm::vec3 vectorToC{ 0.5f, 0.0f, -0.5f };
				const glm::vec3 vectorToD{ -0.5f, 0.0f, -0.5f };

				for (const auto& center : centers)
				{
					mRectangles.emplace_back(center, vectorToA, vectorToB, vectorToC, vectorToD);
				}
			}

			void AddRectanglesToCollider()
			{
				for (const auto& rectangle : mRectangles)
				{
					mColider.AddRectangle(rectangle);
				}
			}

			std::vector<Rectangle> mRectangles;
			RectangleColider mColider;
		};

		class CollisionTest2D
		{
		public:
			bool Run();
		private:
			bool NoCollision();
			bool TouchingInOnePoint();
			bool TouchingOnOneSide();
			bool CollisionInOnePoint();
			bool CollisionOnOneSide();
			bool CompleteOverlap();
			bool CollisionOfRotatedRectangles();
			bool NoCollisionOfRotatedRectangles();
		};

		class CollisionTest3D
		{
		public:
			bool Run();
		private:
			bool NoCollision();
			bool PointToPlaneProjection();
			bool CrossCollision();
		};

		class BoundingPathTest
		{
		public:
			bool Run();
		private:
			bool MoveLeft();
			bool MoveRight();
			bool MoveForward();
			bool MoveBackward();
			bool MoveDiagonallyLeftBackward();
			bool MoveDiagonallyLeftForward();
			bool MoveDiagonallyRightBackward();
			bool MoveDiagonallyRightForward();
		};
		
		class ProjectionToAxesTest
		{
		public:
			bool Run();
		private:
			bool ProjectRightTriangle();
			bool ProjectObtuseTriangle();
			bool ProjectTrapeze();
			bool ProjectPentagon();

			std::pair<glm::vec3, glm::vec3> ProjectPointsToAxis(const std::vector<glm::vec3>& points, const glm::vec3& lineA, const glm::vec3& lineB);
			std::pair<glm::vec3, glm::vec3> GetPointsOnLine(std::pair<float, float> factors,
				const glm::vec3& lineA, const glm::vec3& lineB);
		};

		class ValidPositionTest
		{
		public:
			bool Run();
		private:
			bool NoCollision();
			bool NearEdgeIntersection();
			bool FarEdgeIntersection();
			bool PathIntersection();
			bool EdgeIntersection();
			bool CornerIntersection();
			bool DiagonalPathIntersection();
			bool CrossIntersection();
			bool CrossIntersection2();
			bool CrossIntersection3();
			bool CrossIntersection4();
			bool CrossIntersection5();
			bool CrossIntersection6();
			bool CrossIntersection7();
		};

		class MultipleObjectsAvoidanceTest
		{
		public:
			bool Run();
		private:
			bool AvoidTwoObjects();
		};

		class SteppingOutOfCollisionTest
		{
		public:
			bool Run();
		private:
			bool StepOutOfCollision4();
			bool StepOutOfCollision();
			bool StepOutOfCollision2();
			bool StepOutOfCollision3();
		};
	};
}


#endif //_MY_CODE_UT_SQUARE_COLIDER_H_