#include "ControlHelper.h"
#include "GL/freeglut.h"
#include <cstdio>
#include <cmath>
#include "../framework/VectorMath.h"

namespace MyCode
{
	const float ControlHelper::PICKUP_DELTA_Y = 0.0f;

	ControlHelper::ControlHelper(const Rectangle& boundingBox, RectangleColider& intersectionHelper)
		: mColisionHelper(intersectionHelper)
		, mBoundingRectangle(boundingBox)
		, mViewTransform(1.0f)
		, mClipToCamera(1.0f)
		, mCameraToWorld(1.0f)
		, mScreenWidth(1.0f)
		, mScreenHeight(1.0f)
		, mClipNearZ(1.0f)
		, mInitialY(boundingBox.Center().y)
		, mIsCubePickedUp(false)
		, mIsCubeSelected(false)
	{}

	ControlHelper::~ControlHelper()
	{}

	bool ControlHelper::HandleKeyPress(unsigned char key)
	{
		bool isHandled = false;
		if (mIsCubePickedUp || mIsCubeSelected)
		{
			isHandled = MoveCube(key);

			if (key == 'y')
			{
				mIsCubeSelected = !mIsCubeSelected;
				isHandled = true;
			}
		}
		return isHandled;
	}

	bool ControlHelper::HandleMouseClick(int button, int state, const int screenX, const int screenY)
	{
		bool isHandled = false;
		if (button == GLUT_LEFT_BUTTON)
		{
			switch (state)
			{
			case GLUT_DOWN:
				isHandled = TryToPickUpCube(screenX, screenY);
				break;
			case GLUT_UP:
				ReleaseCube();
				break;
			}
		}
		return isHandled;
	}

	bool ControlHelper::HandleMouseMoved(const int screenX, const int screenY)
	{
		if (mIsCubePickedUp)
		{
			MoveCube(screenX, screenY);
		}
		return false;
	}

	bool ControlHelper::TryToPickUpCube(const int screenX, const int screenY)
	{
		const auto cubePickUpPoint = DetermineCubePickUpPoint(screenX, screenY);
		const bool canPickUp = IsPointRoughlyInsideCube(glm::vec3{ cubePickUpPoint });
		if (canPickUp)
		{
			PickUpCube();
		}
		return canPickUp;
	}

	glm::vec4 ControlHelper::DetermineCubePickUpPoint(const int screenX, const int screenY)
	{
		const auto ray = CastRayThroughPoint(screenX, screenY);
		const glm::vec4 cubeCenter{ mBoundingRectangle.Center(), 1.0f };
		return VectorMath::GetProjectionPointOnLine(ray.first, ray.second, cubeCenter);
	}

	bool ControlHelper::IsPointRoughlyInsideCube(const glm::vec3& point) const
	{
		const float distance = glm::length(point - mBoundingRectangle.Center());
		return (distance <= GetRadius());
	}

	void ControlHelper::PickUpCube()
	{
		if (mIsCubePickedUp == false)
		{
			mIsCubePickedUp = true;
			UpdatePosition(glm::vec3{ 0.0f, PICKUP_DELTA_Y, 0.0f });
		}
	}

	bool ControlHelper::MoveCube(unsigned char key)
	{
		bool isHandled = true;
		const float DELTA = 1.0f;
		const float SHIFT_DELTA = 0.1f * DELTA;
		glm::vec3 positionDelta{ 0.0f, 0.0f, 0.0f };
		switch (key)
		{
		case 'i':
			positionDelta.z -= DELTA;
			break;
		case 'k':
			positionDelta.z += DELTA;
			break;
		case 'j':
			positionDelta.x -= DELTA;
			break;
		case 'l':
			positionDelta.x += DELTA;
			break;
		case 'o':
			positionDelta.y -= DELTA;
			break;
		case 'u':
			positionDelta.y += DELTA;
			break;
		case 'I':
			positionDelta.z -= SHIFT_DELTA;
			break;
		case 'K':
			positionDelta.z += SHIFT_DELTA;
			break;
		case 'J':
			positionDelta.x -= SHIFT_DELTA;
			break;
		case 'L':
			positionDelta.x += SHIFT_DELTA;
			break;
		case 'O':
			positionDelta.y -= SHIFT_DELTA;
			break;
		case 'U':
			positionDelta.y += SHIFT_DELTA;
			break;
		default:
			isHandled = false;
			break;
		}
		// Rotate positionDelta so that the movement
		// is in the view direction.
		//UpdatePosition(mViewTransform * positionDelta);
		UpdatePosition(positionDelta);

		return isHandled;
	}

