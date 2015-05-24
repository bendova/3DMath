#include "Logging.h"

namespace MyCode
{
	void Log(const char* format)
	{
		std::stringstream outStream;
		while (format && *format)
		{
			const char currentChar = *format;
			if (currentChar)
			{
				outStream << currentChar;

				const char nextChar = *(++format);
				if ((currentChar == '%') && (nextChar != '%'))
				{
					assert(false && "Log() Missing arguments in format!");
				}
			}
		}
		OutputDebugStringA(outStream.str().c_str());
	}
}