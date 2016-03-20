#include "Scene.h"
#include "GL/freeglut.h"
#include "MousePole.h"
#include "glutil/MatrixStack.h"
#include <algorithm>

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
			Scene* scene = Scene::GetInstance();
			if (scene)
			{
				scene->OnMouseClick(button, state, x, y);
			}
		}

		void onMouseMoved(int x, int y)
		{
			Scene* scene = Scene::GetInstance();
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

	Scene* Scene::mInstance = NULL;
	Scene::Scene()
		: mPosColorProgram("PosColor.vert", "PosColor.frag")
		, dPlaneMesh("LargePlane.xml")
		, dCubeMesh("UnitCube.xml")
		, dPyramidMesh("UnitPyramid.xml")
		, mScreenWidth(0)
		, mScreenHeight(0)
		, mPyramidWidth(1.0f)
		, mPyramidHeight(1.0f)
		, mCameraToClipMatrix()
		, mDrawLinesMode(false)
	{
		mInstance = this;
		ConfigureOpenGL();
		ConfigureInput();
	}

	Scene::~Scene()
	{
	}

	void Scene::ConfigureOpenGL()
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

	void Scene::ConfigureInput()
	{
		glutMouseFunc(onMouseClick);
		glutMotionFunc(onMouseMoved);
		glutMouseWheelFunc(onMouseWheel);
	}

	void Scene::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glutil::MatrixStack modelToCameraTransform;
		modelToCameraTransform.ApplyMatrix(gViewPole.CalcMatrix());

		glUseProgram(mPosColorProgram.GetProgramID());

		RenderPlane(modelToCameraTransform);
		RenderCube(modelToCameraTransform);
		RenderPyramid(modelToCameraTransform);

		glUseProgram(GL_NONE);

		glutSwapBuffers();
		glutPostRedisplay();
	}

	void Scene::RenderPlane(glutil::MatrixStack& modelMatrix)
	{
		glutil::PushStack push(modelMatrix);

		glUniformMatrix4fv(mPosColorProgram.GetModelToCameraTransformUniform(),
			1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		dPlaneMesh.Render();
	}

	void Scene::RenderCube(glutil::MatrixStack& modelMatrix)
	{
		glutil::PushStack push(modelMatrix);

		float cubeSide = (mPyramidWidth * mPyramidHeight) / (mPyramidWidth + mPyramidHeight);
		modelMatrix.Scale(cubeSide);
		modelMatrix.Translate(glm::vec3(0.0f, 0.51f, 0.0f));
		glUniformMatrix4fv(mPosColorProgram.GetModelToCameraTransformUniform(),
			1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		dCubeMesh.Render();
	}


	void Scene::RenderPyramid(glutil::MatrixStack& modelMatrix)
	{
		glutil::PushStack push(modelMatrix);

		modelMatrix.Scale(glm::vec3(mPyramidWidth, mPyramidHeight, mPyramidWidth));
		modelMatrix.Translate(glm::vec3(0.0f, 0.51f, 0.0f));
		glUniformMatrix4fv(mPosColorProgram.GetModelToCameraTransformUniform(),
			1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		dPyramidMesh.Render();
	}

	void Scene::Reshape(GLint width, GLint height)
	{
		mScreenWidth = width;
		mScreenHeight = height;

		UpdateCameraToClipMatrix();
		UploadCameraToClipToOpenGL();
	}

	void Scene::UpdateCameraToClipMatrix()
	{
		glutil::MatrixStack cameraToClipTransform;
		const float aspectRatio = static_cast<float>(mScreenWidth) / static_cast<float>(mScreenHeight);
		cameraToClipTransform.Perspective(45.0f, aspectRatio, 0.1f, 1000.0f);

		mCameraToClipMatrix = cameraToClipTransform.Top();
	}
	void Scene::UploadCameraToClipToOpenGL()
	{
		glUseProgram(mPosColorProgram.GetProgramID());
		glUniformMatrix4fv(mPosColorProgram.GetCameraToClipTransformUniform(),
			1, GL_FALSE, glm::value_ptr(mCameraToClipMatrix));
		glUseProgram(GL_NONE);
	}

	void Scene::HandleInput(unsigned char key, int x, int y)
	{
		bool isHandled = HandlePyramidInput(key);
		if (isHandled == false)
		{
			gViewPole.CharPress(key);
		}
	}

	bool Scene::HandlePyramidInput(unsigned char key)
	{
		static float SIZE_INC = 0.1f;

		bool handled = true;
		switch (key)
		{
		case 'j':
			mPyramidWidth = std::max(mPyramidWidth - SIZE_INC, 0.1f);
			break;
		case 'l':
			mPyramidWidth += SIZE_INC;
			break;
		case 'k':
			mPyramidHeight = std::max(mPyramidHeight - SIZE_INC, 0.1f);;
			break;
		case 'i':
			mPyramidHeight += SIZE_INC;
			break;
		default:
			handled = false;
			break;
		}
		return handled;
	}

	void Scene::OnMouseClick(int button, int state, int x, int y)
	{
		Framework::ForwardMouseButton(gViewPole, button, state, x, y);
	}
	
	void Scene::OnMouseMoved(int x, int y)
	{
		Framework::ForwardMouseMotion(gViewPole, x, y);
	}
}