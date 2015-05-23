#include "UT_Util.h"

namespace MyCode
{
	bool CheckIsTrue(bool value, const char* expr, const char* testName)
	{
		if (value == false)
		{
			std::printf("%s() Failed because expression (%s) is false\n", testName, expr);
		}
		return value;
	}

	bool AreSegmentsEqualWithinMargin(const std::pair<glm::vec3, glm::vec3>& ab, const std::pair<glm::vec3, glm::vec3>& cd,
		const double margin)
	{
		return (AreVectorsEqualWithinMargin(ab.first, cd.first, margin)
			&& AreVectorsEqualWithinMargin(ab.second, cd.second, margin));
	}

	bool AreVectorsEqualWithinMargin(const glm::vec3& a, const glm::vec3& b, const double margin)
	{
		return ((std::abs(a.x - b.x) <= margin)
			&& (std::abs(a.y - b.y) <= margin)
			&& (std::abs(a.z - b.z) <= margin));
	}
}
