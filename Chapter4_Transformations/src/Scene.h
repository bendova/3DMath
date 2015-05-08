#ifndef _SCENE_LIGHTING_PROGRAM_H_
#define _SCENE_LIGHTING_PROGRAM_H_

#include <memory>
#include <vector>
#include <string>
#include <glload/gl_3_3.h>
#include <glm/glm.hpp>
#include "Scene.h"
#include "programs/PosColorProgram.h"
#include "control/ControlHelper.h"
#include "intersection/IntersectionHelper.h"
#include "../framework/MyMesh.h"

namespace glutil
{
	class MatrixStack;
}

namespace MyCode
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void Render();
		void Reshape(GLint width, GLint height);

		void HandleInput(unsigned char key, int x, int y);
		void OnMouseClick(int button, int state, int x, int y);
		void OnMouseMoved(int x, int y);

		static Scene* GetInstance() { return mInstance; }
	private:
		struct ControlledCube
		{
			ControlledCube(const std::string& mesh, const glm::vec3& position, 
				const float sideLength, const IntersectionHelper& intersectionHelper)
				: mCubeMesh(mesh)
				, mCubeControl(position, sideLength, intersectionHelper)
			{}

			ControlledCube(ControlledCube&& other)
				: mCubeMesh{ std::forward<Mesh>(other.mCubeMesh) }
				, mCubeControl{ std::forward<ControlHelper>(other.mCubeControl) }
			{}

			Mesh mCubeMesh;
			ControlHelper mCubeControl;
		};
		
		void InitCubes();
		void InitIntersectionHelper();

		void RenderPlane(glutil::MatrixStack& modelMatrix);
		void RenderCubes(glutil::MatrixStack& modelMatrix);
		void RenderCube(glutil::MatrixStack& modelMatrix, ControlledCube& cube);

		void ConfigureOpenGL();
		void ConfigureInput();

		void UpdateCubeControls();
		void UpdateCubeControl(ControlledCube& cube);
		void UpdateCameraToClipMatrix();
		void UploadCameraToClipToOpenGL();

		bool ForwardKeyboardInputToCubes(unsigned char key);
		bool ForwardMouseClickToCubes(int button, int state, int x, int y);
		bool ForwardMouseMoveToCubes(int x, int y);

		static Scene* mInstance;
		PosColorProgram mPosColorProgram;
		Mesh dPlaneMesh;
		GLint mScreenWidth;
		GLint mScreenHeight;
		glm::mat4 mCameraToClipMatrix;
		std::vector<ControlledCube> mCubes;
		IntersectionHelper mIntersectionHelper;
		const float mCubeSideLength;
		static const float zNear;
		static const float zFar;
	};
}

#endif