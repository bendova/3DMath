#include "UT_PolygonCollider.h"
#include "../framework/UTUtil.h"
#include "../framework/VectorMath.h"
#include <vector>

namespace MyCode
{
	using namespace VectorMath;
	using namespace Intersection3D;

	bool UT_PolygonCollider::Validate()
	{
		CollisionTest3D collisionTest3D;
		PathCollisionDetectionTest pathCollisionDetection;

		return collisionTest3D.Run()
			&& pathCollisionDetection.Run();
	}

	bool UT_PolygonCollider::CollisionTest3D::Run()
	{
		return NoCollision()
			&& CrossCollision();
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

	bool UT_PolygonCollider::PathCollisionDetectionTest::Run()
	{
		return NoCollision()
			&& NearSideCollision()
			&& FarSideCollision();
	}

	bool UT_PolygonCollider::PathCollisionDetectionTest::NoCollision()
	{
		const Polygon target{
			glm::vec3{ -1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 1.0f },
			glm::vec3{ 1.0f, 0.0f, -1.0f }, glm::vec3{ -1.0f, 0.0f, -1.0f } };
		const Polygon obstacle{
			glm::vec3{ 4.0f, -1.0f, 0.0f }, glm::vec3{ 6.0f, -1.0f, 0.0f },
			glm::vec3{ 6.0f, 1.0f, 0.0f }, glm::vec3{ 4.0f, 1.0f, 0.0f } };
		const glm::vec3 destination{0.0f, 0.0f, 10.0f};

		const bool collision = PathCollisionDetection::DoesPathCollide(target, destination, obstacle);
		const bool collisionExpected = false;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::PathCollisionDetectionTest::NearSideCollision()
	{
		const Polygon sourceYZ{
			glm::vec3{ 0.0f, -1.0f, -1.0f }, glm::vec3{ 0.0f, -1.0f, 1.0f },
			glm::vec3{ 0.0f, 1.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, -1.0f } };
		const Polygon obstacleXZ{
			glm::vec3{ 4.0f, 0.0f, 1.0f }, glm::vec3{ 6.0f, 0.0f, 1.0f },
			glm::vec3{ 6.0f, 0.0f, -1.0f }, glm::vec3{ 4.0f, 0.0f, -1.0f } };
		const glm::vec3 destination{ 0.0f, 0.0f, 10.0f };

		// TODO
		// This test proves that our method of simply checking the intersections between
		// the sides of the travel path bounding polygon and the sides of the obstacle
		// does not work. In this case the travel path bounding polygon crosses through
		// the obstacle, without touching any sides.
		// I propose as alternative the following:
		// - do shadows intersection between the obstacle and the polygon comprised of the
		//   source vertices and the destination vertices.
		const bool collision = PathCollisionDetection::DoesPathCollide(sourceYZ, destination, obstacleXZ);
		const bool collisionExpected = true;

		return CHECK_EQUALS(collision, collisionExpected);
	}

	bool UT_PolygonCollider::PathCollisionDetectionTest::FarSideCollision()
	{
		return CHECK_IS_TRUE(false);
	}
}