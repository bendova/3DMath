#ifndef _MY_CODE_INTERSECTION_CUBE_H_
#define _MY_CODE_INTERSECTION_CUBE_H_

#include <vector>
#include <glm/glm.hpp>
#include "Rectangle.h"

namespace MyCode
{
	class Cube
	{
	public:
		Cube(const glm::vec3& center,
			const std::vector<glm::vec3>& bottomTopFacesVectorsCCW);

		const std::vector<Rectangle>& GetFaces() const { return mFaces; }
		const glm::vec3& GetCenter() const { return mCenter; }
		void SetCenter(const glm::vec3& newCenter);

		int FacesCount() const { return FACES_COUNT; }

		const Rectangle& FrontFace()	const { return mFaces[0]; }
		const Rectangle& RightFace()	const { return mFaces[1]; }
		const Rectangle& BackFace()		const { return mFaces[2]; }
		const Rectangle& LeftFace()		const { return mFaces[3]; }
		const Rectangle& BottomFace()	const { return mFaces[4]; }
		const Rectangle& TopFace()		const { return mFaces[5]; }

		const Rectangle& operator[](const int index) const
		{
			return mFaces[index];
		}

	private:
		void GenerateFaces();
		void AddFace(const int indexA, const int indexB, const int indexC, const int indexD);

		static const int FACES_COUNT;
		glm::vec3 mCenter;
		std::vector<Rectangle> mFaces;
		const std::vector<glm::vec3> mBottomTopFacesVectorsCCW;
	};
}

#endif //_MY_CODE_INTERSECTION_CUBE_H_