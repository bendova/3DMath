#ifndef _SCENE_LIGHTING_PROGRAM_H_
#define _SCENE_LIGHTING_PROGRAM_H_

#include <memory>
#include <glload/gl_3_3.h>
#include <glm/glm.hpp>
#include "Scene.h"
#include "programs/PosColorProgram.h"
#include "../framework/MyMesh.h"
#include "renderables/LinePainter.h"

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
		void RenderLinePainter(glutil::MatrixStack& modelMatrix);

		void TestPointProjection(const glm::mat4& modelToCamera);

		void ConfigureOpenGL();
		void ConfigureInput();

		void UpdateCameraToClipMatrix();
		void UploadCameraToClipToOpenGL();
		void UpdateLinePainter();

		static IntersectionScene* mInstance;
		PosColorProgram mPosColorProgram;
		Mesh dPlaneMesh;
		Mesh dCubeMesh;
		LinePainter dLinePainter;
		GLint mScreenWidth;
		GLint mScreenHeight;
		glm::mat4 mCameraToClipMatrix;
		bool mDrawLinesMode;
	};
}

#endif