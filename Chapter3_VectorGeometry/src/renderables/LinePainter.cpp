#include "LinePainter.h"
#include "glutil/MatrixStack.h"
#include "GL/freeglut.h"
#include <assert.h>
#include "../programs/PosColorProgram.h"
#include "../geometryhelpers/TriangleIntersection.h"
#include "../geometryhelpers/UT_TriangleIntersection.h"

namespace MyCode
{
	const int LinePainter::VERTEX_BUFFER_MAX_SIZE = 2 << 20;
	const int LinePainter::VERTEX_COMPONENT_COUNT = 3;
	const int LinePainter::COLOR_COMPONENT_COUNT = 4;

	LinePainter::LinePainter(const PosColorProgram& posColorProgram)
		: mPosColorProgram(posColorProgram)
		, mLineRenderer(posColorProgram)
		, mTriangleRenderer(posColorProgram)
		, mCameraToClipInverse(1.0f)
		, mModelToCameraInverse(1.0f)
		, mPlaneNormal(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))
		, mPlanePoint(glm::vec4(3.0f, 0.0f, -3.0f, 1.0f))
	{
		mQuadrilateral = std::make_unique<Quadrilateral>(Quadrilateral{ glm::vec4(-3.0f, 0.0f, -3.0f, 1.0f), glm::vec4(3.0f, 0.0f, -3.0f, 1.0f),
			glm::vec4(3.0f, 0.0f, 3.0f, 1.0f), glm::vec4(-3.0f, 0.0f, 3.0f, 1.0f) });

		UT_TriangleIntersection intersectionTest;
		assert(intersectionTest.Run());
	}

	LinePainter::~LinePainter()
	{}

	void LinePainter::ClearLinePoints()
	{
		mLineRenderer.ClearPoints();
		mTriangleRenderer.ClearPoints();
	}

	void LinePainter::Render(const glm::mat4& modelMatrix)
	{
		mModelToCameraInverse = glm::inverse(modelMatrix);

		mLineRenderer.Render();
		mTriangleRenderer.Render();
	}

	bool LinePainter::OnMouseClick(int button, int state, int x, int y)
	{
		bool isHandled = true;
		switch (state)
		{
		case GLUT_DOWN:
			HandleMousePressed(button, state, x, y);
			break;
		case GLUT_UP:
			HandleMouseReleased(button, state, x, y);
		default:
			isHandled = false;
			break;
		}
		return isHandled;
	}
	
	bool LinePainter::HandleMousePressed(int button, int state, int x, int y)
	{
		bool isHandled = true;
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			StartLineDrawing(x, y);
			break;
		case GLUT_RIGHT_BUTTON:
			ClearLinePoints();
			break;
		default:
			isHandled = false;
			break;
		}
		return isHandled;
	}

	bool LinePainter::OnMouseMoved(int x, int y)
	{
		UpdateLineDrawing(x, y);
		return true;
	}

	bool LinePainter::HandleMouseReleased(int button, int state, int x, int y)
	{
		bool isHandled = false;
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			CheckIfTriangleIsComplete();
			break;
		default:
			isHandled = false;
			break;
		}
		return isHandled;
	}

	void LinePainter::StartLineDrawing(const int screenX, const int screenY)
	{
		glm::vec4 intersectionPoint;
		if (GetPointOnQuad(screenX, screenY, intersectionPoint))
		{
			if (mLineRenderer.GetWorldPoints().size() == 0)
			{
				mLineRenderer.AddPointToRender(intersectionPoint);
			}
			mLineRenderer.AddPointToRender(intersectionPoint);
		}
	}

	void LinePainter::UpdateLineDrawing(const int screenX, const int screenY)
	{
		glm::vec4 intersectionPoint;
		if (GetPointOnQuad(screenX, screenY, intersectionPoint))
		{
			mLineRenderer.ReplaceLastPoint(intersectionPoint);
		}
	}

	void LinePainter::CheckIfTriangleIsComplete()
	{
		bool canDrawTriangle = (mLineRenderer.GetWorldPoints().size() >= 3);
		if (canDrawTriangle)
		{
			mTriangleRenderer.AddTrianglePoints(mLineRenderer.GetWorldPoints());
			mLineRenderer.ClearPoints();
		}
	}

	bool LinePainter::GetPointOnQuad(const int screenX, const int screenY, glm::vec4& pointOnQuad)
	{
		glm::vec4 ndcPoint = GetPointInNDCFromScreenSpace(static_cast<float>(screenX), static_cast<float>(screenY));
		glm::vec4 linePoint = GetPositionInWorldSpaceFromNDC(ndcPoint);
		glm::vec4 cameraPosition = GetPositionInWorldSpaceFromCameraSpace(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glm::vec4 lineDirection = linePoint - cameraPosition;

		pointOnQuad = GetIntersectionPointWithPlane(linePoint, lineDirection);
		pointOnQuad.y += 0.1f;//FIXME There's gotta be a better way of getting rid of the flicker
		bool isInsideQuad = ((pointOnQuad.w != 0.0f) &&
			(Contains(*mQuadrilateral, pointOnQuad)));
		return isInsideQuad;
	}

	glm::vec4 LinePainter::GetPointInNDCFromScreenSpace(const float screenX, const float screenY)
	{
		const float halfScreenWidth = mScreenWidth / 2.0f;
		const float halfScreenHeight = mScreenHeight / 2.0f;
		glm::vec4 ndcPoint;
		ndcPoint.x = (screenX - halfScreenWidth) / halfScreenWidth;
		ndcPoint.y = (halfScreenHeight - screenY) / halfScreenHeight;

		return ndcPoint;
	}

	glm::vec4 LinePainter::GetPositionInWorldSpaceFromNDC(const glm::vec4& ndcPoint)
	{
		glm::vec4 screenPoint(ndcPoint.x, ndcPoint.y, ndcPoint.z, 1.0f);
		glm::vec4 worldPoint = mModelToCameraInverse * mCameraToClipInverse * screenPoint;

		worldPoint /= worldPoint.w;

		return worldPoint;
	}

	glm::vec4 LinePainter::GetPositionInWorldSpaceFromCameraSpace(const glm::vec4& cameraPoint)
	{
		glm::vec4 worldPoint = mModelToCameraInverse * cameraPoint;
		return worldPoint;
	}

	glm::vec4 LinePainter::GetIntersectionPointWithPlane(const glm::vec4& linePoint, const glm::vec4& lineDirection)
	{
		glm::vec4 intersectionPoint;
		
		float factor = 0.0f;
		const float planeDotLine = glm::dot(mPlaneNormal, lineDirection);
		if (planeDotLine != 0.0f)
		{
			glm::vec4 planeToLineVector = linePoint - mPlanePoint;
			factor = -(glm::dot(mPlaneNormal, planeToLineVector) / planeDotLine);
			intersectionPoint = linePoint + factor * lineDirection;
		}
		return intersectionPoint;
	}
}