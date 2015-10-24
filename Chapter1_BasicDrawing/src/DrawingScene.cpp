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

	const float FRONT_EXTENT = 1.0f;
	const float BACK_EXTENT = -1.0f;
	const float TOP_EXTENT = 1.0f;
	const float BOTTOM_EXTENT = -1.0f;
	const float RIGHT_EXTENT = 1.0f;
	const float LEFT_EXTENT = -1.0f;
#define LEFT_TOP_FRONT LEFT_EXTENT, TOP_EXTENT, FRONT_EXTENT 
#define RIGHT_TOP_FRONT RIGHT_EXTENT, TOP_EXTENT, FRONT_EXTENT 
#define RIGHT_BOTTOM_FRONT RIGHT_EXTENT, BOTTOM_EXTENT, FRONT_EXTENT 
#define LEFT_BOTTOM_FRONT LEFT_EXTENT, BOTTOM_EXTENT, FRONT_EXTENT 
#define LEFT_TOP_BACK LEFT_EXTENT, TOP_EXTENT, BACK_EXTENT 
#define RIGHT_TOP_BACK RIGHT_EXTENT, TOP_EXTENT, BACK_EXTENT 
#define RIGHT_BOTTOM_BACK RIGHT_EXTENT, BOTTOM_EXTENT, BACK_EXTENT 
#define LEFT_BOTTOM_BACK LEFT_EXTENT, BOTTOM_EXTENT, BACK_EXTENT 

#define COLOR_RED 1.0f, 0.0f, 0.0f, 1.0f
#define COLOR_GREEN 0.0f, 1.0f, 0.0f, 1.0f
#define COLOR_BLUE 0.0f, 0.0f, 1.0f, 1.0f
#define COLOR_YELLOW 1.0f, 1.0f, 0.0f, 1.0f
#define COLOR_CYAN 0.0f, 1.0f, 1.0f, 1.0f
#define COLOR_MAGENTA 1.0f, 0.0f, 1.0f, 1.0f

	const int COMPONENT_COUNT_PER_VERTEX = 3;
	const int COMPONENT_COUNT_PER_COLOR = 4;
	const int VERTICES_COUNT = 24;
	const float g_vertexData[] = 
	{
		// Vertex data
		// front face
		LEFT_TOP_FRONT, RIGHT_TOP_FRONT, RIGHT_BOTTOM_FRONT, LEFT_BOTTOM_FRONT,
		// back face
		RIGHT_TOP_BACK, LEFT_TOP_BACK, LEFT_BOTTOM_BACK, RIGHT_BOTTOM_BACK,
		// right face
		RIGHT_TOP_FRONT, RIGHT_TOP_BACK, RIGHT_BOTTOM_BACK, RIGHT_BOTTOM_FRONT,
		// left face
		LEFT_TOP_FRONT, LEFT_BOTTOM_FRONT, LEFT_BOTTOM_BACK, LEFT_TOP_BACK,
		// top face
		LEFT_TOP_BACK, RIGHT_TOP_BACK, RIGHT_TOP_FRONT, LEFT_TOP_FRONT,
		// bottom face
		LEFT_BOTTOM_FRONT, RIGHT_BOTTOM_FRONT, RIGHT_BOTTOM_BACK, LEFT_BOTTOM_BACK, 

		// Color data
		// front face
		COLOR_RED, COLOR_RED, COLOR_RED, COLOR_RED,
		// back face
		COLOR_GREEN, COLOR_GREEN, COLOR_GREEN, COLOR_GREEN,
		// right face
		COLOR_BLUE, COLOR_BLUE, COLOR_BLUE, COLOR_BLUE,
		// left face
		COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW,
		// top face
		COLOR_CYAN, COLOR_CYAN, COLOR_CYAN, COLOR_CYAN,
		// bottom face
		COLOR_MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA,
	};

	const GLushort g_indexData[] =
	{
		// front face
		0, 1, 2,
		2, 3, 0,
		// back face
		4, 5, 6,
		6, 7, 4,
		// right face
		8, 9, 10,
		10, 11, 8,
		// left face
		12, 13, 14,
		14, 15, 12,
		// top face
		16, 17, 18,
		18, 19, 16,
		// bottom face
		20, 21, 22,
		22, 23, 20,
	};
#define ARRAY_LENGTH(array) sizeof(array) / sizeof(array[0]) 

	DrawingScene::DrawingScene()
		: m_Cube(new Mesh("UnitCube.xml"))
		, m_Program(new PosColorProgram("PosColor.vert", "PosColor.frag"))
		, m_vertexArrayBuffer(GL_INVALID_INDEX)
		, m_indexArrayBuffer(GL_INVALID_INDEX)
		, m_vertexArrayObject(GL_INVALID_INDEX)
	{
		ConfigureOpenGL();
		ConfigureInput();
		InitCubeData();
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

	void DrawingScene::InitCubeData()
	{
		InitVertexBuffer();
		InitIndexBuffer();
		InitVertexArrayObject();
	}

	void DrawingScene::InitVertexBuffer()
	{
		glGenBuffers(1, &m_vertexArrayBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertexData), g_vertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	}

	void DrawingScene::InitIndexBuffer()
	{
		glGenBuffers(1, &m_indexArrayBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexArrayBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indexData), g_indexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
	}

	void DrawingScene::InitVertexArrayObject()
	{
		glGenVertexArrays(1, &m_vertexArrayObject);
		glBindVertexArray(m_vertexArrayObject);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexArrayBuffer);

		const int stride = 0;
		glEnableVertexAttribArray(m_Program->GetPositionAttrib());
		glVertexAttribPointer(m_Program->GetPositionAttrib(), COMPONENT_COUNT_PER_VERTEX, GL_FLOAT, GL_FALSE, stride, 0);

		const int colorOffset = VERTICES_COUNT * COMPONENT_COUNT_PER_VERTEX * sizeof(g_vertexData[0]);
		glEnableVertexAttribArray(m_Program->GetColorAttrib());
		glVertexAttribPointer(m_Program->GetColorAttrib(), COMPONENT_COUNT_PER_COLOR, GL_FLOAT, GL_FALSE, stride, (void*)colorOffset );

		glBindVertexArray(GL_NONE);
	}

	void DrawingScene::Render()
	{
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		glUseProgram(m_Program->GetProgramID());
		UploadModelToCamera(m_Program->GetModelToCameraUniform());
		//DrawCubeFromMesh();
		DrawCubeFromData();
		glUseProgram(GL_NONE);

		glutSwapBuffers();
	}

	void DrawingScene::DrawCubeFromMesh()
	{
		m_Cube->Render();
	}

	void DrawingScene::DrawCubeFromData()
	{
		glBindVertexArray(m_vertexArrayObject);
		const int arrayLength = ARRAY_LENGTH(g_indexData);
		glDrawElements(GL_TRIANGLES, arrayLength, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(GL_NONE);
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