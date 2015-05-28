#ifndef _MY_CODE_UT_UTIL_H_
#define _MY_CODE_UT_UTIL_H_

#include "Logging.h"
#include <utility>
#include <glm/glm.hpp>

namespace MyCode
{
#define CHECK_EQUALS(actual, expected) CheckEquals((actual), (expected), __FUNCTION__)
#define CHECK_EQUALS_MSG(actual, expected, msg) CheckEquals((actual), (expected), __FUNCTION__, (msg))
#define CHECK_IS_TRUE(expr) CheckIsTrue((expr), #expr, __FUNCTION__)

	template<typename T, typename U>
	bool CheckEquals(const T& actual, const U& expected, const char* testName, const char* msg = nullptr)
	{
		bool equals = (actual == expected);
		if (!equals)
		{
			if (msg)
			{
				Log("%s() Failed: %s!\n", testName, msg);
			}
			else
			{
				Log("%s() Failed!\n", testName);
			}
		}

		return equals;
	}

	bool CheckIsTrue(bool value, const char* expr, const char* testName);
}

#endif //_MY_CODE_UT_UTIL_H_