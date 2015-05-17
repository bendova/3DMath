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

		bool DoPolygonsIntersect(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b);
		
		bool DoPolygonProjectionsIntersect(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2);

		std::pair<glm::vec3, glm::vec3> ProjectPolygonToAxis(const std::vector<glm::vec3>& polygon,
			const glm::vec3& axisPointA, const glm::vec3& axisPointB);

		bool DoColinearLineSegmentsIntersect(const glm::vec3& a, const glm::vec3& b,
			const glm::vec3& c, const glm::vec3& d);

		std::vector<glm::vec3> GetNearestPoints(const Rectangle& r1, const glm::vec3& targetPoint, const glm::vec3& otherCenter);
		std::vector<Collision> GetCollisions(const std::vector<glm::vec3>& forwardsR1, const std::vector<glm::vec3>& forwardsR2,
			const glm::vec3& directionVector);
		std::pair<Collision, bool> GetCollision(const std::vector<glm::vec3>& forwardsR1, const std::vector<glm::vec3>& forwardsR2,
			const glm::vec3& currentCenter, const glm::vec3& targetCenter);
		std::pair<glm::vec3, bool> GetClosestIntersectionPoint(const glm::vec3& a, const glm::vec3& b, const std::vector<glm::vec3>& lineSegments);
		glm::vec3 GetCenterThatAvoidCollision(const glm::vec3& currentCenter, const glm::vec3& targetCenter,
			const Collision& collision);
		
		std::vector<int> GetSortedIndicesByDistanceFromPoint(const std::vector<glm::vec3>& vertices,
			const glm::vec3& targetPoint);

		bool DoesTravelPathCollide(const Rectangle& rectangle, const glm::vec3& targetCenter, const Rectangle& obstacle);

		std::vector<glm::vec3> GetTravelPathBounding(const Rectangle& rectangle, const glm::vec3& targetCenter);
		bool IsBoundingPathRectangle(const std::vector<glm::vec3>& vertices, const glm::vec3& directionVector);
		std::vector<glm::vec3> GetBoundingPathAsRectangle(std::vector<glm::vec3> vertices,
			const std::vector<int> indices, const glm::vec3& directionVector);
		std::vector<glm::vec3> GetBoundingPathAsHexagon(const std::vector<glm::vec3>& vertices,
			const std::vector<int>& indices, const glm::vec3& directionVector);

		glm::vec3 GetPositionOnNearEdge(const Rectangle& a, const glm::vec3& targetCenter, const Rectangle& b);
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