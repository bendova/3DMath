#include <glload/gl_3_3.h>
#include "GL/freeglut.h"
#include "DrawingScene.h"
#include <memory>

std::unique_ptr<MyCode::DrawingScene> gScene = nullptr;

void init()
{
	gScene = std::make_unique<MyCode::DrawingScene>();
}

void display()
{
	gScene->Render();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	gScene->Resize(w, h);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		break;
	default:
		gScene->HandleInput(key, x, y);
		break;
	}
}

unsigned int defaults(unsigned int displayMode, int& width, int& height)
{
	width = 1280;
	height = 720;
	return displayMode;
}