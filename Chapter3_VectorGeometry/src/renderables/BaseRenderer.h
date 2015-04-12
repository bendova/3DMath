#ifndef _MY_CODE_RENDERABLES_BASE_RENDERER_H
#define _MY_CODE_RENDERABLES_BASE_RENDERER_H

#include <glload/gl_3_3.h>
#include <glm/glm.hpp>
#include <vector>
#include "../framework/MathUtil.h"


namespace MyCode
{
	using namespace MathUtil;

	class PosColorProgram;

	class BaseRenderer
	{
	public:
		BaseRenderer(const PosColorProgram& posColorProgram);
		virtual ~BaseRenderer();

		void Render();

		void AddPointToRender(const glm::vec4& worldPoint, const Color& color = Color(1.0f));
		void ReplaceLastPoint(const glm::vec4& worldPoint);

		const std::vector<ColoredPoint>& GetRenderPoints() const { return mRenderPoints; }
		const std::vector<glm::vec4>& GetWorldPoints() const { return mWorldPoints; }
		void ClearPoints() 
		{ 
			mRenderPoints.clear();
			mWorldPoints.clear();
		}

	protected:
		virtual void DoRender() = 0;

	private:
		void InitVertexBuffer();
		void InitVertexArrayObject();
		void UploadPointsToOpenGL();

		void SetPoint(const glm::vec4& worldPoint, const int pointIndex);

		static const int VERTEX_BUFFER_MAX_SIZE;
		static const int VERTEX_COMPONENT_COUNT;
		static const int COLOR_COMPONENT_COUNT;
		const PosColorProgram& mPosColorProgram;
		GLuint mVertexBufferID;
		GLuint mVertexArrayObjectID;
		std::vector<glm::vec4> mWorldPoints;
		std::vector<ColoredPoint> mRenderPoints;
	};
}

#endif//_MY_CODE_RENDERABLES_BASE_RENDERER_H