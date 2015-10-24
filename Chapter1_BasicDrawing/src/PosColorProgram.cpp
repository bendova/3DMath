#include "PosColorProgram.h"

namespace MyCode
{
	PosColorProgram::PosColorProgram(const std::string& vertexShader, const std::string& fragmentShader)
		: BaseProgramData(vertexShader, fragmentShader)
		, m_modelToCameraUniform(GetUniform("modelToCamera"))
		, m_cameraToClipUniform(GetUniform("cameraToClip"))
	{

	}

	PosColorProgram::~PosColorProgram()
	{

	}
}