#ifndef _MY_CODE_UT_UTIL_H_
#define _MY_CODE_UT_UTIL_H_

#include <cstdio>

namespace MyCode
{
#define CHECK_EQUALS(actual, expected) CheckEquals((actual), (expected), __FUNCTION__)
#define CHECK_EQUALS_MSG(actual, expected, msg) CheckEquals((actual), (expected), __FUNCTION__, (msg))

	template<typename T, typename U>
	bool CheckEquals(const T& actual, const U& expected, const char* testName, const char* msg = nullptr)
	{
		bool equals = (actual == expected);
		if (!equals)
		{
			if (msg)
			{
				std::printf("%s() Failed: %s!\n", testName, msg);
			}
			else
			{
				std::printf("%s() Failed!\n", testName);
			}
		}

		return equals;
	}
}

#endif //_MY_CODE_UT_UTIL_H_