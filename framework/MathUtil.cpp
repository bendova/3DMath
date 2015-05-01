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

		std::pair<float, bool> GetSegmentIntersectionFactor(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& d)
		{
			bool doesIntersectionPointExist = false;
			float factorAB = FLT_MAX;
			const glm::vec3 lineDirectionAB{ b - a };
			const glm::vec3 lineDirectionCD{ d - c };
			const glm::vec3 lineDirectionsCross{ glm::cross(lineDirectionAB, lineDirectionCD) };
			const auto lineDirectionsCrossLength = glm::length(lineDirectionsCross);
			if (lineDirectionsCrossLength)
			{
				const glm::vec3 ca{ a - c };
				factorAB = -glm::dot(glm::cross(ca, lineDirectionCD), lineDirectionsCross)
					/ (lineDirectionsCrossLength * lineDirectionsCrossLength);
				doesIntersectionPointExist = true;
			}
			return std::make_pair(factorAB, doesIntersectionPointExist);
		}

		float IsIntersectionFactorOnSegment(const float factor)
		{
			const float minFactor = 0.0f;
			const float maxFactor = 1.0f;
			return ((minFactor <= factor) && (factor <= maxFactor));
		}

		glm::vec4 GetIntersectionPoint(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& d)
		{
			glm::vec4 intersectionPoint{ 0.0f, 0.0f, 0.0f, 0.0f };

			const auto factorAB = GetSegmentIntersectionFactor(a, b, c, d);
			const auto factorCD = GetSegmentIntersectionFactor(c, d, a, b);
			if (factorAB.second && IsIntersectionFactorOnSegment(factorAB.first) &&
				factorCD.second && IsIntersectionFactorOnSegment(factorCD.first))
			{
				intersectionPoint = a + factorAB.first * (b - a);
			}
			return intersectionPoint;
		}

		float FloorWithPrecision(const float x, const int precision)
		{
			const int scale = static_cast<int>(pow(10, precision));
			
			double temp = x * scale;
			temp = temp < 0.0 ? ceil(temp) : floor(temp);
			temp /= scale;
			return static_cast<float>(temp);
		}

		void Floor(glm::vec4& v)
		{
			const int precision = 5;
			v.x = FloorWithPrecision(v.x, precision);
			v.y = FloorWithPrecision(v.y, precision);
			v.z = FloorWithPrecision(v.z, precision);
			v.w = FloorWithPrecision(v.w, precision);
		}

		glm::vec4 GetProjectionPoint(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c)
		{
			glm::vec4 lineDirection = b - a;
			
			const float lineDirectionLength = glm::length(lineDirection);
			glm::vec4 ac = c - a;
			float factor = glm::dot(lineDirection, ac) / (lineDirectionLength * lineDirectionLength);
			factor = FloorWithPrecision(factor, 6);
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
			return (res >= 0.0);
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
