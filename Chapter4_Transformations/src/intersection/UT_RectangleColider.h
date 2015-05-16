#ifndef _MY_CODE_UT_SQUARE_COLIDER_H_
#define _MY_CODE_UT_SQUARE_COLIDER_H_

#include <glm/glm.hpp>
#include "RectangleColider.h"

namespace MyCode
{
	class UT_RectangleColider
	{
	public:
		bool Validate();
	private:
		struct Setup
		{
			Setup(const glm::vec3& c1, const glm::vec3& c2)
				: mCenterR1{ c1 }
				, mCenterR2{ c2 }
				, mVectorToA{ -0.5f, 0.0f,  0.5f }
				, mVectorToB{  0.5f, 0.0f,  0.5f }
				, mVectorToC{  0.5f, 0.0f, -0.5f }
				, mVectorToD{ -0.5f, 0.0f, -0.5f }
				, mRectangle1{ mCenterR1, mVectorToA, mVectorToB, mVectorToC, mVectorToD }
				, mRectangle2{ mCenterR2, mVectorToA, mVectorToB, mVectorToC, mVectorToD }
			{
				mColider.AddRectangle(mRectangle1);
				mColider.AddRectangle(mRectangle2);
			}

			const glm::vec3 mCenterR1;
			const glm::vec3 mCenterR2;
			const glm::vec3 mVectorToA;
			const glm::vec3 mVectorToB;
			const glm::vec3 mVectorToC;
			const glm::vec3 mVectorToD;
			Rectangle mRectangle1;
			Rectangle mRectangle2;
			RectangleColider mColider;
		};

		class RectangleCollisionTest
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

		class ForwardPointsTest
		{
		public:
			bool Run();
		private:
			bool ForwardPointsForMovingLeft();
			bool ForwardPointsForMovingRight();
			bool ForwardPointsForMovingForward();
			bool ForwardPointsForMovingBackward();
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
		};

		class ValidPositionTest
		{
		public:
			bool Run();
		private:
			bool ValidPositionForNoCollision();
			bool ValidPositionForNearEdgeIntersection();
			bool ValidPositionForFarEdgeIntersection();
			bool ValidPositionForPathIntersection();
			bool ValidPositionForEdgeIntersection();
			bool ValidPositionForCornerIntersection();
			bool ValidPositionForDiagonalPathIntersection();
			bool ValidPositionForCrossIntersection();
			bool ValidPositionForCrossIntersection2();
		};
	};

	bool AreSegmentsEqualWithinMargin(const std::pair<glm::vec3, glm::vec3>& ab, const std::pair<glm::vec3, glm::vec3>& cd,
		const double margin);
	bool AreVectorsEqualWithinMargin(const glm::vec3& a, const glm::vec3& b, const double margin);
}


#endif //_MY_CODE_UT_SQUARE_COLIDER_H_