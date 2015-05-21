#ifndef _MY_CODE_INTERSECTION_SQUARE_COLIDER_H_
#define _MY_CODE_INTERSECTION_SQUARE_COLIDER_H_

#include <glm/glm.hpp>
#include <vector>
#include <utility>

namespace MyCode
{
	class Rectangle;

	namespace PolygonCollision
	{
		struct Collision
		{
			glm::vec3 mCollidingVertex;
			glm::vec3 mPointOfCollision;
		};

		namespace PolygonIntersection
		{
			bool DoPolygonsIntersect(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b);
			bool DoPolygonProjectionsIntersect(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2);
			std::pair<float, float> ProjectPolygonToAxis(const std::vector<glm::vec3>& polygon,
				const glm::vec3& axisPointA, const glm::vec3& axisPointB);
			bool DoColinearLineSegmentsIntersect(const float& factorA, const float& factorB,
				const float& factorC, const float& factorD);
			bool DoSegmentsIntersect(const float& factorA, const float& factorB,
				const float& factorC, const float& factorD);
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
				const glm::vec3& targetCenter, const std::vector<glm::vec3> verticesOfR2, 
				const CollisionAvoider::Avoidance avoidance = CollisionAvoider::Avoidance::OUTSIDE_IN);
		}
	}

	class RectangleColider
	{
		friend class UT_RectangleColider;

	public:
		void AddRectangle(const Rectangle& rectangle);
		glm::vec3 GetPositionThatAvoidCollisions(const Rectangle& rectangle, glm::vec3 targetCenter) const;

	private:
		std::vector<const Rectangle*> mRectangles;
	};

	std::vector<const Rectangle*> SortByDistanceFromPoint(std::vector<const Rectangle*> rectangles, const glm::vec3& point);
	glm::vec3 GetValidCenter(const Rectangle& rectangle, const Rectangle& obstacle, const glm::vec3& targetCenter);

	void CollisionSanityCheck(const Rectangle& target, const glm::vec3& newTargetCenter, const Rectangle& obstacle);

	class Rectangle
	{
	private:
		glm::vec3 mCenter;
		std::vector<glm::vec3> mOffsets;

	public:
		Rectangle(const glm::vec3& a, const glm::vec3& b, 
				const glm::vec3& c, const glm::vec3& d)
		{
			mCenter = (a + b + c + d) / 4.0f;
			mOffsets.push_back(a - mCenter);
			mOffsets.push_back(b - mCenter);
			mOffsets.push_back(c - mCenter);
			mOffsets.push_back(d - mCenter);
		}

		Rectangle(const glm::vec3& center, const glm::vec3& vectorToA,
			const glm::vec3& vectorToB, const glm::vec3& vectorToC,
			const glm::vec3& vectorToD)
			: mCenter(center)
			, mOffsets()
		{
			mOffsets.push_back(vectorToA);
			mOffsets.push_back(vectorToB);
			mOffsets.push_back(vectorToC);
			mOffsets.push_back(vectorToD);
		}

		void SetCenter(const glm::vec3& center)
		{
			mCenter = center;
		}

		const glm::vec3& Center() const { return mCenter; }
		glm::vec3 A() const { return mCenter + OffsetToA(); }
		glm::vec3 B() const { return mCenter + OffsetToB(); }
		glm::vec3 C() const { return mCenter + OffsetToC(); }
		glm::vec3 D() const { return mCenter + OffsetToD(); }

		const glm::vec3& OffsetToA() const { return mOffsets[0]; }
		const glm::vec3& OffsetToB() const { return mOffsets[1]; }
		const glm::vec3& OffsetToC() const { return mOffsets[2]; }
		const glm::vec3& OffsetToD() const { return mOffsets[3]; }

		glm::vec3 operator[](const int index) const
		{
			return mCenter + mOffsets[index];
		}

		auto OffsetsBegin() const -> decltype(mOffsets.cbegin())
		{
			return mOffsets.cbegin();
		}

		auto OffsetsEnd() const -> decltype(mOffsets.cend())
		{
			return mOffsets.cend();
		}

		auto VerticesCount() const -> decltype(mOffsets.size())
		{
			return mOffsets.size();
		}

		bool operator==(const Rectangle& other) const
		{
			return ((mCenter == other.mCenter) && (mOffsets == other.mOffsets));
		}
	};
}


#endif //_MY_CODE_INTERSECTION_SQUARE_COLIDER_H_