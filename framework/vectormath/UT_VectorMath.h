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

		class LinesIntersection
		{
		public:
			bool Run();
		private:
			bool NoIntersectionBetweenClosedLineSegments();
			bool NoIntersectionBetweenOpenLineSegments();
			bool IntersectionBetweenClosedLineSegments();
			bool IntersectionBetweenOpenLineSegments();
			bool IntersectionBetweenLineSegmentAndRay();
			bool IntersectionBetweenLineSegmentAndLine();
			
			bool NoIntersectionBetweenRays();
			bool IntersectionBetweenRays();

			bool NoIntersectionBetweenLines();
			bool IntersectionBetweenLines();
		};

		class ColinearLineSegmentsIntersection
		{
		public:
			bool Run();
		private:
			bool NoIntersection();
			bool IntersectionInA();
			bool IntersectionInB();
			bool IntersectionInC();
			bool IntersectionInD();
			bool IntersectionWithMarginError();
			bool IntersectionOpenEnded();
			bool IntersectionOpenEndedOverlapped();
			bool IntersectionOpenEndedOverlapped2();
		};

		class LineSegmentPolygonIntersection
		{
		public:
			bool Run();
		private:
			bool NoIntersection();
			bool SegmentCrossingThroughPolygon();
			bool SegmentIsInPolygon();
			bool SegmentIntersectsSideOfCoplanarPolygon();
			bool SegmentIntersectsSideOfPolygon();
			bool NoIntersectionForTouchingPolygon();
		};

		class CoplanarityTest
		{
		public:
			bool Run();
		private:
			bool PointIsNotInPlane();
			bool PointIsInPlane();
			bool PlaneDescription();
		};
	};
}

#endif //_MY_CODE_UT_VECTOR_MATH_H_