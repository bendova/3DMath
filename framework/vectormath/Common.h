#ifndef _MY_CODE_VECTOR_MATH_COMMON_H_
#define _MY_CODE_VECTOR_MATH_COMMON_H_

#include <cmath>
#include <glm/glm.hpp>
#include <initializer_list>
#include <vector>
#include <algorithm>
#include "DataTypes.h"

namespace MyCode
{
	namespace VectorMath
	{
		bool AreSegmentsEqualWithinMargin(const std::pair<glm::vec3, glm::vec3>& ab, const std::pair<glm::vec3, glm::vec3>& cd,
			const double margin = 1e-6);
		bool AreVectorsEqualWithinMargin(const glm::vec3& a, const glm::vec3& b, const double margin = 1e-6);
		bool AreVectorsEqualWithinMargin(const glm::vec4& a, const glm::vec4& b, const double margin = 1e-6);

		float FloorWithPrecision(const float x, const int precision);
		double FloorWithPrecision(const double x, const int precision);
		void Floor(glm::vec3& v, const int precision = 5);
		void Floor(glm::vec4& v, const int precision = 5);
		bool AreEqualWithMargin(const float a, const float b, const float errorMargin = 1e-5);

		bool IsInInterval(const float p, float a, float b);

		float GetDistanceFromPointToLine(const glm::vec3& p, const glm::vec3& pointOnLine, const glm::vec3& lineDirection);
		
		glm::vec3 GetProjectionPointOnPlane(const glm::vec3& pointToProject, const glm::vec3& pointInPlane, const glm::vec3& planeNormal);
		float GetDistanceBetweenPointAndPlane(const glm::vec3& pointToProject, const glm::vec3& pointInPlane, const glm::vec3& planeNormal);

		bool IsPointInPlane(const glm::vec4& planePointA, const glm::vec4& planePointB,
			const glm::vec4& planePointC, const glm::vec4& point);
		bool IsPointInPlane(const glm::vec3& planePointA, const glm::vec3& planePointB,
			const glm::vec3& planePointC, const glm::vec3& point);

		glm::vec3 GetNormalToPolygonPlane(const std::vector<glm::vec3>& polygon);
		bool IsPointCoplanarWithPolygon(const glm::vec3& point, const std::vector<glm::vec3>& polygon);

		bool DoesRayIntersectPolygon(const glm::vec3& origin, const glm::vec3& direction,
			const std::vector<glm::vec3>& polygon);

		bool operator==(const Plane& a, const Plane& b);

		Plane GetPolygonPlane(const std::vector<glm::vec3>& polygon);
		bool ArePolygonsCoplanar(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2);

		bool IsLineInPlane(const glm::vec3& a, const glm::vec3& b, const Plane& plane);
		
		bool IsFullSegment(const std::vector<float>& factors);
		bool AreColinearSegmentsEqual(const std::vector<float>& factorsAB, const std::vector<float>& factorsCD);

		bool IsSegment(const std::vector<float>& factorsAB);
		bool DoColinearSegmentsOverlap(const std::vector<float>& factorsAB, const std::vector<float>& factorsCD);

		std::pair<float, float> GetMinMaxLengthsPair(const std::vector<glm::vec3>& points);
	}
}

#endif //_MY_CODE_VECTOR_MATH_COMMON_H_