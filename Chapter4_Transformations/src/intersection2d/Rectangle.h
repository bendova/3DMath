#ifndef _MY_CODE_INTERSECTION_RECTANGLE_H_
#define _MY_CODE_INTERSECTION_RECTANGLE_H_

#include <vector>
#include <glm/glm.hpp>
#include <utility>

namespace MyCode
{
	class Rectangle
	{
	private:
		glm::vec3 mCenter;
		std::vector<glm::vec3> mOffsets;
		float mInscribedCircleRadius;
		float mCircumscribedCircleRadius;

	public:
		Rectangle(const glm::vec3& a, const glm::vec3& b,
			const glm::vec3& c, const glm::vec3& d);

		Rectangle(const glm::vec3& center, const glm::vec3& vectorToA,
			const glm::vec3& vectorToB, const glm::vec3& vectorToC,
			const glm::vec3& vectorToD);

		void UpdateBoundingRadiuses();

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

		float CircumbscribedCircleRadius() const
		{
			return mCircumscribedCircleRadius;
		}
		float InscribedCircleRadius() const
		{
			return mInscribedCircleRadius;
		}
	};
}

#endif //_MY_CODE_INTERSECTION_RECTANGLE_H_