	void ControlHelper::MoveCube(const int screenX, const int screenY)
	{
		const auto newCubeCenterPosition = DetermineNewCubeCenter(screenX, screenY);
		if (newCubeCenterPosition.w != 0.0f)
		{
			SetPosition(glm::vec3{ newCubeCenterPosition.x, GetPosition().y, newCubeCenterPosition.z });
		}
	}

	glm::vec4 ControlHelper::DetermineNewCubeCenter(const int screenX, const int screenY)
	{
		const glm::vec4 cubeCenter{ mBoundingRectangle.Center(), 1.0f };
		const glm::vec4 cubePlaneNormal{0.0f, 1.0f, 0.0f, 1.0f};

		const auto ray = CastRayThroughPoint(screenX, screenY);
		const VectorMath::MarginPoint<glm::vec4> rayOrigin{ ray.first };
		const VectorMath::MarginPoint<glm::vec4> rayPoint{ ray.second, VectorMath::MarginPoint<glm::vec4>::UNBOUNDED };
		const auto intersection = VectorMath::GetIntersectionOfLineWithPlane3D(rayOrigin, rayPoint, cubeCenter, cubePlaneNormal);
		return intersection.first;
	}

	glm::vec4 ControlHelper::GetIntersectionOfLineWithPlane(const glm::vec4& lineA, const glm::vec4& lineB, 
		const glm::vec4& planePoint, const glm::vec4& planeNormal)
	{
		glm::vec4 intersectionPoint{0.0f};

		const auto vectorPlaneToLine = planePoint - lineA;
		const auto lineDirection = lineB - lineA;
		const auto numerator = glm::dot(planeNormal, vectorPlaneToLine);
		const auto denominator = glm::dot(planeNormal, lineDirection);
		if (numerator == 0.0f)
		{
			// the line is in the plane
			intersectionPoint = lineA;
		}
		else if (denominator == 0.0f)
		{
			// the line is parallel with the plane, and never shall the two meet
		}
		else
		{
			const float factor = numerator / denominator;
			intersectionPoint = lineA + factor * lineDirection;
		}
		return intersectionPoint;
	}

	std::pair<glm::vec4, glm::vec4> ControlHelper::CastRayThroughPoint(const int screenX, const int screenY)
	{
		const glm::vec4 eyeInWorldSpace = mCameraToWorld * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		const glm::vec4 pointInClipSpace = GetPointInClipSpaceFromScreenSpace(screenX, screenY);
		const glm::vec4 pointInWorldSpace = mCameraToWorld * mClipToCamera * pointInClipSpace;

		return std::make_pair(eyeInWorldSpace, pointInWorldSpace);
	}

	glm::vec4 ControlHelper::GetPointInClipSpaceFromScreenSpace(const int screenX, const int screenY)
	{
		const float halfScreenWidth = mScreenWidth / 2.0f;
		const float halfScreenHeight = mScreenHeight / 2.0f;
		const float ndcX = (static_cast<float>(screenX)-halfScreenWidth) / halfScreenWidth;
		const float ndcY = (halfScreenHeight - static_cast<float>(screenY)) / halfScreenHeight;
		
		const glm::vec4 pointInClipSpace{ ndcX * mClipNearZ, ndcY * mClipNearZ, -mClipNearZ, mClipNearZ };
		return pointInClipSpace;
	}

	void ControlHelper::ReleaseCube()
	{
		if (mIsCubePickedUp)
		{
			mIsCubePickedUp = false;
			SetPosition(glm::vec3{ mBoundingRectangle.Center().x, mInitialY, mBoundingRectangle.Center().z });
		}
	}

	void ControlHelper::UpdatePosition(const glm::vec3& delta)
	{
		SetPosition(mBoundingRectangle.Center() + delta);
	}

	void ControlHelper::SetPosition(const glm::vec3& position)
	{
		if (mBoundingRectangle.Center() != position)
		{
			mBoundingRectangle.SetCenter(mColisionHelper.GetPositionThatAvoidsCollisions(mBoundingRectangle, position));
		}
	}
}