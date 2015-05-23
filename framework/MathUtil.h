#ifndef _MATH_UTIL_
#define _MATH_UTIL_

#include <cmath>
#include <glm/glm.hpp>

namespace MyCode
{
	namespace MathUtil
	{
		extern const float PI;

		float DegreesToRadians(float angleInDegrees);
		float GetFrustumScale(float angleInDegrees);
		glm::mat4 RotateX(float angleInDegrees);
		glm::mat4 RotateY(float angleInDegrees);
		glm::mat4 RotateZ(float angleInDegrees);

		float Clamp(const float value, const float min, const float max);
	}
}
#endif