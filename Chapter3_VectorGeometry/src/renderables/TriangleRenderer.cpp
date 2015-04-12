#include "TriangleRenderer.h"

namespace MyCode
{
	const int TriangleRenderer::TRIANGLE_VERTICES_COUNT = 3;

	TriangleRenderer::TriangleRenderer(const PosColorProgram& posColorProgram)
		: BaseRenderer(posColorProgram)
		, mColors()
		, mCurrentColorIndex(0)
	{
		InitColors();
	}

	void TriangleRenderer::InitColors()
	{
		mColors.push_back(Color(1.0f, 0.0f, 0.0f, 1.0f));
		mColors.push_back(Color(0.0f, 1.0f, 0.0f, 1.0f));
		mColors.push_back(Color(0.0f, 0.0f, 1.0f, 1.0f));
		mColors.push_back(Color(1.0f, 1.0f, 0.0f, 1.0f));
		mColors.push_back(Color(0.0f, 1.0f, 1.0f, 1.0f));
		mColors.push_back(Color(1.0f, 0.0f, 1.0f, 1.0f));
	}

	TriangleRenderer::~TriangleRenderer()
	{}

	void TriangleRenderer::DoRender()
	{
		glDrawArrays(GL_TRIANGLES, 0, GetRenderPoints().size());
	}

	void TriangleRenderer::AddTrianglePoints(const std::vector<glm::vec4>& trianglePoints)
	{
		if (trianglePoints.size() >= TRIANGLE_VERTICES_COUNT)
		{
			const Color& triangleColor = GetNextColor();
			for (int i = 0; i < TRIANGLE_VERTICES_COUNT; ++i)
			{
				AddPointToRender(trianglePoints[i], triangleColor);
			}
		}
	}

	Color& TriangleRenderer::GetNextColor()
	{
		int nextColorIndex = mCurrentColorIndex++;
		mCurrentColorIndex = mCurrentColorIndex % mColors.size();
		return mColors[nextColorIndex];
	}
}