#ifndef _MY_CODE_RENDERABLES_LINE_RENDERER_H
#define _MY_CODE_RENDERABLES_LINE_RENDERER_H

#include "BaseRenderer.h"

namespace MyCode
{
	class LineRenderer : public BaseRenderer
	{
	public:
		LineRenderer(const PosColorProgram& posColorProgram);
		virtual ~LineRenderer();

	protected:
		virtual void DoRender();
	};
}

#endif //_MY_CODE_RENDERABLES_LINE_RENDERER_H