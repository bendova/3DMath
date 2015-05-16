#include <cmath>
#include "MathUtil.h"

namespace MyCode
{
	namespace MathUtil
	{
		const float PI = 3.1415926535897932384626433832795f;

		float DegreesToRadians(float angleInDegrees)
		{
			return angleInDegrees * PI / 180.0f;
		}

		// http://en.wikipedia.org/wiki/Angle_of_view#Derivation_of_the_angle-of-view_formula
		float GetFrustumScale(float angleInDegrees)
		{
			float halfAngleInRadians = DegreesToRadians(angleInDegrees) / 2.0f;
			return 1.0f / std::tan(halfAngleInRadians);
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

		float IsIntersectionFactorOnSegmentStrictly(const float factor)
		{
			const float minFactor = 0.0f;
			const float maxFactor = 1.0f;
			return ((minFactor < factor) && (factor < maxFactor));
		}

		float IsIntersectionFactorOnSegment(const float factor)
		{
			const float minFactor = 0.0f;
			const float maxFactor = 1.0f;
			return ((minFactor <= factor) && (factor <= maxFactor));
		}

		float FloorWithPrecision(const float x, const int precision)
		{
			const int scale = static_cast<int>(pow(10, precision));
			
			double temp = x * scale;
			temp = temp < 0.0 ? ceil(temp) : floor(temp);
			temp /= scale;
			return static_cast<float>(temp);
		}

		double FloorWithPrecision(const double x, const int precision)
		{
			const int scale = static_cast<int>(pow(10, precision));

			double temp = x * scale;
			temp = temp < 0.0 ? ceil(temp) : floor(temp);
			temp /= scale;
			return static_cast<float>(temp);
		}

		void Floor(glm::vec3& v, const int precision)
		{
			v.x = FloorWithPrecision(v.x, precision);
			v.y = FloorWithPrecision(v.y, precision);
			v.z = FloorWithPrecision(v.z, precision);
		}

		void Floor(glm::vec4& v, const int precision)
		{
			v.x = FloorWithPrecision(v.x, precision);
			v.y = FloorWithPrecision(v.y, precision);
			v.z = FloorWithPrecision(v.z, precision);
			v.w = FloorWithPrecision(v.w, precision);
		}

		float GetDistanceFromPointToLine(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b)
		{
			glm::vec3 lineDirection = b - a;

			const float lineDirectionLength = glm::length(lineDirection);
			glm::vec3 ap = p - a;
			float shadowLength = glm::dot(lineDirection, ap) / lineDirectionLength;
			shadowLength = FloorWithPrecision(shadowLength, 6);
			const float apLength = length(ap);
			float distanceToLine = sqrt(apLength * apLength - shadowLength * shadowLength);
			distanceToLine = FloorWithPrecision(distanceToLine, 6);
			return distanceToLine;
		}

		glm::vec4 GetNormalToLineFromPoint(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c)
		{
			glm::vec4 projectionPoint = GetProjectionPointOnLine(a, b, c);
			glm::vec4 lineNormal = c - projectionPoint;
			return lineNormal;
		}

		float GetDotBetweenPointAndLine(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& p)
		{
			const glm::vec4 ap = p - a;
			const glm::vec4 abNormal = GetNormalToLineFromPoint(a, b, c);
			const auto res = glm::dot(ap, abNormal);
			return res;
		}

		bool IsPointInsideOfSide(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& p)
		{
			return (GetDotBetweenPointAndLine(a, b, c, p) >= 0.0);
		}

		bool IsPointInsideOfSideStrictly(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& p)
		{
			return (GetDotBetweenPointAndLine(a, b, c, p) > 0.0);
		}

		bool IsPointInPlane(const glm::vec4& planePointA, const glm::vec4& planePointB,
			const glm::vec4& planePointC, const glm::vec4& point)
		{
			return IsPointInPlane( glm::vec3{ planePointA }, glm::vec3{ planePointB }, glm::vec3{ planePointC }, glm::vec3{ point } );
		}

		bool IsPointInPlane(const glm::vec3& planePointA, const glm::vec3& planePointB, const glm::vec3& planePointC, const glm::vec3& point)
		{
			const glm::vec3 ab = planePointB - planePointA;
			const glm::vec3 ac = planePointC - planePointA;
			const glm::vec3 planeNormal = glm::cross(ab, ac);
			const glm::vec3 ap = point - planePointA;
			const bool isInPlane = (glm::dot(planeNormal, ap) == 0);
			return isInPlane;
		}

		bool Contains(const Triangle& q, const glm::vec4& p)
		{
			const auto& a = q[0];
			const auto& b = q[1];
			const auto& c = q[2];

			bool result = IsPointInPlane(a, b, c, p) && IsPointInsideOfSide(a, b, c, p) && IsPointInsideOfSide(b, c, a, p) &&
				IsPointInsideOfSide(c, a, b, p);
			return result;
		}
		
		bool Contains(const Quadrilateral& q, const glm::vec4& p)
		{
			const auto& a = q[0];
			const auto& b = q[1];
			const auto& c = q[2];
			const auto& d = q[3];

			bool result = IsPointInPlane(a, b, c, p) && IsPointInsideOfSide(a, b, d, p) && IsPointInsideOfSide(b, c, a, p) &&
				IsPointInsideOfSide(c, d, b, p) && IsPointInsideOfSide(d, a, c, p);
			return result;
		}

		bool ContainsStrictly(const Triangle& q, const glm::vec4& p)
		{
			const auto& a = q[0];
			const auto& b = q[1];
			const auto& c = q[2];

			bool result = IsPointInPlane(a, b, c, p) && IsPointInsideOfSideStrictly(a, b, c, p) 
				&& IsPointInsideOfSideStrictly(b, c, a, p) && IsPointInsideOfSideStrictly(c, a, b, p);
			return result;
		}

		bool ContainsStrictly(const Quadrilateral& q, const glm::vec4& p)
		{
			const auto& a = q[0];
			const auto& b = q[1];
			const auto& c = q[2];
			const auto& d = q[3];

			bool result = IsPointInPlane(a, b, c, p) && IsPointInsideOfSideStrictly(a, b, d, p) && IsPointInsideOfSideStrictly(b, c, a, p) &&
				IsPointInsideOfSideStrictly(c, d, b, p) && IsPointInsideOfSideStrictly(d, a, c, p);
			return result;
		}

		bool DoLineSegmentsIntersectStrictly(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d)
		{
			bool doTheyIntersect = false;

			const auto factorAB = GetSegmentIntersectionFactor(a, b, c, d);
			const auto factorCD = GetSegmentIntersectionFactor(c, d, a, b);
			if (factorAB.second && IsIntersectionFactorOnSegmentStrictly(factorAB.first) &&
				factorCD.second && IsIntersectionFactorOnSegmentStrictly(factorCD.first))
			{
				doTheyIntersect = true;
			}
			return doTheyIntersect;
		}

		bool DoLineSegmentsIntersectStrictly(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& d)
		{
			const glm::vec4 intersectionPoint = GetIntersectionPointBetweenSegmentsStrictly(a, b, c, d);
			return (intersectionPoint.w != 0);
		}
	}
}
