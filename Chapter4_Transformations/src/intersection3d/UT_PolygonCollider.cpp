#include "UT_PolygonCollider.h"
#include "../framework/UTUtil.h"
#include "../framework/VectorMath.h"
#include <vector>
#include "PolygonCollider.h"

namespace MyCode
{
	using namespace VectorMath;
	using namespace Intersection3D;

	bool UT_PolygonCollider::Validate()
	{
		CollisionTest3D collisionTest3D;

		return collisionTest3D.Run();
	}

	bool UT_PolygonCollider::CollisionTest3D::Run()
	{
		return PointToPlaneProjection() // This should be moved to UT_VectorMath
			&& PointToPlaneProjection1()
			&& NoCollision()
			&& CrossCollision();
	}

	bool UT_PolygonCollider::CollisionTest3D::PointToPlaneProjection()
	{
		const glm::vec3 pointToProject{ 1.0f, 1.0f, 1.0f };
		const glm::vec3 pointInPlane{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 planeNormal{ 0.0f, 0.0f, 1.0f };

		const glm::vec3 projectedPoint = GetProjectionPointOnPlane(pointToProject, pointInPlane, planeNormal);
		const glm::vec3 expectedPoint{ 1.0f, 1.0f, 0.0f };

		return CHECK_EQUALS(projectedPoint, expectedPoint);
	}

	bool UT_PolygonCollider::CollisionTest3D::PointToPlaneProjection1()
	{
		const glm::vec3 pointToProject{ -1.0f, -1.0f, 0.0f };
		const glm::vec3 pointInPlane{ -1.0f, 0.0f, 1.0f };
		const glm::vec3 planeNormal{ 0.0f, 4.0f, 0.0f };

		const glm::vec3 projectedPoint = GetProjectionPointOnPlane(pointToProject, pointInPlane, planeNormal);
		const glm::vec3 expectedPoint{ -1.0f, 0.0f, 0.0f };

		return CHECK_EQUALS(projectedPoint, expectedPoint);
	}

	bool UT_PolygonCollider::CollisionTest3D::NoCollision()
	{
		const std::vector<glm::vec3> rectangle1{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangle2{
			glm::vec3{ -1.0f, 1.0f, 1.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f },
			glm::vec3{ 1.0f, 1.0f, -1.0f }, glm::vec3{ -1.0f, 1.0f, -1.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect3D(rectangle1, rectangle2);
		const bool collisionExpected = false;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::CollisionTest3D::CrossCollision()
	{
		const std::vector<glm::vec3> rectangleInXZ{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const std::vector<glm::vec3> rectangleInXY{
			glm::vec3{ -1.0f, -1.0f, 0.0f }, glm::vec3{ 1.0f, -1.0f, 0.0f },
			glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec3{ -1.0f, 1.0f, 0.0f } };

		const bool collision = PolygonIntersection::DoPolygonsIntersect3D(rectangleInXZ, rectangleInXY);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}
}