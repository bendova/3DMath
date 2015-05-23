#ifndef _MY_CODE_INTERSECTION_RECTANGLE_H_
#define _MY_CODE_INTERSECTION_RECTANGLE_H_

#include <vector>
#include <glm/glm.hpp>

namespace MyCode
{
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

#endif //_MY_CODE_INTERSECTION_RECTANGLE_H_