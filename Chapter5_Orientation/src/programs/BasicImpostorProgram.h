#ifndef _MY_CODE_BASIC_IMPOSTOR_PROGRAM_H_
#define _MY_CODE_BASIC_IMPOSTOR_PROGRAM_H_

#include "BaseProgramData.h"

namespace MyCode
{
	class BasicImpostorProgram 
		: public BaseProgramData
	{
	public:
		BasicImpostorProgram(const std::string& vertexShader, const std::string& fragmentShader);
		virtual ~BasicImpostorProgram();

		void BindUniformBlockMaterial(const int index);
		void BindUniformBlockLight(const int index);
		void BindUniformBlockProjection(const int index);

		GLint GetSphereRadiusUniform() const
		{
			return mSphereRadiusUniform;
		}
		GLint GetCameraSpherePosUniform() const
		{
			return mCameraSpherePosUniform;
		}
		GLint GetMaterialBlockIndex() const
		{
			return mMaterialBlockIndex;
		}
		GLint GetLightBlockIndex()	const
		{ 
			return mLightBlockIndex;
		}
		GLint GetProjectionBlockIndex()	const
		{
			return mProjectionBlockIndex;
		}

	private:
		GLint mSphereRadiusUniform;
		GLint mCameraSpherePosUniform;
		GLint mMaterialBlockIndex;
		GLint mLightBlockIndex;
		GLint mProjectionBlockIndex;
	};
}

#endif //_MY_CODE_BASIC_IMPOSTOR_PROGRAM_H_