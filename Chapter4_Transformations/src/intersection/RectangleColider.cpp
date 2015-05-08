#include "RectangleColider.h"

namespace MyCode
{
	RectangleColider::RectangleColider()
	{}

	RectangleColider::~RectangleColider()
	{}

	void RectangleColider::AddRectangle(const Rectangle& rectangle)
	{
		mRectangle.push_back(rectangle);
	}

	glm::vec3 RectangleColider::GetPositionThatAvoidCollisions(const Rectangle& rectangle, const glm::vec3& targetCenter) const
	{
		return glm::vec3{ 0.0f };
	}
}