#include "Scene.h"
#include "GL/freeglut.h"
#include "../framework/MousePole.h"
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
		, mScreenWidth(0)
		, mScreenHeight(0)
		, mCameraToClipMatrix()
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
		glClearColor(0.529411f, 0.807843f, 0.980392f, 1.0f);//man. city blue :)
		glClearDepth(1.0f);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);

		glEnable(GL_LINE_SMOOTH);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
		//modelToCameraTransform.ApplyMatrix(gViewPole.CalcMatrix());
		
		modelToCameraTransform.SetIdentity();
		modelToCameraTransform.Translate(glm::vec3(2.0f, -2.0f, -4.0f));

		glUseProgram(mPosColorProgram.GetProgramID());

		RenderPlane(modelToCameraTransform);
		RenderCube(modelToCameraTransform);

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

		modelMatrix.Translate(glm::vec3(0.0f, 0.51f, 0.0f));
		glUniformMatrix4fv(mPosColorProgram.GetModelToCameraTransformUniform(),
			1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		dCubeMesh.Render();
	}

	void Scene::Reshape(GLint width, GLint height)
	{
		mScreenWidth = width;
		mScreenHeight = height;

		UpdateCameraToClipMatrix();
	}

	void Scene::UpdateCameraToClipMatrix()
	{
		/*glutil::MatrixStack cameraToClipTransform;
		const float aspectRatio = static_cast<float>(mScreenWidth) / static_cast<float>(mScreenHeight);
		cameraToClipTransform.Perspective(45.0f, aspectRatio, 0.1f, 1000.0f);
		
		mCameraToClipMatrix = cameraToClipTransform.Top();*/
		
		const float frustumScale = 1.0f;
		const float zNear = 1.0f;
		const float zFar = 200.0f;
		mCameraToClipMatrix[0].x = frustumScale;
		mCameraToClipMatrix[1].y = frustumScale;
		mCameraToClipMatrix[2].z = 1.0f;// (zNear + zFar) / (zNear - zFar);
		mCameraToClipMatrix[2].w = -1.0f;
		mCameraToClipMatrix[3].z = 0.0f;// (2 * zNear * zFar) / (zNear - zFar);

		UploadCameraToClipToOpenGL();
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
		gViewPole.CharPress(key);
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