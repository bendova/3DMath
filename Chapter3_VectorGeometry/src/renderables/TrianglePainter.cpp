#include "TrianglePainter.h"
#include "glutil/MatrixStack.h"
#include "GL/freeglut.h"
#include <assert.h>
#include "../programs/PosColorProgram.h"
#include "../geometryhelpers/TriangleIntersection.h"
#include "../geometryhelpers/UT_TriangleIntersection.h"
#include "../geometryhelpers/DemoTriangleIntersection.h"

namespace MyCode
{
	TrianglePainter::TrianglePainter(const PosColorProgram& posColorProgram)
		: mPosColorProgram(posColorProgram)
		, mLineRenderer(posColorProgram)
		, mTriangleRenderer(posColorProgram)
		, mDemoRenderer(posColorProgram)
		, mDrawingState(DrawingState::DRAWED_NOTHING)
		, mTriangleA()
		, mQuadIntersecter(Quadrilateral{ glm::vec4(-3.0f, 0.0f, -3.0f, 1.0f), glm::vec4(3.0f, 0.0f, -3.0f, 1.0f),
			glm::vec4(3.0f, 0.0f, 3.0f, 1.0f), glm::vec4(-3.0f, 0.0f, 3.0f, 1.0f) },
			glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))
	{
		UT_TriangleIntersection intersectionTest;
		assert(intersectionTest.Run());

		DemoTriangleIntersection demo(mDemoRenderer);
	}

	TrianglePainter::~TrianglePainter()
	{}

	void TrianglePainter::ClearLinePoints()
	{
		mLineRenderer.ClearPoints();
		mTriangleRenderer.ClearPoints();
	}

	void TrianglePainter::Render(const glm::mat4& modelMatrix)
	{
		mQuadIntersecter.SetModelToCameraInverse(glm::inverse(modelMatrix));

		mLineRenderer.Render();
		mTriangleRenderer.Render();
		mDemoRenderer.Render();
	}

	bool TrianglePainter::OnMouseClick(int button, int state, int x, int y)
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
	
	bool TrianglePainter::HandleMousePressed(int button, int state, int x, int y)
	{
		bool isHandled = true;
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			if (mDrawingState != DrawingState::DRAWED_INTERSECTION)
			{
				StartLineDrawing(x, y);
			}
			break;
		case GLUT_RIGHT_BUTTON:
			ClearLinePoints();
			mDrawingState = DrawingState::DRAWED_NOTHING;
			break;
		default:
			isHandled = false;
			break;
		}
		return isHandled;
	}

	bool TrianglePainter::OnMouseMoved(int x, int y)
	{
		UpdateLineDrawing(x, y);
		return true;
	}

	bool TrianglePainter::HandleMouseReleased(int button, int state, int x, int y)
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

	void TrianglePainter::StartLineDrawing(const int screenX, const int screenY)
	{
		glm::vec4 intersectionPoint;
		if (mQuadIntersecter.GetPointOnQuad(screenX, screenY, intersectionPoint))
		{
			if (mLineRenderer.GetWorldPoints().size() == 0)
			{
				mLineRenderer.AddPointToRender(intersectionPoint);
			}
			mLineRenderer.AddPointToRender(intersectionPoint);
		}
	}

	void TrianglePainter::UpdateLineDrawing(const int screenX, const int screenY)
	{
		glm::vec4 intersectionPoint;
		if (mQuadIntersecter.GetPointOnQuad(screenX, screenY, intersectionPoint))
		{
			mLineRenderer.ReplaceLastPoint(intersectionPoint);
		}
	}

	void TrianglePainter::CheckIfTriangleIsComplete()
	{
		bool canDrawTriangle = (mLineRenderer.GetWorldPoints().size() >= 3);
		if (canDrawTriangle)
		{
			const auto& points = mLineRenderer.GetWorldPoints();
			mTriangleRenderer.AddTrianglePoints(points);

			switch (mDrawingState)
			{
			case DrawingState::DRAWED_NOTHING:
				mTriangleA = points;
				mDrawingState = DrawingState::DRAWED_FIRST_TRIANGLE;
				break;
			case DrawingState::DRAWED_FIRST_TRIANGLE:
				{
					const auto& mTriangleB = points;
					const auto intersection = TriangleIntersection::GetIntersectionArea
						(Triangle{ mTriangleA[0], mTriangleA[1], mTriangleA[2] },
						Triangle{ mTriangleB[0], mTriangleB[1], mTriangleB[2] });
					mTriangleRenderer.AddTriangleFanPoints(intersection, Color(0.0f, 0.0f, 0.0f, 1.0f));
					mDrawingState = DrawingState::DRAWED_INTERSECTION;
				}
				break;
			}

			mLineRenderer.ClearPoints();
		}
	}
}