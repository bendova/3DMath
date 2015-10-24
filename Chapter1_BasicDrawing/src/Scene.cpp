#include "Scene.h"

#include <GL/freeglut.h>
#include <glutil/glutil.h>
#include "MyMesh.h"
#include "MousePole.h"

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

	const float LEFT_EXTENT = -1.0f;
	const float RIGHT_EXTENT = 1.0f;
	const float FRONT_EXTENT = 1.0f;
	const float BACK_EXTENT = -1.0f;
	const float TOP_EXTENT = 1.0f;
	const float BOTTOM_EXTENT = -1.0f;
	
#define LEFT_TOP_FRONT LEFT_EXTENT, TOP_EXTENT, FRONT_EXTENT
#define RIGHT_TOP_FRONT RIGHT_EXTENT, TOP_EXTENT, FRONT_EXTENT
#define LEFT_BOTTOM_FRONT LEFT_EXTENT, BOTTOM_EXTENT, FRONT_EXTENT
#define RIGHT_BOTTOM_FRONT RIGHT_EXTENT, BOTTOM_EXTENT, FRONT_EXTENT
#define LEFT_TOP_BACK LEFT_EXTENT, TOP_EXTENT, BACK_EXTENT
#define RIGHT_TOP_BACK RIGHT_EXTENT, TOP_EXTENT, BACK_EXTENT
#define LEFT_BOTTOM_BACK LEFT_EXTENT, BOTTOM_EXTENT, BACK_EXTENT
#define RIGHT_BOTTOM_BACK RIGHT_EXTENT, BOTTOM_EXTENT, BACK_EXTENT

	const GLint VERTEX_COMPONENT_COUNT = 3;
	const float g_vertexData[] = // CW ordering
	{
		// front face
		LEFT_TOP_FRONT, RIGHT_TOP_FRONT, RIGHT_BOTTOM_FRONT, LEFT_BOTTOM_FRONT,
		// back face
		RIGHT_TOP_BACK, LEFT_TOP_BACK, LEFT_BOTTOM_BACK, RIGHT_BOTTOM_BACK,
		// left face
		LEFT_TOP_BACK, LEFT_TOP_FRONT, LEFT_BOTTOM_FRONT, LEFT_BOTTOM_BACK,
		// right face
		RIGHT_TOP_FRONT, RIGHT_TOP_BACK, RIGHT_BOTTOM_BACK, RIGHT_BOTTOM_FRONT,
		// top face
		LEFT_TOP_BACK, RIGHT_TOP_BACK, RIGHT_TOP_FRONT, LEFT_TOP_FRONT,
		// bottom face
		LEFT_BOTTOM_FRONT, RIGHT_BOTTOM_FRONT, RIGHT_BOTTOM_BACK, LEFT_BOTTOM_BACK,
	};

#define COLOR_RED 1.0f, 0.0f, 0.0f, 1.0f
#define COLOR_GREEN 0.0f, 1.0f, 0.0f, 1.0f
#define COLOR_BLUE 0.0f, 0.0f, 1.0f, 1.0f
#define COLOR_YELLOW 1.0f, 1.0f, 0.0f, 1.0f
#define COLOR_CYAN 0.0f, 1.0f, 1.0f, 1.0f
#define COLOR_MAGENTA 1.0f, 0.0f, 1.0f, 1.0f

	const GLint COLOR_COMPONENT_COUNT = 4;
	const float g_colorData[] =
	{
		// front face
		COLOR_RED, COLOR_RED, COLOR_RED, COLOR_RED,
		// back face
		COLOR_GREEN, COLOR_GREEN, COLOR_GREEN, COLOR_GREEN,
		// left face
		COLOR_BLUE, COLOR_BLUE, COLOR_BLUE, COLOR_BLUE,
		// right face
		COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW,
		// top face
		COLOR_CYAN, COLOR_CYAN, COLOR_CYAN, COLOR_CYAN,
		// bottom face
		COLOR_MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA,
	};

	const GLshort g_indexData[] =
	{
		// Vertex indices CW
		// front face
		0, 1, 2,
		2, 3, 0,
		// back face
		4, 5, 6,
		6, 7, 4,
		// left face
		8, 9, 10,
		10, 11, 8,
		// right face
		12, 13, 14,
		14, 15, 12,
		// top face
		16, 17, 18,
		18, 19, 16,
		// bottom face
		20, 21, 22,
		22, 23, 20,
	};
