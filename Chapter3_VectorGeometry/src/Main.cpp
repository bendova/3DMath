#include <glload/gl_3_3.h>
#include "GL/freeglut.h"
#include "IntersectionScene.h"
#include <memory>

std::unique_ptr<MyCode::IntersectionScene> gSnowflake;

void init()
{
	gSnowflake = std::unique_ptr<MyCode::IntersectionScene>(new MyCode::IntersectionScene());
}

void display()
{
	gSnowflake->Render();
}

void reshape(int w, int h)
{
	w = 979;
	h = 579;
	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
	gSnowflake->Reshape(static_cast<GLint>(w), static_cast<GLint>(h));
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		break;
	default:
		gSnowflake->HandleInput(key, x, y);
		break;
	}
}

unsigned int defaults(unsigned int displayMode, int &width, int &height)
{
	width = 1000;
	height = 600;
	return displayMode;
}