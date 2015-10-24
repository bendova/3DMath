#include "PosColorProgram.h"

namespace MyCode
{
	PosColorProgram::PosColorProgram(const std::string& vertexShader, const std::string& fragmentShader)
		: BaseProgramData(vertexShader, fragmentShader)
		, m_modelToCameraUniform(GetUniform("modelToCamera"))
		, m_cameraToClipUniform(GetUniform("cameraToClip"))
		, m_positionAttribute(GetAttribute("position"))
		, m_colorAttribute(GetAttribute("color"))
	{

	}
	PosColorProgram::~PosColorProgram()
	{

	}
}