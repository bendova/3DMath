#ifndef _MY_CODE_RENDERABLES_TRIANGLE_RENDERER_H
#define _MY_CODE_RENDERABLES_TRIANGLE_RENDERER_H

#include "BaseRenderer.h"
#include "../framework/MathUtil.h"

namespace MyCode
{
	class TriangleRenderer
	{
	public:
		TriangleRenderer(const PosColorProgram& posColorProgram);
		virtual ~TriangleRenderer();
		
		void Render();

		void AddTriangleFanPoints(const std::vector<glm::vec4>& trianglePoints, const Color& triangleColor);

		void AddTrianglePoints(const std::vector<glm::vec4>& trianglePoints, const Color& triangleColor);
		void AddTrianglePoints(const std::vector<glm::vec4>& trianglePoints);

		void ClearPoints();
	private:
		void DrawArrays(const BaseRenderer& renderer, const GLenum renderType);

		template<typename T>
		void DoAddTrianglePoints(T& itBegin, const T& itEnd, const Color& triangleColor)
		{
			for (; itBegin != itEnd; ++itBegin)
			{
				mTrianglesRenderer.AddPointToRender(*itBegin, triangleColor);
			}
		}

		void InitColors();
		Color& GetNextColor();

		std::vector<Color> mColors;
		int mCurrentColorIndex;
		const PosColorProgram& mPosColorProgram;
		BaseRenderer mTrianglesRenderer;
		std::vector<BaseRenderer> mTriangleFanRenderers;
	};
}

#endif //_MY_CODE_RENDERABLES_TRIANGLE_RENDERER_H