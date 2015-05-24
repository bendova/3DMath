#ifndef _MY_CODE_LOGGING_H_
#define _MY_CODE_LOGGING_H_

#include <cassert>
#include <sstream>
#include <Windows.h>
#include <WinBase.h>

namespace MyCode
{
	void Log(const char* format);

	template<typename T, typename... Args>
	void Log(const char* format, T value, Args... args)
	{
		std::stringstream outStream;
		while (format && *format)
		{
			const char currentChar = *format;
			if(currentChar)
			{
				const char nextChar = *(++format);
				if ((currentChar == '%') && (nextChar != '%'))
				{
					outStream << value;
					OutputDebugStringA(outStream.str().c_str());
					return Log(++format, args...);
				}
				outStream << currentChar;
			}
		}
	}
}
#endif //_MY_CODE_LOGGING_H_