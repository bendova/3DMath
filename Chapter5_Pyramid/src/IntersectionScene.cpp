#include "IntersectionScene.h"
#include "GL/freeglut.h"
#include "MousePole.h"
#include "glutil/MatrixStack.h"

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
			IntersectionScene* scene = IntersectionScene::GetInstance();
			if (scene)
			{
				scene->OnMouseClick(button, state, x, y);
			}
		}

		void onMouseMoved(int x, int y)
		{
			IntersectionScene* scene = IntersectionScene::GetInstance();
			if (scene)
			{
				scene->OnMouseMoved(x, y);
			}
		}

		void onMouseWheel(int wheel, int direction, int x, int y)
		{
			Framework::ForwardMouseWheel(gViewPole, wheel, direction, x, y);
		}
	}

	IntersectionScene* IntersectionScene::mInstance = NULL;
	IntersectionScene::IntersectionScene()
		: mPosColorProgram("PosColor.vert", "PosColor.frag")
		, dPlaneMesh("LargePlane.xml")
		, dCubeMesh("UnitCube.xml")
		, dTrianglePainter(mPosColorProgram)
		, mScreenWidth(0)
		, mScreenHeight(0)
		, mCameraToClipMatrix()
		, mDrawLinesMode(false)
	{
		mInstance = this;
		ConfigureOpenGL();
		ConfigureInput();
	}

	IntersectionScene::~IntersectionScene()
	{
	}

	void IntersectionScene::ConfigureOpenGL()
	{
		glClearColor(0.529411f, 0.807843f, 0.980392f, 1.0f);//sky blue
		glClearDepth(1.0f);

		//glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);

		glEnable(GL_LINE_SMOOTH);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void IntersectionScene::ConfigureInput()
	{
		glutMouseFunc(onMouseClick);
		glutMotionFunc(onMouseMoved);
		glutMouseWheelFunc(onMouseWheel);
	}

	void IntersectionScene::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glutil::MatrixStack modelToCameraTransform;
		modelToCameraTransform.ApplyMatrix(gViewPole.CalcMatrix());

		glUseProgram(mPosColorProgram.GetProgramID());

		RenderPlane(modelToCameraTransform);
		RenderCube(modelToCameraTransform);
		RenderTrianglePainter(modelToCameraTransform);

		//TestPointProjection(modelToCameraTransform.Top());

		glUseProgram(GL_NONE);

		glutSwapBuffers();
		glutPostRedisplay();
	}

	void IntersectionScene::TestPointProjection(const glm::mat4& modelToCamera)
	{
		glm::vec4 worldPoint(3.0f, 0.0f, -3.0f, 1.0f);
		glm::vec4 cameraPoint = modelToCamera * worldPoint;
		glm::vec4 clipPoint = mCameraToClipMatrix * cameraPoint;
		glm::vec4 ndcPoint = clipPoint / clipPoint.w;

		const float halfScreenWidth = mScreenWidth / 2.0f;
		const float halfScreenHeight = mScreenHeight / 2.0f;
		const float screenPointX = static_cast<float>((ndcPoint.x * halfScreenWidth) + halfScreenWidth);
		const float screenPointY = static_cast<float>(halfScreenHeight - (ndcPoint.y * halfScreenHeight));
	}

	void IntersectionScene::RenderPlane(glutil::MatrixStack& modelMatrix)
	{
		glutil::PushStack push(modelMatrix);

		glUniformMatrix4fv(mPosColorProgram.GetModelToCameraTransformUniform(),
			1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		dPlaneMesh.Render();
	}

	void IntersectionScene::RenderCube(glutil::MatrixStack& modelMatrix)
	{
		glutil::PushStack push(modelMatrix);

		modelMatrix.Scale(2.0f);
		modelMatrix.Translate(glm::vec3(2.0f, 0.51f, 2.0f));
		glUniformMatrix4fv(mPosColorProgram.GetModelToCameraTransformUniform(),
			1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		dCubeMesh.Render();
	}

	void IntersectionScene::RenderTrianglePainter(glutil::MatrixStack& modelMatrix)
	{
		glutil::PushStack push(modelMatrix);
		glUniformMatrix4fv(mPosColorProgram.GetModelToCameraTransformUniform(),
			1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		dTrianglePainter.Render(modelMatrix.Top());
	}

	void IntersectionScene::Reshape(GLint width, GLint height)
	{
		mScreenWidth = width;
		mScreenHeight = height;

		UpdateCameraToClipMatrix();
		UploadCameraToClipToOpenGL();
		UpdateTrianglePainter();
	}

	void IntersectionScene::UpdateCameraToClipMatrix()
	{
		glutil::MatrixStack cameraToClipTransform;
		const float aspectRatio = static_cast<float>(mScreenWidth) / static_cast<float>(mScreenHeight);
		cameraToClipTransform.Perspective(45.0f, aspectRatio, 0.1f, 1000.0f);

		mCameraToClipMatrix = cameraToClipTransform.Top();
	}
	void IntersectionScene::UploadCameraToClipToOpenGL()
	{
		glUseProgram(mPosColorProgram.GetProgramID());
		glUniformMatrix4fv(mPosColorProgram.GetCameraToClipTransformUniform(),
			1, GL_FALSE, glm::value_ptr(mCameraToClipMatrix));
		glUseProgram(GL_NONE);
	}
	void IntersectionScene::UpdateTrianglePainter()
	{
		dTrianglePainter.SetCameraToClipMatrixInverse(glm::inverse(mCameraToClipMatrix));
		dTrianglePainter.SetScreenDimensions(mScreenWidth, mScreenHeight);
	}

	void IntersectionScene::HandleInput(unsigned char key, int x, int y)
	{
		gViewPole.CharPress(key);
	}

	void IntersectionScene::OnMouseClick(int button, int state, int x, int y)
	{
		if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
		{
			mDrawLinesMode = !mDrawLinesMode;
		}
		else
		{
			bool isHandled = false;
			if (mDrawLinesMode)
			{
				isHandled = dTrianglePainter.OnMouseClick(button, state, x, y);
			}

			if (isHandled == false)
			{
				Framework::ForwardMouseButton(gViewPole, button, state, x, y);
			}
		}
	}
	
	void IntersectionScene::OnMouseMoved(int x, int y)
	{
		bool isHandled = false;
		if (mDrawLinesMode)
		{
			isHandled = dTrianglePainter.OnMouseMoved(x, y);
		}

		if (isHandled == false)
		{
			Framework::ForwardMouseMotion(gViewPole, x, y);
		}
	}
}