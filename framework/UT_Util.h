#ifndef _MY_CODE_UT_UTIL_H_
#define _MY_CODE_UT_UTIL_H_

#include <cstdio>
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
				std::printf("%s() Failed: %s!\n", testName, msg);
			}
			else
			{
				std::printf("%s() Failed!\n", testName);
			}
		}

		return equals;
	}

	bool CheckIsTrue(bool value, const char* expr, const char* testName);

	bool AreSegmentsEqualWithinMargin(const std::pair<glm::vec3, glm::vec3>& ab, const std::pair<glm::vec3, glm::vec3>& cd,
		const double margin);
	bool AreVectorsEqualWithinMargin(const glm::vec3& a, const glm::vec3& b, const double margin);
}

#endif //_MY_CODE_UT_UTIL_H_