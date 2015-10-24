#ifndef _MY_CODE_SCENE_H_
#define _MY_CODE_SCENE_H_

#include <memory>
#include <glload/gl_3_3.h>

namespace MyCode
{
	class Mesh;
	class PosColorProgram;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void Render();
		void Resize(const int width, const int height);
		void HandleKeyboard(const unsigned char key);

	private:
		using MeshPtr = std::unique_ptr<Mesh>;
		using ProgramPtr = std::unique_ptr<PosColorProgram>;

		void ConfigureOpenGL();
		void ConfigureInput();
		void InitCubeData();
		void InitVertexBuffer();
		void InitIndicesBuffer();
		void InitVertexArrayObject();

		void UpdateModelToCamera();
	private:
		MeshPtr m_cube;
		ProgramPtr m_program;
		GLuint m_vertexBuffer;
		GLuint m_indicesBuffer;
		GLuint m_vertexArrayObject;
	};
}

#endif //_MY_CODE_SCENE_H_