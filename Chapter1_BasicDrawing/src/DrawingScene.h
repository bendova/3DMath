#ifndef _MY_CODE_DRAWING_SCENE_H_
#define _MY_CODE_DRAWING_SCENE_H_

#include <memory>

#include "glload/gl_3_3.h"
#include <glm/glm.hpp>

namespace glutil
{
	class MatrixStack;
}

namespace MyCode
{
	class Mesh;
	class PosColorProgram;

	class DrawingScene
	{
	public:
		DrawingScene();
		~DrawingScene();

		void Render();
		void Resize(const int w, const int h);
		void HandleInput(unsigned char key, int x, int y);
	private:
		using MeshPtr = std::unique_ptr < Mesh > ;
		using ProgramPtr = std::unique_ptr < PosColorProgram > ;

		void ConfigureOpenGL();
		void ConfigureInput();
		void ApplyPerspectiveTransform(glutil::MatrixStack& matrixStack, const int width, const int height);
		void UploadCameraToClip(const glm::mat4& cameraToClip);
		void UploadModelToCamera(const GLuint modelToCameraUniform);
		void DrawCubeFromMesh();
		void DrawCubeFromData();
		void InitCubeData();
		void InitVertexBuffer();
		void InitIndexBuffer();
		void InitVertexArrayObject();
	private:

		MeshPtr m_Cube;
		ProgramPtr m_Program;
		GLuint m_vertexArrayBuffer;
		GLuint m_indexArrayBuffer;
		GLuint m_vertexArrayObject;
	};
}

#endif //_MY_CODE_DRAWING_SCENE_H_