#define ARRAY_SIZE(arr) sizeof(arr) / sizeof((arr)[0])

	Scene::Scene()
		: m_cube(new Mesh("UnitCube.xml"))
		, m_program(new PosColorProgram("PosColor.vert", "PosColor.frag"))
		, m_vertexBuffer(GL_INVALID_INDEX)
		, m_indicesBuffer(GL_INVALID_INDEX)
		, m_vertexArrayObject(GL_INVALID_INDEX)
	{
		ConfigureOpenGL();
		ConfigureInput();
		InitCubeData();
	}
	Scene::~Scene()
	{

	}

	void Scene::ConfigureOpenGL()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);

		glEnable(GL_DEPTH_TEST);
		glDepthRange(0.0f, 1.0f);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
	}

	void Scene::ConfigureInput()
	{
		glutMouseFunc(onMouseClick);
		glutMotionFunc(onMouseMoved);
		glutMouseWheelFunc(onMouseWheel);
	}

	void Scene::InitCubeData()
	{
		InitVertexBuffer();
		InitIndicesBuffer();
		InitVertexArrayObject();
	}

	void Scene::InitVertexBuffer()
	{
		glGenBuffers(1, &m_vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

		const GLsizeiptr totalSize = sizeof(g_vertexData) + sizeof(g_colorData);
		glBufferData(GL_ARRAY_BUFFER, totalSize, NULL, GL_STATIC_DRAW);

		GLintptr vertexDataOffset = 0;
		GLsizeiptr vertexDataSize = sizeof(g_vertexData);
		glBufferSubData(GL_ARRAY_BUFFER, vertexDataOffset, vertexDataSize, g_vertexData);

		GLintptr colorDataOffset = sizeof(g_vertexData);
		GLsizeiptr colorDataSize = sizeof(g_colorData);
		glBufferSubData(GL_ARRAY_BUFFER, colorDataOffset, colorDataSize, g_colorData);

		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	}
	void Scene::InitIndicesBuffer()
	{
		glGenBuffers(1, &m_indicesBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indexData), g_indexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
	}
	void Scene::InitVertexArrayObject()
	{
		glGenVertexArrays(1, &m_vertexArrayObject);
		glBindVertexArray(m_vertexArrayObject);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);

		const GLsizei stride = 0;
		glEnableVertexAttribArray(m_program->GetPositionAttribute());
		glVertexAttribPointer(m_program->GetPositionAttribute(), VERTEX_COMPONENT_COUNT, GL_FLOAT, GL_FALSE, stride, 0);

		const int offset = sizeof(g_vertexData);
		glEnableVertexAttribArray(m_program->GetColorAttribute());
		glVertexAttribPointer(m_program->GetColorAttribute(), COLOR_COMPONENT_COUNT, GL_FLOAT, GL_FALSE, stride, (void*)offset);

		glBindVertexArray(GL_NONE);
	}

	void Scene::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(m_program->GetProgramID());
		UpdateModelToCamera();
		//m_cube->Render("color");
		
		glBindVertexArray(m_vertexArrayObject);
		glDrawElements(GL_TRIANGLES, ARRAY_SIZE(g_indexData), GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(GL_NONE);
		
		glUseProgram(GL_NONE);

		glutSwapBuffers();
	}

	void Scene::UpdateModelToCamera()
	{
		glutil::MatrixStack matrixStack;
		matrixStack.ApplyMatrix(gViewPole.CalcMatrix());
		glUniformMatrix4fv(m_program->GetModelToCameraUniform(), 1, GL_FALSE, glm::value_ptr(matrixStack.Top()));
	}

	void Scene::Resize(const int width, const int height)
	{
		glutil::MatrixStack matrixStack;
		const float fov = 45.0f;
		const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		const float zNear = 0.1f;
		const float zFar = 1000.0f;
		matrixStack.Perspective(fov, aspectRatio, zNear, zFar);

		glUseProgram(m_program->GetProgramID());
		glUniformMatrix4fv(m_program->GetCameraToClipUniform(), 1, GL_FALSE, glm::value_ptr(matrixStack.Top()));
		glUseProgram(GL_NONE);

		glutPostRedisplay();
	}
	
	void Scene::HandleKeyboard(const unsigned char key)
	{
		switch (key)
		{
		case 27: //ESC key
			glutLeaveMainLoop();
			break;
		default:
			gViewPole.CharPress(key);
			glutPostRedisplay();
			break;
		}
	}
}
