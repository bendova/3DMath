#ifndef _MY_CODE_RENDERABLES_TRIANGLE_RENDERER_H
#define _MY_CODE_RENDERABLES_TRIANGLE_RENDERER_H

#include "BaseRenderer.h"
#include "../framework/MathUtil.h"

namespace MyCode
{
	class TriangleRenderer : public BaseRenderer
	{
	public:
		TriangleRenderer(const PosColorProgram& posColorProgram);
		virtual ~TriangleRenderer();

		void AddTrianglePoints(const std::vector<glm::vec4>& trianglePoints);

		static const int TRIANGLE_VERTICES_COUNT;
	protected:
		virtual void DoRender();

	private:
		void InitColors();
		Color& GetNextColor();

		std::vector<Color> mColors;
		int mCurrentColorIndex;
	};
}

#endif //_MY_CODE_RENDERABLES_TRIANGLE_RENDERER_H