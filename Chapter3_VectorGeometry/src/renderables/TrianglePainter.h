#ifndef _MY_CODE_LINE_PAINTER_H_
#define _MY_CODE_LINE_PAINTER_H_

#include <glload/gl_3_3.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "../framework/MathUtil.h"
#include "helpers/LineRenderer.h"
#include "helpers/TriangleRenderer.h"
#include "helpers/QuadIntersecter.h"

namespace MyCode
{
	using namespace MathUtil;
	class PosColorProgram;

	class TrianglePainter
	{
	public:
		TrianglePainter(const PosColorProgram& posColorProgram);
		~TrianglePainter();

		void Render(const glm::mat4& modelMatrix);
		void SetCameraToClipMatrixInverse(const glm::mat4& cameraToClipInverse) 
		{ 
			mQuadIntersecter.SetCameraToClipMatrixInverse(cameraToClipInverse); 
		}
		void SetScreenDimensions(const GLint w, const GLint h) 
		{
			mQuadIntersecter.SetScreenDimensions(w, h);
		}

		bool OnMouseClick(int button, int state, int x, int y); 
		bool OnMouseMoved(int x, int y); 
	private:
		enum class DrawingState
		{
			DRAWED_NOTHING,
			DRAWED_FIRST_TRIANGLE,
			DRAWED_INTERSECTION
		};
		
		void ClearLinePoints();
		
		void DrawDemoTriangleIntersections();

		void StartLineDrawing(const int screenX, const int screenY);
		void UpdateLineDrawing(const int screenX, const int screenY);
		void CheckIfTriangleIsComplete();
		bool GetPointOnQuad(const int screenX, const int screenY, glm::vec4& pointOnQuad);

		glm::vec4 GetPointInNDCFromScreenSpace(const float screenX, const float screenY);
		glm::vec4 GetPositionInWorldSpaceFromNDC(const glm::vec4& ndcPoint);
		glm::vec4 GetPositionInWorldSpaceFromCameraSpace(const glm::vec4& ndcPoint);
		glm::vec4 GetIntersectionPointWithPlane(const glm::vec4& worldPoint, const glm::vec4& worldPointDirection);

		bool HandleMousePressed(int button, int state, int x, int y);
		bool HandleMouseReleased(int button, int state, int x, int y);

		LineRenderer mLineRenderer;
		TriangleRenderer mTriangleRenderer;
		TriangleRenderer mDemoRenderer;
		const PosColorProgram& mPosColorProgram;

		DrawingState mDrawingState;
		std::vector<glm::vec4> mTriangleA;

		QuadIntersecter mQuadIntersecter;
	};
}

#endif //_MY_CODE_LINE_PAINTER_H_