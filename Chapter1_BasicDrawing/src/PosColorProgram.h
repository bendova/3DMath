#ifndef _MY_CODE_POS_COLOR_PROGRAM_H_
#define _MY_CODE_POS_COLOR_PROGRAM_H_

#include "BaseProgramData.h"

namespace MyCode
{
	class PosColorProgram: public BaseProgramData
	{
	public:
		PosColorProgram(const std::string& vertexShader, const std::string& fragmentShader);
		virtual ~PosColorProgram();

		GLint GetPositionAttrib() const { return m_positionAttrib; }
		GLint GetColorAttrib() const { return m_colorAttrib; }
		GLint GetModelToCameraUniform() const { return m_modelToCameraUniform; }
		GLint GetCameraToClipUniform() const { return m_cameraToClipUniform; }

	private:
		GLint m_positionAttrib;
		GLint m_colorAttrib;
		GLint m_modelToCameraUniform;
		GLint m_cameraToClipUniform;
	};
}

#endif //_MY_CODE_POS_COLOR_PROGRAM_H_