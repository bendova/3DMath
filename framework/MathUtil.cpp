#include <math.h>
#include "MathUtil.h"

namespace MyCode
{
	namespace MathUtil
	{
		float DegreesToRadians(float angleInDegrees)
		{
			return angleInDegrees * PI / 180.0f;
		}

		// http://en.wikipedia.org/wiki/Angle_of_view#Derivation_of_the_angle-of-view_formula
		float GetFrustumScale(float angleInDegrees)
		{
			float halfAngleInRadians = DegreesToRadians(angleInDegrees) / 2.0f;
			return 1.0f / tan(halfAngleInRadians / 2.0f);
		}

		glm::mat4 RotateX(float angleInDegrees)
		{
			float angleInRadians = DegreesToRadians(angleInDegrees);
			float cosOfAngle = cos(angleInRadians);
			float sinOfAngle = sin(angleInRadians);

			glm::mat4 xRotationMatrix(1.0f);
			xRotationMatrix[1].y = cosOfAngle;
			xRotationMatrix[1].z = sinOfAngle;
			xRotationMatrix[2].y = -sinOfAngle;
			xRotationMatrix[2].z = cosOfAngle;

			return xRotationMatrix;
		}

		glm::mat4 RotateY(float angleInDegrees)
		{
			float angleInRadians = DegreesToRadians(angleInDegrees);
			float cosOfAngle = cos(angleInRadians);
			float sinOfAngle = sin(angleInRadians);

			glm::mat4 yRotationMatrix(1.0f);
			yRotationMatrix[0].x = cosOfAngle;
			yRotationMatrix[0].z = -sinOfAngle;
			yRotationMatrix[2].x = sinOfAngle;
			yRotationMatrix[2].z = cosOfAngle;

			return yRotationMatrix;
		}

		glm::mat4 RotateZ(float angleInDegrees)
		{
			float angleInRadians = DegreesToRadians(angleInDegrees);
			float cosOfAngle = cos(angleInRadians);
			float sinOfAngle = sin(angleInRadians);

			glm::mat4 zRotationMatrix(1.0f);
			zRotationMatrix[0].x = cosOfAngle;
			zRotationMatrix[0].y = sinOfAngle;
			zRotationMatrix[1].x = -sinOfAngle;
			zRotationMatrix[1].y = cosOfAngle;

			return zRotationMatrix;
		}

		float Clamp(const float value, const float min, const float max)
		{
			if (value < min)
			{
				return min;
			}
			else if (value > max)
			{
				return max;
			}
			else
			{
				return value;
			}
		}

		glm::vec4 GetIntersectionPoint(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& d)
		{
			glm::vec4 intersectionPoint{ 0.0f, 0.0f, 0.0f, 0.0f };

			const glm::vec3 lineDirection1 { b - a };
			const glm::vec3 lineDirection2 { d - c };
			const glm::vec3 lineDirectionsCross{ glm::cross(lineDirection1, lineDirection2) };
			const auto lineDirectionsCrossLength = glm::length(lineDirectionsCross);
			if (lineDirectionsCrossLength)
			{
				const glm::vec3 ac{ c - a };
				const float factor = -glm::dot(glm::cross(ac, lineDirection2), lineDirectionsCross) 
					/ (lineDirectionsCrossLength * lineDirectionsCrossLength);

				intersectionPoint = a + factor * glm::vec4{ lineDirection1, 1.0f };
			}
			return intersectionPoint;
		}

		glm::vec4 GetProjectionPoint(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c)
		{
			glm::vec4 lineDirection = b - a;
			
			const auto lineDirectionLength = glm::length(lineDirection);;
			glm::vec4 ac = c - a;
			const auto factor = glm::dot(lineDirection, ac) / (lineDirectionLength * lineDirectionLength);
			glm::vec4 projectionPoint = a + factor * lineDirection;
			return projectionPoint;
		}

		glm::vec4 GetNormalToLineFromPoint(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c)
		{
			glm::vec4 projectionPoint = GetProjectionPoint(a, b, c);
			glm::vec4 lineNormal = c - projectionPoint;
			return lineNormal;
		}

		bool IsPointInsideOfSide(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& p)
		{
			const glm::vec4 ap = p - a;
			const glm::vec4 abNormal = GetNormalToLineFromPoint(a, b, c);
			const auto res = glm::dot(ap, abNormal);
			return (res >= -1.0e-5);
		}

		bool Contains(Triangle q, const glm::vec4& p)
		{
			const auto& a = q[0];
			const auto& b = q[1];
			const auto& c = q[2];

			bool result = IsPointInsideOfSide(a, b, c, p) && IsPointInsideOfSide(b, c, a, p) &&
				IsPointInsideOfSide(c, a, b, p);
			return result;
		}

		bool Contains(Quadrilateral q, const glm::vec4& p)
		{
			const auto& a = q[0];
			const auto& b = q[1];
			const auto& c = q[2];
			const auto& d = q[3];

			bool result = IsPointInsideOfSide(a, b, d, p) && IsPointInsideOfSide(b, c, a, p) &&
				IsPointInsideOfSide(c, d, b, p) && IsPointInsideOfSide(d, a, c, p);
			return result;
		}
	}
}
