#include "Scene.h"
#include "GL/freeglut.h"
#include "../framework/MousePole.h"
#include "../framework/MathUtil.h"
#include "intersection2d/UT_RectangleColider.h"
#include "intersection3d/UT_CubeCollider.h"
#include "intersection3d/UT_PolygonCollider.h"
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

	const float mNearZ = 0.1f;
	const float mFarZ = 1000.0f;

	Scene* Scene::mInstance = NULL;
	Scene::Scene()
		: mPosColorProgram("PosColor.vert", "PosColor.frag")
		, dPlaneMesh("LargePlane.xml")
		, mScreenWidth(0)
		, mScreenHeight(0)
		, mCameraToClipMatrix()
		, mCubes()
		, mColisionionHelper()
		, mCubeSideLength(1.0f)
	{
		mInstance = this;
		InitCubes();
		InitColliderForPolygons2D();
		InitCubeCollider();

		ConfigureOpenGL();
		ConfigureInput();
	}

	Scene::~Scene()
	{}

	void Scene::InitCubes()
	{
		const glm::vec3 vectorToA{ -0.5f, 0.0f, 0.5f };
		const glm::vec3 vectorToB{ 0.5f, 0.0f, 0.5f };
		const glm::vec3 vectorToC{ 0.5f, 0.0f, -0.5f };
		const glm::vec3 vectorToD{ -0.5f, 0.0f, -0.5f };

		const int cubesCount = 30;
		const int cubesPerLine = 6;
		const int linesOfCubes = static_cast<int>(std::ceil(static_cast<float>(cubesCount) / cubesPerLine));
		const float startX = -4.0f;
		const float startZ = -6.0f;
		const float incX = 2.0f;
		const float incY = 2.0f;

		int cubesRemaining = cubesCount;
		for (int i = 0; i < linesOfCubes; ++i)
		{
			const int count = std::min(cubesPerLine, cubesRemaining);
			for (int j = 0; j < count; ++j)
			{
				const float x = startX + j * incX;
				const float z = startZ + i * incY;
				const glm::vec3 center{ x, 0.51f, z };
				const Rectangle boundingRectangle{ center, vectorToA, vectorToB, vectorToC, vectorToD };
				mCubes.emplace_back("UnitCube.xml", boundingRectangle, mColisionionHelper);
			}
			cubesRemaining -= count;
		}

		/*const glm::vec3 center1{ 0.0f, 0.51f, 0.0f };
		const Rectangle boundingRectangle1{ center1, vectorToA, vectorToB, vectorToC, vectorToD };
		mCubes.emplace_back("UnitCube.xml", boundingRectangle1, mColisionionHelper);

		const glm::vec3 center2{ -2.0f, 0.51f, 0.0f };
		const Rectangle boundingRectangle2{ center2, vectorToA, vectorToB, vectorToC, vectorToD };
		mCubes.emplace_back("UnitCube.xml", boundingRectangle2, mColisionionHelper);*/
	}

	void Scene::InitColliderForPolygons2D()
	{
		ValidateColliderForPolygons2D();
		for (const auto& cube: mCubes)
		{
			mColisionionHelper.AddRectangle(cube.mCubeControl.GetBoundingBox());
		}
	}

	void Scene::ValidateColliderForPolygons2D()
	{
		UT_RectangleColider squareColider;
		assert(squareColider.Validate());
	}


	void Scene::InitCubeCollider()
	{
		ValidateColliderForPolygons3D();
		ValidateCubeCollider();
	}

	void Scene::ValidateColliderForPolygons3D()
	{
		UT_PolygonCollider collider;
		assert(collider.Validate());
	}

	void Scene::ValidateCubeCollider()
	{
		UT_CubeCollider cubeCollider;
		assert(cubeCollider.Validate());
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
		modelToCameraTransform.ApplyMatrix(gViewPole.CalcMatrix());
		
		glUseProgram(mPosColorProgram.GetProgramID());

		RenderPlane(modelToCameraTransform);
		RenderCubes(modelToCameraTransform);

		glUseProgram(GL_NONE);

		glutSwapBuffers();
		glutPostRedisplay();
	}

	void Scene::RenderPlane(glutil::MatrixStack& modelMatrix)
	{
		glutil::PushStack push(modelMatrix);

		//modelMatrix.Scale(glm::vec3{ 1.0f / 2.0f, 0.0f, 1.0f / 2.0f });
		glUniformMatrix4fv(mPosColorProgram.GetModelToCameraTransformUniform(),
			1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		dPlaneMesh.Render();
	}

	void Scene::RenderCubes(glutil::MatrixStack& modelMatrix)
	{
		for (auto& cube: mCubes)
		{
			RenderCube(modelMatrix, cube);
		}
	}

	void Scene::RenderCube(glutil::MatrixStack& modelMatrix, ControlledCube& cube)
	{
		glutil::PushStack push(modelMatrix);

		cube.mCubeControl.SetWorldToCameraTransfrom(modelMatrix.Top());

		modelMatrix.Translate(cube.mCubeControl.GetPosition());
		glUniformMatrix4fv(mPosColorProgram.GetModelToCameraTransformUniform(),
			1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		cube.mCubeMesh.Render();
	}

	void Scene::Reshape(GLint width, GLint height)
	{
		mScreenWidth = width;
		mScreenHeight = height;

		UpdateCameraToClipMatrix();
	}

	void Scene::UpdateCameraToClipMatrix()
	{
		const float aspectRatio = static_cast<float>(mScreenWidth) / static_cast<float>(mScreenHeight);
		glutil::MatrixStack cameraToClipTransform;
		cameraToClipTransform.Perspective(45.0f, aspectRatio, mNearZ, mFarZ);
		
		mCameraToClipMatrix = cameraToClipTransform.Top();
		UpdateCubeControls();
		
		UploadCameraToClipToOpenGL();
	}

	void Scene::UpdateCubeControls()
	{
		for (auto& cube : mCubes)
		{
			UpdateCubeControl(cube);
		}
	}

	void Scene::UpdateCubeControl(ControlledCube& cube)
	{
		cube.mCubeControl.SetCameraToClipTransfrom(mCameraToClipMatrix);
		cube.mCubeControl.SetClipNearZ(mNearZ);
		cube.mCubeControl.SetScreenDimensions(mScreenWidth, mScreenHeight);
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
		bool isHandled = ForwardKeyboardInputToCubes(key);
		if (isHandled == false)
		{
			gViewPole.CharPress(key);
		}
	}

	void Scene::OnMouseClick(int button, int state, int x, int y)
	{
		bool isHandled = ForwardMouseClickToCubes(button, state, x, y);
		if (isHandled == false)
		{
			Framework::ForwardMouseButton(gViewPole, button, state, x, y);
		}
	}
	
	void Scene::OnMouseMoved(int x, int y)
	{
		bool isHandled = ForwardMouseMoveToCubes(x, y);
		if (isHandled == false)
		{
			Framework::ForwardMouseMotion(gViewPole, x, y);
		}
	}

	bool Scene::ForwardKeyboardInputToCubes(unsigned char key)
	{
		bool isHandled = false;
		for (auto& c : mCubes)
		{
			isHandled = c.mCubeControl.HandleKeyPress(key);
			if (isHandled)
			{
				break;
			}
		}

		return isHandled;
	}

	bool Scene::ForwardMouseClickToCubes(int button, int state, int x, int y)
	{
		bool isHandled = false;
		for (auto& c: mCubes)
		{
			isHandled = c.mCubeControl.HandleMouseClick(button, state, x, y);
			if (isHandled)
			{
				break;
			}
		}

		return isHandled;
	}

	bool Scene::ForwardMouseMoveToCubes(int x, int y)
	{
		bool isHandled = false;
		for (auto& c : mCubes)
		{
			isHandled = c.mCubeControl.HandleMouseMoved(x, y);
			if (isHandled)
			{
				break;
			}
		}

		return isHandled;
	}
}