#ifndef _MY_CODE_LINE_PAINTER_H_
#define _MY_CODE_LINE_PAINTER_H_

#include <glload/gl_3_3.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "../framework/MathUtil.h"
#include "LineRenderer.h"
#include "TriangleRenderer.h"

namespace MyCode
{
	using namespace MathUtil;
	class PosColorProgram;

	class LinePainter
	{
	public:
		LinePainter(const PosColorProgram& posColorProgram);
		~LinePainter();

		void Render(const glm::mat4& modelMatrix);
		void SetCameraToClipMatrixInverse(const glm::mat4& cameraToClipInverse) { mCameraToClipInverse = cameraToClipInverse; }
		void SetScreenDimensions(const GLint w, const GLint h) { mScreenWidth = w; mScreenHeight = h; }

		bool OnMouseClick(int button, int state, int x, int y); 
		bool OnMouseMoved(int x, int y); 
	private:
		void ClearLinePoints();
		
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

		static const int VERTEX_BUFFER_MAX_SIZE;
		static const int VERTEX_COMPONENT_COUNT;
		static const int COLOR_COMPONENT_COUNT;
		LineRenderer mLineRenderer;
		TriangleRenderer mTriangleRenderer;
		std::vector<ColoredPoint> mLinePoints;
		const PosColorProgram& mPosColorProgram;
		glm::mat4 mCameraToClipInverse;
		glm::mat4 mModelToCameraInverse;
		GLint mScreenWidth;
		GLint mScreenHeight;
		glm::vec4 mPlaneNormal;
		glm::vec4 mPlanePoint;

		std::unique_ptr<Quadrilateral> mQuadrilateral;
	};
}

#endif //_MY_CODE_LINE_PAINTER_H_