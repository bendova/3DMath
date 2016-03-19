#include "BasicImpostorProgram.h"

namespace MyCode
{
	BasicImpostorProgram::BasicImpostorProgram(const std::string& vertexShader, const std::string& fragmentShader)
		: BaseProgramData(vertexShader, fragmentShader)
		, mSphereRadiusUniform(GetUniform("sphereRadius"))
		, mCameraSpherePosUniform(GetUniform("cameraSpherePos"))
		, mMaterialBlockIndex(GetUniformBlockIndex("Material"))
		, mLightBlockIndex(GetUniformBlockIndex("Light"))
		, mCameraToClipUniform(GetUniform("cameraToClip"))
	{}

	BasicImpostorProgram::~BasicImpostorProgram()
	{

	}

	void BasicImpostorProgram::BindUniformBlockMaterial(const int index)
	{
		glUniformBlockBinding(mProgramID, mMaterialBlockIndex, index);
	}
	void BasicImpostorProgram::BindUniformBlockLight(const int index)
	{
		glUniformBlockBinding(mProgramID, mLightBlockIndex, index);
	}
	/*void BasicImpostorProgram::BindUniformBlockProjection(const int index)
	{
		glUniformBlockBinding(mProgramID, mProjectionBlockIndex, index);
	}*/
}