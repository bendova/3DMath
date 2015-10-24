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

		GLuint GetModelToCameraUniform() const { return m_modelToCameraUniform; }
		GLuint GetCameraToClipUniform() const { return m_cameraToClipUniform; }

	private:
		GLuint m_modelToCameraUniform;
		GLuint m_cameraToClipUniform;
	};
}

#endif //_MY_CODE_POS_COLOR_PROGRAM_H_