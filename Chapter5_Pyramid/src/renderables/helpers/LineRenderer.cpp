#include "LineRenderer.h"

namespace MyCode
{
	LineRenderer::LineRenderer(const PosColorProgram& posColorProgram)
		: BaseRenderer(posColorProgram)
	{}

	LineRenderer::~LineRenderer()
	{}

	void LineRenderer::DoRender()
	{
		glLineWidth(1.0f);
		glDrawArrays(GL_LINE_STRIP, 0, GetRenderPoints().size());
	}
}