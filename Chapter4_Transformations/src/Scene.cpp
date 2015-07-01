#include "Scene.h"
#include "GL/freeglut.h"
#include "MousePole.h"
#include "MathUtil.h"
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
		, mRectangles()
		, mCubes()
		, mRectangleCollider()
		, mCubeCollider()
	{
		mInstance = this;

		InitScene2D();
		InitScene3D();

		ConfigureOpenGL();
		ConfigureInput();
	}

	Scene::~Scene()
	{}

	void Scene::InitScene2D()
	{
		InitRectangles();
		InitColliderForPolygons2D();
	}

	void Scene::InitRectangles()
	{
		const glm::vec3 vectorToA{ -0.5f, 0.0f, 0.5f };
		const glm::vec3 vectorToB{ 0.5f, 0.0f, 0.5f };
		const glm::vec3 vectorToC{ 0.5f, 0.0f, -0.5f };
		const glm::vec3 vectorToD{ -0.5f, 0.0f, -0.5f };

		const int cubesCount = 6;
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
				mRectangles.emplace_back("UnitCube.xml", boundingRectangle, mRectangleCollider);
			}
			cubesRemaining -= count;
		}

		/*const glm::vec3 center1{ 0.0f, 0.51f, 0.0f };
		const Rectangle boundingRectangle1{ center1, vectorToA, vectorToB, vectorToC, vectorToD };
		mRectangles.emplace_back("UnitCube.xml", boundingRectangle1, mColisionionHelper);

		const glm::vec3 center2{ -2.0f, 0.51f, 0.0f };
		const Rectangle boundingRectangle2{ center2, vectorToA, vectorToB, vectorToC, vectorToD };
		mRectangles.emplace_back("UnitCube.xml", boundingRectangle2, mColisionionHelper);*/
	}

	void Scene::InitColliderForPolygons2D()
	{
		for (const auto& rectangle: mRectangles)
		{
			mRectangleCollider.AddRectangle(rectangle.mControl.GetBoundingBox());
		}
	}

	void Scene::InitScene3D()
	{
		InitCubes();
		InitColliderForPolygons3D();
	}

	void Scene::InitCubes()
	{
		const glm::vec3 bottomA{ -0.5f, -0.5f, 0.5f };
		const glm::vec3 bottomB{ 0.5f, -0.5f, 0.5f };
		const glm::vec3 bottomC{ 0.5f, -0.5f, -0.5f };
		const glm::vec3 bottomD{ -0.5f, -0.5f, -0.5f };
		const glm::vec3 topA{ -0.5f, 0.5f, 0.5f };
		const glm::vec3 topB{ 0.5f, 0.5f, 0.5f };
		const glm::vec3 topC{ 0.5f, 0.5f, -0.5f };
		const glm::vec3 topD{ -0.5f, 0.5f, -0.5f };

		const std::vector<glm::vec3> cubeVertices{
			bottomA, bottomB, bottomC, bottomD,
			topA, topB, topC, topD
		};

		const float startX = 0.0f;
		const float incX = 2.0f;
		const int cubesCount = 2;
		for (int i = 0; i < cubesCount; ++i)
		{
			const float x = startX + i * incX;
			const glm::vec3 center{ x, 0.51f, 0.0f };
			const Cube bounding{ center, cubeVertices };
			mCubes.emplace_back("UnitCube.xml", bounding, mCubeCollider);
		}
	}

	void Scene::InitColliderForPolygons3D()
	{
		for (const auto& cube : mCubes)
		{
			mCubeCollider.AddCube(cube.mControl.GetBoundingBox());
		}
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
		RenderRectangles(modelToCameraTransform);
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

	void Scene::RenderRectangles(glutil::MatrixStack& modelMatrix)
	{
		for (auto& rectangle: mRectangles)
		{
			RenderMesh(modelMatrix, rectangle);
		}
	}

	void Scene::RenderCubes(glutil::MatrixStack& modelMatrix)
	{
		for (auto& cube: mCubes)
		{
			RenderMesh(modelMatrix, cube);
		}
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
		UpdateControls();
		
		UploadCameraToClipToOpenGL();
	}

	void Scene::UpdateControls()
	{
		for (auto& rectangle : mRectangles)
		{
			UpdateControl(rectangle);
		}
		for (auto& cube : mCubes)
		{
			UpdateControl(cube);
		}
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
		bool isHandled = ForwardKeyboardInput(key);
		if (isHandled == false)
		{
			gViewPole.CharPress(key);
		}
	}

	void Scene::OnMouseClick(int button, int state, int x, int y)
	{
		bool isHandled = ForwardMouseClick(button, state, x, y);
		if (isHandled == false)
		{
			Framework::ForwardMouseButton(gViewPole, button, state, x, y);
		}
	}
	
	void Scene::OnMouseMoved(int x, int y)
	{
		bool isHandled = ForwardMouseMove(x, y);
		if (isHandled == false)
		{
			Framework::ForwardMouseMotion(gViewPole, x, y);
		}
	}

	bool Scene::ForwardKeyboardInput(unsigned char key)
	{
		return ForwardKeybardInput(key, mRectangles) ||
			ForwardKeybardInput(key, mCubes);
	}

	bool Scene::ForwardMouseClick(int button, int state, int x, int y)
	{
		return ForwardMouseClick(button, state, x, y, mRectangles) ||
			ForwardMouseClick(button, state, x, y, mCubes);
	}

	bool Scene::ForwardMouseMove(int x, int y)
	{
		return ForwardMouseMove(x, y, mRectangles) ||
			ForwardMouseMove(x, y, mCubes);
	}
}