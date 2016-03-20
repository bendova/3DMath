#ifndef _SCENE_LIGHTING_PROGRAM_H_
#define _SCENE_LIGHTING_PROGRAM_H_

#include <memory>
#include <glload/gl_3_3.h>
#include <glm/glm.hpp>
#include "Scene.h"
#include "programs/PosColorProgram.h"
#include "MyMesh.h"

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
		bool HandlePyramidInput(unsigned char key);
		void OnMouseClick(int button, int state, int x, int y);
		void OnMouseMoved(int x, int y);

		static Scene* GetInstance() { return mInstance; }
	private:
		void RenderPlane(glutil::MatrixStack& modelMatrix);
		void RenderCube(glutil::MatrixStack& modelMatrix);
		void RenderPyramid(glutil::MatrixStack& modelMatrix);

		void TestPointProjection(const glm::mat4& modelToCamera);

		void ConfigureOpenGL();
		void ConfigureInput();

		void UpdateCameraToClipMatrix();
		void UploadCameraToClipToOpenGL();
		void UpdateTrianglePainter();

	private:
		static Scene* mInstance;
		PosColorProgram mPosColorProgram;
		Mesh dPlaneMesh;
		Mesh dCubeMesh;
		Mesh dPyramidMesh;
		GLint mScreenWidth;
		GLint mScreenHeight;

		float mPyramidWidth;
		float mPyramidHeight;

		glm::mat4 mCameraToClipMatrix;
		bool mDrawLinesMode;
	};
}

#endif