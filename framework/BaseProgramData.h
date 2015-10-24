#ifndef _MY_CODE_BASE_PROGRAM_DATA_H_
#define _MY_CODE_BASE_PROGRAM_DATA_H_

#include <string>
#include "IBaseProgramData.h"

namespace MyCode
{ 
	class BaseProgramData : public IBaseProgramData
	{
	public:
		BaseProgramData(const std::string& vertexShader, const std::string& fragmentShader);
		virtual ~BaseProgramData();

		virtual GLuint GetProgramID() const
		{ 
			return mProgramID; 
		}

	protected:
		void InitUniform(GLint& uniform, const char* uniformName);
		void InitAttribute(GLint& attribute, const char* attributeName);
		
		GLint GetAttribute(const char* attributeName) const { return glGetAttribLocation(mProgramID, attributeName); }
		GLint GetUniform(const char* uniformName) const { return glGetUniformLocation(mProgramID, uniformName); }
		GLuint GetUniformBlockIndex(const char* blockName) const { return glGetUniformBlockIndex(mProgramID, blockName); }

		GLuint mProgramID;

	private:
		void CreateProgram(const std::string& vertexShader, const std::string& fragmentShader);
	};
}

#endif // _MY_CODE_BASE_PROGRAM_DATA_H_