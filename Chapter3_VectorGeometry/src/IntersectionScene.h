#ifndef _SCENE_LIGHTING_PROGRAM_H_
#define _SCENE_LIGHTING_PROGRAM_H_

#include <memory>
#include <glload/gl_3_3.h>
#include <glm/glm.hpp>
#include "Scene.h"
#include "programs/PosColorProgram.h"
#include "MyMesh.h"
#include "renderables/TrianglePainter.h"

namespace glutil
{
	class MatrixStack;
}

namespace MyCode
{
	class IntersectionScene
	{
	public:
		IntersectionScene();
		~IntersectionScene();

		void Render();
		void Reshape(GLint width, GLint height);

		void HandleInput(unsigned char key, int x, int y);
		void OnMouseClick(int button, int state, int x, int y);
		void OnMouseMoved(int x, int y);

		static IntersectionScene* GetInstance() { return mInstance; }
	private:
		void RenderPlane(glutil::MatrixStack& modelMatrix);
		void RenderCube(glutil::MatrixStack& modelMatrix);
		void RenderTrianglePainter(glutil::MatrixStack& modelMatrix);

		void TestPointProjection(const glm::mat4& modelToCamera);

		void ConfigureOpenGL();
		void ConfigureInput();

		void UpdateCameraToClipMatrix();
		void UploadCameraToClipToOpenGL();
		void UpdateTrianglePainter();

		static IntersectionScene* mInstance;
		PosColorProgram mPosColorProgram;
		Mesh dPlaneMesh;
		Mesh dCubeMesh;
		TrianglePainter dTrianglePainter;
		GLint mScreenWidth;
		GLint mScreenHeight;
		glm::mat4 mCameraToClipMatrix;
		bool mDrawLinesMode;
	};
}

#endif