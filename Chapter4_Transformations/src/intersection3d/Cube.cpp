#include "Cube.h"

namespace MyCode
{
	const int Cube::FACES_COUNT = 6;

	Cube::Cube(const glm::vec3& center,
		const std::vector<glm::vec3>& bottomTopFacesVectorsCCW)
		: mCenter(center)
		, mFaces()
		, mBottomTopFacesVectorsCCW(bottomTopFacesVectorsCCW)
	{
		GenerateFaces();
	}

	void Cube::GenerateFaces()
	{
		const int sideFacesCount = 4;
		const int verticesPerFaceCount = 4;
		for (int i = 0; i < sideFacesCount; ++i)
		{
			const int aIndex = i;
			const int bIndex = (i + 1) % verticesPerFaceCount;
			const int cIndex = bIndex + verticesPerFaceCount;
			const int dIndex = aIndex + verticesPerFaceCount;
			AddFace(aIndex, bIndex, cIndex, dIndex);
		}

		AddFace(0, 1, 2, 3);// bottom face
		AddFace(4, 5, 6, 7);// top face
	}

	void Cube::AddFace(const int indexA, const int indexB, const int indexC, const int indexD)
	{
		const glm::vec3 a = mCenter + mBottomTopFacesVectorsCCW[indexA];
		const glm::vec3 b = mCenter + mBottomTopFacesVectorsCCW[indexB];
		const glm::vec3 c = mCenter + mBottomTopFacesVectorsCCW[indexC];
		const glm::vec3 d = mCenter + mBottomTopFacesVectorsCCW[indexD];

		mFaces.push_back(Polygon{ a, b, c, d });
	}

	void Cube::SetCenter(const glm::vec3& newCenter)
	{
		mCenter = newCenter;
		mFaces.clear();
		GenerateFaces();
	}
}