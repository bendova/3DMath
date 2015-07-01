#ifndef _MY_CODE_INTERSECTION_2D_POLLIGON_INTERSECTION_H_
#define _MY_CODE_INTERSECTION_2D_POLLIGON_INTERSECTION_H_

#include <vector>
#include <glm/glm.hpp>
#include "vectormath/DataTypes.h"

namespace MyCode
{
	namespace Intersection2D
	{
		namespace PolygonIntersection
		{
			bool DoPolygonsIntersect2D(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b, 
				const VectorMath::PointType pointType = VectorMath::PointType::CLOSED_ENDED);

			namespace Detail
			{
				enum class IntersectionType
				{
					POLYGON_WITH_POLYGON = 0,
					POLYGON_WITH_LINE_SEGMENT,
					LINE_SEGMENT_WITH_POLYGON,
					LINE_SEGMENT_WITH_LINE_SEGMENT,
				};

				IntersectionType GetIntersectionType(const std::vector<glm::vec3>& a, const std::vector<glm::vec3>& b);
				bool IsLineSegment(const std::vector<glm::vec3>& points);
				std::pair<glm::vec3, glm::vec3> GetLineSegmentFromCollinearPoints(const std::vector<glm::vec3>& collinearPoints);
				std::vector<glm::vec3> GetPairwiseDistinctPoints(const std::vector<glm::vec3>& points, const int count);
				bool DoPolygonWithLineSegmentIntersection(const std::vector<glm::vec3>& polygon,
					const std::vector<glm::vec3>& collinearPoints, const VectorMath::PointType pointType);
				bool DoLineSegmentsIntersection(const std::vector<glm::vec3>& collinearPointsA,
					const std::vector<glm::vec3>& collinearPointsB, const VectorMath::PointType pointType);

				bool DoCoplanarPolygonsIntersect(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2,
					const VectorMath::PointType pointType);
				bool DoPolygonsSideIntersection(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2,
					const VectorMath::PointType pointType);

				bool DoPolygonToAxisIntersection(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2,
					const glm::vec3& axisA, const glm::vec3& axisB,
					const VectorMath::PointType pointType);

				std::pair<glm::vec3, glm::vec3> ProjectPolygonToAxis(const std::vector<glm::vec3>& polygon,
					const glm::vec3& axisPointA, const glm::vec3& axisPointB);
				bool DoColinearLineSegmentsIntersect(const float& factorA, const float& factorB,
					const float& factorC, const float& factorD);
				bool DoSegmentsIntersect(const float& factorA, const float& factorB,
					const float& factorC, const float& factorD);
			}
		}
	}
}

#endif //_MY_CODE_INTERSECTION_2D_POLLIGON_INTERSECTION_H_