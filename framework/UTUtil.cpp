#include "UTUtil.h"

namespace MyCode
{
	bool CheckIsTrue(bool value, const char* expr, const char* testName)
	{
		if (value == false)
		{
			Log("%s() Failed because expression (%s) is false\n", testName, expr);
		}
		return value;
	}
}
