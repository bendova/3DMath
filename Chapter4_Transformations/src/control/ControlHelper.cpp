#include "ControlHelper.h"
#include "GL/freeglut.h"
#include <cstdio>
#include <cmath>
#include "../framework/MathUtil.h"
#include "../intersection/IntersectionHelper.h"

namespace MyCode
{
	const float ControlHelper::PICKUP_DELTA_Y = 0.0f;

	ControlHelper::ControlHelper(const glm::vec3& initialPosition, const float sideLength, const IntersectionHelper& intersectionHelper)
		: mPosition(initialPosition)
		, mIntersectionHelper(intersectionHelper)
		, mViewTransform(1.0f)
		, mClipToCamera(1.0f)
		, mCameraToWorld(1.0f)
		, mScreenWidth(1.0f)
		, mScreenHeight(1.0f)
		, mClipNearZ(1.0f)
		, mSideLength(sideLength)
		, mInitialY(initialPosition.y)
		, mIsCubePickedUp(false)
	{}

	ControlHelper::~ControlHelper()
	{}

	bool ControlHelper::HandleKeyPress(unsigned char key)
	{
		bool isHandled = false;
		if (mIsCubePickedUp)
		{
			isHandled = MoveCube(key);
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
		const bool canPickUp = IsPointInsideCube(glm::vec3{ cubePickUpPoint });
		if (canPickUp)
		{
			PickUpCube();
		}
		return canPickUp;
	}

	glm::vec4 ControlHelper::DetermineCubePickUpPoint(const int screenX, const int screenY)
	{
		const auto ray = CastRayThroughPoint(screenX, screenY);
		const glm::vec4 cubeCenter{ mPosition, 1.0f };
		return MathUtil::GetProjectionPoint(ray.first, ray.second, cubeCenter);
	}

	bool ControlHelper::IsPointInsideCube(const glm::vec3& point) const
	{
		const float distance = glm::length(point - mPosition);
		//FIXME replace this with a proper object(cube) intersection 
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
		const float INC = 1.0f;
		const float SHIFT_INC = 0.1f * INC;
		glm::vec3 positionDelta{ 0.0f, 0.0f, 0.0f };
		switch (key)
		{
		case 'i':
			positionDelta.z -= INC;
			break;
		case 'k':
			positionDelta.z += INC;
			break;
		case 'j':
			positionDelta.x -= INC;
			break;
		case 'l':
			positionDelta.x += INC;
			break;
		case 'o':
			positionDelta.y -= INC;
			break;
		case 'u':
			positionDelta.y += INC;
			break;
		case 'I':
			positionDelta.z -= SHIFT_INC;
			break;
		case 'K':
			positionDelta.z += SHIFT_INC;
			break;
		case 'J':
			positionDelta.x -= SHIFT_INC;
			break;
		case 'L':
			positionDelta.x += SHIFT_INC;
			break;
		case 'O':
			positionDelta.y -= SHIFT_INC;
			break;
		case 'U':
			positionDelta.y += SHIFT_INC;
			break;
		default:
			isHandled = false;
			break;
		}
		// Rotate positionDelta so that the movement
		// is in the view direction.
		UpdatePosition(mViewTransform * positionDelta);

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
		const glm::vec4 cubeCenter{ mPosition, 1.0f };
		const glm::vec4 cubePlaneNormal{0.0f, 1.0f, 0.0f, 1.0f};

		const auto ray = CastRayThroughPoint(screenX, screenY);
		const glm::vec4 newCenter = GetIntersectionOfLineWithPlane(ray.first, ray.second, cubeCenter, cubePlaneNormal);
		return newCenter;
	}

	glm::vec4 ControlHelper::GetIntersectionOfLineWithPlane(const glm::vec4& lineA, const glm::vec4& lineB, 
		const glm::vec4& planePoint, const glm::vec4& planeNormal)
	{
		glm::vec4 intersectionPoint; 

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
			intersectionPoint = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f }; // at infinity
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
			SetPosition(glm::vec3{ mPosition.x, mInitialY, mPosition.z });
		}
	}

	void ControlHelper::UpdatePosition(const glm::vec3& delta)
	{
		SetPosition(mPosition + delta);
	}

	void ControlHelper::SetPosition(const glm::vec3& position)
	{
		if (mPosition != position)
		{
			mPosition = mIntersectionHelper.GetValidPosition(*this, position);
		}
	}
}