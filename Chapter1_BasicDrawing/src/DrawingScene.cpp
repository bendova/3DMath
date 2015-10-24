#include "DrawingScene.h"
#include "GL/freeglut.h"
#include <glutil/glutil.h>

#include "MousePole.h"

#include "MyMesh.h"
#include "PosColorProgram.h"

namespace MyCode
{
	glutil::ViewData gInitialViewData =
	{
		glm::vec3(0.0f, 0.5f, 0.0f),
		glm::fquat(1.0f, 0.0f, 0.0f, 0.0f),
		5.0f,
		0.0f
	};

	glutil::ViewScale gViewScale =
	{
		1.0f, 20.0f,
		1.0f, 0.1f,
		1.0f, 0.1f,
		90.0f / 250.0f
	};

	glutil::ViewPole gViewPole = glutil::ViewPole(gInitialViewData, gViewScale, glutil::MB_LEFT_BTN);

	namespace
	{
		void onMouseClick(int button, int state, int x, int y)
		{
			Framework::ForwardMouseButton(gViewPole, button, state, x, y);
			glutPostRedisplay();
		}

		void onMouseMoved(int x, int y)
		{
			Framework::ForwardMouseMotion(gViewPole, x, y);
			glutPostRedisplay();
		}

		void onMouseWheel(int wheel, int direction, int x, int y)
		{
			Framework::ForwardMouseWheel(gViewPole, wheel, direction, x, y);
			glutPostRedisplay();
		}
	}

	DrawingScene::DrawingScene()
		: m_Cube(new Mesh("UnitCube.xml"))
		, m_Program(new PosColorProgram("PosColor.vert", "PosColor.frag"))
	{
		ConfigureOpenGL();
		ConfigureInput();
	}

	DrawingScene::~DrawingScene()
	{

	}

	void DrawingScene::ConfigureOpenGL()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);
	}

	void DrawingScene::ConfigureInput()
	{
		glutMouseFunc(onMouseClick);
		glutMotionFunc(onMouseMoved);
		glutMouseWheelFunc(onMouseWheel);
	}

	void DrawingScene::Render()
	{
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		glUseProgram(m_Program->GetProgramID());
		UploadModelToCamera(m_Program->GetModelToCameraUniform());
		m_Cube->Render();
		glUseProgram(GL_NONE);

		glutSwapBuffers();
	}

	void DrawingScene::UploadModelToCamera(const GLuint modelToCameraUniform)
	{
		glutil::MatrixStack matrixStack;
		matrixStack.ApplyMatrix(gViewPole.CalcMatrix());
		const glm::mat4& modelToCamera = matrixStack.Top();
		glUniformMatrix4fv(modelToCameraUniform, 1, GL_FALSE, glm::value_ptr(modelToCamera));
	}

	void DrawingScene::Resize(const int w, const int h)
	{
		glutil::MatrixStack matrixStack;
		ApplyPerspectiveTransform(matrixStack, w, h);
		UploadCameraToClip(matrixStack.Top());
		glutPostRedisplay();
	}

	void DrawingScene::ApplyPerspectiveTransform(glutil::MatrixStack& matrixStack, const int width, const int height)
	{
		const float fov = 45.0f;
		const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		const float zNear = 0.1f;
		const float zFar = 1000.0f;
		matrixStack.Perspective(fov, aspectRatio, zNear, zFar);
	}

	void DrawingScene::UploadCameraToClip(const glm::mat4& cameraToClip)
	{
		glUseProgram(m_Program->GetProgramID());
		glUniformMatrix4fv(m_Program->GetCameraToClipUniform(), 1, GL_FALSE, glm::value_ptr(cameraToClip));
		glUseProgram(GL_NONE);
	}

	void DrawingScene::HandleInput(unsigned char key, int x, int y)
	{
		gViewPole.CharPress(key);
		glutPostRedisplay();
	}

}