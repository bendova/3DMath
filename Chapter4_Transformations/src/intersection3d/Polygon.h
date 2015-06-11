#ifndef _MY_CODE_INTERSECTION_3D_POLYGON_H_
#define _MY_CODE_INTERSECTION_3D_POLYGON_H_

#include <vector>
#include <initializer_list>
#include <glm/glm.hpp>

namespace MyCode
{
	class Polygon
	{
	private:
		glm::vec3 mCenter;
		std::vector<glm::vec3> mVertices;

	public:
		Polygon(const std::initializer_list<glm::vec3>& vertices)
			: mVertices(vertices)
		{
			CalculateCenter();
		}

		Polygon(const std::vector<glm::vec3>& vertices)
			: mVertices(vertices)
		{
			CalculateCenter();
		}

		void SetCenter(const glm::vec3& center)
		{
			UpdateVertices(center);
			mCenter = center;
		}
		const glm::vec3& Center() const { return mCenter; }

		const glm::vec3& operator[](const int index) const
		{
			return mVertices[index];
		}

		const std::vector<glm::vec3>& Vertices() const
		{
			return mVertices;
		}
		std::vector<glm::vec3>& Vertices()
		{
			return mVertices;
		}

		bool operator==(const Polygon& other) const
		{
			return ((mCenter == other.mCenter) && (mVertices == other.mVertices));
		}

	private:
		void CalculateCenter()
		{
			glm::vec3 sum{ 0.0f };
			for (const glm::vec3& vertice : mVertices)
			{
				sum += vertice;
			}
			mCenter = sum / static_cast<float>(mVertices.size());
		}

		void UpdateVertices(const glm::vec3& newCenter)
		{
			for (glm::vec3& vertice : mVertices)
			{
				const glm::vec3 verticeOffset = vertice - mCenter;
				vertice = newCenter + verticeOffset;
			}
		}
	};
}

#endif //_MY_CODE_INTERSECTION_3D_POLYGON_H_