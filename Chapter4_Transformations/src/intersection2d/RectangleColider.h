#ifndef _MY_CODE_INTERSECTION_SQUARE_COLIDER_H_
#define _MY_CODE_INTERSECTION_SQUARE_COLIDER_H_

#include <glm/glm.hpp>
#include <vector>
#include <utility>
#include "Rectangle.h"

namespace MyCode
{
	class RectangleColider
	{
	public:
		void AddRectangle(const Rectangle& rectangle);
		void AddRectangles(const std::vector<Rectangle>& rectangles);
		glm::vec3 GetPositionThatAvoidsCollisions(const Rectangle& rectangle, glm::vec3 targetCenter) const;

	private:
		std::vector<const Rectangle*> mRectangles;
	};

	namespace RectangleColiderHelpers
	{
		std::vector<const Rectangle*> SortByDistanceFromPoint(std::vector<const Rectangle*> rectangles, const glm::vec3& point);
		glm::vec3 GetValidCenter(const Rectangle& rectangle, const Rectangle& obstacle, const glm::vec3& targetCenter);

		void CollisionSanityCheck(const Rectangle& target, const glm::vec3& newTargetCenter, const Rectangle& obstacle);
	}

	namespace Intersection2D
	{
		struct Collision
		{
			glm::vec3 mCollidingVertex;
			glm::vec3 mPointOfCollision;
		};

		namespace PolygonIntersection
		{
			bool DoPolygonsIntersect2D(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b);

			namespace Detail
			{
				enum class IntersectionType
				{
					POLYGON_WITH_POLYGON = 0,
					POLYGON_WITH_LINE_SEGMENT,
					LINE_SEGMENT_WITH_POLYGON,
					LINE_SEGMENT_WITH_LINE_SEGMENT,
				};

				IntersectionType GetIntersectionType(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b);
				bool IsLineSegment(const std::vector<glm::vec3>& points);
				std::pair<glm::vec3, glm::vec3> GetLineSegmentFromCollinearPoints(const std::vector<glm::vec3>& collinearPoints);
				std::vector<glm::vec3> GetPairwiseDistinctPoints(const std::vector<glm::vec3>& points, const int count);
				bool DoPolygonWithLineSegmentIntersection(const std::vector<glm::vec3>& polygon,
					const std::vector<glm::vec3>& collinearPoints);
				bool DoLineSegmentsIntersection(const std::vector<glm::vec3>& collinearPointsA,
					const std::vector<glm::vec3>& collinearPointsB);

				namespace PolygonsIntersection
				{
					bool DoCoplanarPolygonsIntersect(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2);
					bool DoPolygonsSideIntersection(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2);

					bool DoPolygonToAxisIntersection(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2,
						const glm::vec3& axisA, const glm::vec3& axisB);

					std::pair<float, float> ProjectPolygonToAxis(const std::vector<glm::vec3>& polygon,
						const glm::vec3& axisPointA, const glm::vec3& axisPointB);
					bool DoColinearLineSegmentsIntersect(const float& factorA, const float& factorB,
						const float& factorC, const float& factorD);
					bool DoSegmentsIntersect(const float& factorA, const float& factorB,
						const float& factorC, const float& factorD);
				}
			}
		}

		namespace TravelPathBounding
		{
			bool DoesTravelPathCollide(const Rectangle& rectangle, const glm::vec3& targetCenter, const Rectangle& obstacle);
			std::vector<glm::vec3> GetTravelPathBounding(const Rectangle& rectangle, const glm::vec3& targetCenter);
			std::vector<int> GetSortedIndicesByDistanceFromPoint(const std::vector<glm::vec3>& vertices,
				const glm::vec3& targetPoint);
			bool IsBoundingPathRectangle(const std::vector<glm::vec3>& vertices, const glm::vec3& directionVector);
			std::vector<glm::vec3> GetBoundingPathAsRectangle(std::vector<glm::vec3> vertices,
				const std::vector<int> indices, const glm::vec3& directionVector);
			std::vector<glm::vec3> GetBoundingPathAsHexagon(const std::vector<glm::vec3>& vertices,
				const std::vector<int>& indices, const glm::vec3& directionVector);
		}

		namespace CollisionAvoidance
		{
			class CollisionAvoider
			{
			public:
				enum class Avoidance
				{
					OUTSIDE_IN = 0,
					INSIDE_OUT
				};

				CollisionAvoider(const std::vector<glm::vec3>& verticesR1, const std::vector<glm::vec3>& verticesR2,
					const glm::vec3& currentCenter, const glm::vec3& targetCenter,
					const Avoidance avoidance = Avoidance::OUTSIDE_IN);
				
				const glm::vec3& GetValidCenter() const { return mValidCenter; }
			private:
				void DetermineValidCenter();
				std::pair<Collision, bool> GetNearEdgeCollision();
				std::vector<Collision> GetCollisionsFromBothPolygons();
				
				std::vector<Collision> GetCollisions(const std::vector<glm::vec3>& forwardsR1, const std::vector<glm::vec3>& forwardsR2,
					const glm::vec3& directionVector) const ;

				std::pair<glm::vec3, bool> GetClosestIntersectionPoint(const glm::vec3& a, const glm::vec3& b, 
					const std::vector<glm::vec3>& lineSegments) const;
				std::pair<glm::vec3, bool> GetFarthestIntersectionPoint(const glm::vec3& a, const glm::vec3& b,
					const std::vector<glm::vec3>& lineSegments) const;

				void SortAscending(std::vector<Collision>& collisions, const glm::vec3& biasPoint) const ;
				glm::vec3 GetCenterThatAvoidsCollision(const glm::vec3& currentCenter, const Collision& collision) const;
				
				const std::vector<glm::vec3>& mVerticesR1;
				const std::vector<glm::vec3>& mVerticesR2;
				const glm::vec3& mCurrentCenter; 
				const glm::vec3& mTargetCenter;
				const glm::vec3 mDirectionVector;
				const Avoidance mAvoidance;
				glm::vec3 mValidCenter;
			};

			glm::vec3 GetPositionOnNearEdge(const std::vector<glm::vec3>& verticesOfR1, const glm::vec3& currentCenter,
				const glm::vec3& targetCenter, const std::vector<glm::vec3>& verticesOfR2, 
				const CollisionAvoider::Avoidance avoidance = CollisionAvoider::Avoidance::OUTSIDE_IN);
		}
	}
}


#endif //_MY_CODE_INTERSECTION_SQUARE_COLIDER_H_