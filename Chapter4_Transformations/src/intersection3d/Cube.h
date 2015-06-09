#ifndef _MY_CODE_INTERSECTION_CUBE_H_
#define _MY_CODE_INTERSECTION_CUBE_H_

#include <vector>
#include <glm/glm.hpp>
#include "Polygon.h"

namespace MyCode
{
	class Cube
	{
	public:
		Cube(const glm::vec3& center,
			const std::vector<glm::vec3>& bottomTopFacesVectorsCCW);

		const std::vector<Polygon>& GetFaces() const { return mFaces; }
		const glm::vec3& Center() const { return mCenter; }
		void SetCenter(const glm::vec3& newCenter);

		int FacesCount() const { return FACES_COUNT; }

		const Polygon& FrontFace()	const { return mFaces[0]; }
		const Polygon& RightFace()	const { return mFaces[1]; }
		const Polygon& BackFace()	const { return mFaces[2]; }
		const Polygon& LeftFace()	const { return mFaces[3]; }
		const Polygon& BottomFace()	const { return mFaces[4]; }
		const Polygon& TopFace()	const { return mFaces[5]; }

		const Polygon& operator[](const int index) const
		{
			return mFaces[index];
		}

		float CircumbscribedCircleRadius() const;
		float InscribedCircleRadius() const;
	private:
		void GenerateFaces();
		void AddFace(const int indexA, const int indexB, const int indexC, const int indexD);
		void UpdateBoundingRadiuses();

		static const int FACES_COUNT;
		glm::vec3 mCenter;
		std::vector<Polygon> mFaces;
		const std::vector<glm::vec3> mBottomTopFacesVectorsCCW;
		float mInscribedCircleRadius;
		float mCircumscribedCircleRadius;
	};

	inline float Cube::CircumbscribedCircleRadius() const
	{
		return mCircumscribedCircleRadius;
	}
	inline float Cube::InscribedCircleRadius() const
	{
		return mInscribedCircleRadius;
	}
}

#endif //_MY_CODE_INTERSECTION_CUBE_H_