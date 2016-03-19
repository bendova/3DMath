#include "TriangleRenderer.h"

namespace MyCode
{
	TriangleRenderer::TriangleRenderer(const PosColorProgram& posColorProgram)
		: mTrianglesRenderer(posColorProgram)
		, mTriangleFanRenderers()
		, mPosColorProgram(posColorProgram)
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

	void TriangleRenderer::Render()
	{
		DrawArrays(mTrianglesRenderer, GL_TRIANGLES);

		for (BaseRenderer& fanRenderer : mTriangleFanRenderers)
		{
			DrawArrays(fanRenderer, GL_TRIANGLE_FAN);
		}
	}

	void TriangleRenderer::DrawArrays(const BaseRenderer& renderer, const GLenum renderType)
	{
		glBindVertexArray(renderer.GetVertexArrayObjectID());
		glDrawArrays(renderType, 0, renderer.GetRenderPoints().size());
		glBindVertexArray(GL_NONE);
	}

	void TriangleRenderer::AddTriangleFanPoints(const std::vector<glm::vec4>& trianglePoints, const Color& triangleColor)
	{
		BaseRenderer renderer{ mPosColorProgram };
		for (const glm::vec4& p : trianglePoints)
		{
			renderer.AddPointToRender(p, triangleColor);
		}

		mTriangleFanRenderers.push_back(renderer);
	}

	void TriangleRenderer::AddTrianglePoints(const std::vector<glm::vec4>& trianglePoints, const Color& triangleColor)
	{
		DoAddTrianglePoints(trianglePoints.begin(), trianglePoints.end(), triangleColor);
	}

	void TriangleRenderer::AddTrianglePoints(const std::vector<glm::vec4>& trianglePoints)
	{
		const Color& triangleColor = GetNextColor();
		DoAddTrianglePoints(trianglePoints.begin(), trianglePoints.end(), triangleColor);
	}

	Color& TriangleRenderer::GetNextColor()
	{
		int nextColorIndex = mCurrentColorIndex++;
		mCurrentColorIndex = mCurrentColorIndex % mColors.size();
		return mColors[nextColorIndex];
	}

	void TriangleRenderer::ClearPoints()
	{
		mTrianglesRenderer.ClearPoints();
		for (BaseRenderer& fanRenderer : mTriangleFanRenderers)
		{
			fanRenderer.ClearPoints();
		}
	}
}