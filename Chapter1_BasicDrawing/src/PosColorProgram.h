#ifndef _MY_CODE_POS_COLOR_PROGRAM_H_
#define _MY_CODE_POS_COLOR_PROGRAM_H_

#include "BaseProgramData.h"

namespace MyCode
{
	class PosColorProgram : public BaseProgramData
	{
	public:
		PosColorProgram(const std::string& vertexShader, const std::string& fragmentShader);
		virtual ~PosColorProgram();

		GLint GetModelToCameraUniform() const { return m_modelToCameraUniform; }
		GLint GetCameraToClipUniform() const { return m_cameraToClipUniform; }
		GLint GetPositionAttribute() const { return m_positionAttribute; }
		GLint GetColorAttribute() const { return m_colorAttribute; }

	private:
		GLint m_modelToCameraUniform;
		GLint m_cameraToClipUniform;
		GLint m_positionAttribute;
		GLint m_colorAttribute;
	};
}

#endif //_MY_CODE_POS_COLOR_PROGRAM_H_