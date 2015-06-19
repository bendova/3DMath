#include "Polygon.h"
#include <cassert>
#include <../framework/VectorMath.h>

namespace MyCode
{
	void Polygon::Init()
	{
		DoSanityCheck();
		CalculateCenter();
	}

	void Polygon::DoSanityCheck()
	{
		assert(mVertices.size() > 2);
		AssertVerticesAreCoplanar();
	}

	void Polygon::AssertVerticesAreCoplanar()
	{
		const auto& a = mVertices[0];
		const auto& b = mVertices[1];
		const auto& c = mVertices[2];
		const glm::vec3 normalToPlane = glm::cross(b - a, c - a);
		const size_t verticesCount = mVertices.size();
		for (size_t i = 3; i < verticesCount; ++i)
		{
			const glm::vec3 ap = mVertices[i] - a;
			const bool isInPlane = VectorMath::AreEqualWithMargin(glm::dot(normalToPlane, ap), 0.0f);
			assert(isInPlane);
			if (isInPlane == false)
			{
				break;
			}
		}
	}

	void Polygon::CalculateCenter()
	{
		glm::vec3 sum{ 0.0f };
		for (const glm::vec3& vertice : mVertices)
		{
			sum += vertice;
		}
		mCenter = sum / static_cast<float>(mVertices.size());
	}

	void Polygon::UpdateVertices(const glm::vec3& newCenter)
	{
		for (glm::vec3& vertice : mVertices)
		{
			const glm::vec3 verticeOffset = vertice - mCenter;
			vertice = newCenter + verticeOffset;
		}
	}
}
