#include "Rectangle.h"
#include "vectormath/Common.h"

namespace MyCode
{
	Rectangle::Rectangle(const glm::vec3& a, const glm::vec3& b,
		const glm::vec3& c, const glm::vec3& d)
		: mCenter((a + b + c + d) / 4.0f)
		, mOffsets()
		, mInscribedCircleRadius(0.0f)
		, mCircumscribedCircleRadius(0.0f)
	{
		mOffsets.push_back(a - mCenter);
		mOffsets.push_back(b - mCenter);
		mOffsets.push_back(c - mCenter);
		mOffsets.push_back(d - mCenter);

		UpdateBoundingRadiuses();
	}

	Rectangle::Rectangle(const glm::vec3& center, const glm::vec3& vectorToA,
		const glm::vec3& vectorToB, const glm::vec3& vectorToC,
		const glm::vec3& vectorToD)
		: mCenter(center)
		, mOffsets()
		, mInscribedCircleRadius(0.0f)
		, mCircumscribedCircleRadius(0.0f)
	{
		mOffsets.push_back(vectorToA);
		mOffsets.push_back(vectorToB);
		mOffsets.push_back(vectorToC);
		mOffsets.push_back(vectorToD);

		UpdateBoundingRadiuses();
	}

	void Rectangle::UpdateBoundingRadiuses()
	{
		std::pair<float, float> minMaxPair = VectorMath::GetMinMaxLengthsPair(mOffsets);
		mInscribedCircleRadius = minMaxPair.first;
		mCircumscribedCircleRadius = minMaxPair.second;
	}
}
