#ifndef _MY_CODE_INTERSECTION_3D_POLYGON_H_
#define _MY_CODE_INTERSECTION_3D_POLYGON_H_

#include <vector>
#include <initializer_list>
#include <glm/glm.hpp>
#include <cassert>
#include <../framework/VectorMath.h>

namespace MyCode
{
	class Polygon
	{
	private:
		glm::vec3 mCenter;
		std::vector<glm::vec3> mVertices;

	public:
		Polygon(const std::initializer_list<glm::vec3>& vertices)
			: mCenter(0.0f)
			, mVertices(vertices)
		{
			Init();
		}

		Polygon(const std::vector<glm::vec3>& vertices)
			: mCenter(0.0f)
			, mVertices(vertices)
		{
			Init();
		}

		Polygon(std::vector<glm::vec3>&& vertices)
			: mCenter(0.0f)
			, mVertices(std::forward<std::vector<glm::vec3>>(vertices))
		{
			Init();
		}

		void Init();
		void DoSanityCheck();
		void AssertVerticesAreCoplanar();

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
		void CalculateCenter();
		void UpdateVertices(const glm::vec3& newCenter);
	};
}

#endif //_MY_CODE_INTERSECTION_3D_POLYGON_H_