#ifndef _MY_CODE_INTERSECTION_UT_CUBE_COLLIDER_H_
#define _MY_CODE_INTERSECTION_UT_CUBE_COLLIDER_H_

#include "CubeCollider.h"

namespace MyCode
{
	class UT_CubeCollider
	{
	public:
		bool Validate();

	private:
		class Setup
		{
		public:
			Setup(const std::initializer_list<glm::vec3> centers)
				: mCubes()
				, mColider()
			{
				BuildCubes(centers);
				AddCubesToCollider();
			}

			const CubeCollider& Collider() { return mColider; }
			const std::vector<Cube>& Rectangles() { return mCubes; }

			const Cube& operator[](int index)
			{
				return mCubes[index];
			}

		private:

			void BuildCubes(const std::initializer_list<glm::vec3> centers)
			{
				const glm::vec3 bottomA{ -1.0f, -1.0f, 1.0f };
				const glm::vec3 bottomB{ 1.0f, -1.0f, 1.0f };
				const glm::vec3 bottomC{ 1.0f, -1.0f, -1.0f };
				const glm::vec3 bottomD{ -1.0f, -1.0f, -1.0f };
				const glm::vec3 topA{ -1.0f, 1.0f, 1.0f };
				const glm::vec3 topB{ 1.0f, 1.0f, 1.0f };
				const glm::vec3 topC{ 1.0f, 1.0f, -1.0f };
				const glm::vec3 topD{ -1.0f, 1.0f, -1.0f };

				const std::vector<glm::vec3> cubeVertices{
					bottomA, bottomB, bottomC, bottomD,
					topA, topB, topC, topD
				};

				for (const auto& center : centers)
				{
					mCubes.emplace_back(center, cubeVertices);
				}
			}

			void AddCubesToCollider()
			{
				for (const auto& rectangle : mCubes)
				{
					mColider.AddCube(rectangle);
				}
			}

			std::vector<Cube> mCubes;
			CubeCollider mColider;
		};


		bool CubeRepresentation();
		bool AvoidCollision();
		bool AvoidCollisionOfTouchingCubes();
		bool MoveAwayTouchingCubes();
		bool SlideCubeAlongsideAnotherCube();
	};
}

#endif //_MY_CODE_INTERSECTION_UT_CUBE_COLLIDER_H_