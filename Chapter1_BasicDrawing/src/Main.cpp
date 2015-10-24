#include <glload/gl_3_3.h>
#include <GL/freeglut.h>
#include <memory>

#include "Scene.h"

std::unique_ptr<MyCode::Scene> g_scene;

void init()
{
	g_scene = std::make_unique<MyCode::Scene>();
}

void display()
{
	g_scene->Render();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	g_scene->Resize(w, h);
}

void keyboard(unsigned char key, int x, int y)
{
	g_scene->HandleKeyboard(key);
}

unsigned int defaults(unsigned int displayMode, int& width, int& height)
{
	width = 1280;
	height = 720;
	return displayMode;
}