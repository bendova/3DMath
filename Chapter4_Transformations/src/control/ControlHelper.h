#ifndef _MY_CODE_CONTROL_HELPER_H_
#define _MY_CODE_CONTROL_HELPER_H_

#include <glm/glm.hpp>
#include <utility>
#include "GL/freeglut.h"
#include <cstdio>
#include <cmath>
#include "../intersection2d/RectangleColider.h"
#include "vectormath/Projection.h"
#include "vectormath/Intersection.h"

namespace MyCode
{
	template<typename Bounding, typename Collider>
	class BoundingCollider
	{
	public:
		BoundingCollider(const Bounding& bounding, const Collider& collider)
			: mBoundingBox(bounding)
			, mCollider(collider)
		{}
		~BoundingCollider()
		{}

		Bounding& BoundingBox()
		{
			return mBoundingBox;
		}
		const Bounding& BoundingBox() const
		{
			return mBoundingBox;
		}
		const Collider& GetCollider() const
		{
			return mCollider;
		}
		
	private:
		Bounding mBoundingBox;
		const Collider& mCollider;
	};

	template<typename Bounding, typename Collider>
	class ControlHelper
	{
	public:
		ControlHelper(BoundingCollider<Bounding, Collider>& boundCollider)
			: mBoundCollider(boundCollider)
			, mViewTransform(1.0f)
			, mClipToCamera(1.0f)
			, mCameraToWorld(1.0f)
			, mScreenWidth(1.0f)
			, mScreenHeight(1.0f)
			, mClipNearZ(1.0f)
			, mInitialY(boundCollider.BoundingBox().Center().y)
			, mIsCubePickedUp(false)
			, mIsCubeSelected(false)
		{}
		~ControlHelper() {}

		bool HandleKeyPress(unsigned char key)
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
		bool HandleMouseClick(int button, int state, int screenX, int screenY)
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
		bool HandleMouseMoved(int screenX, int screenY)
		{
			if (mIsCubePickedUp)
			{
				MoveCube(screenX, screenY);
			}
			return false;
		}

		void SetCubeSideLength(const float sideLength)
		{
			mSideLength = sideLength;
		}
		void SetClipNearZ(const float zNear)
		{
			mClipNearZ = zNear;
		}
		void SetScreenDimensions(const int width, const int height)
		{
			mScreenWidth = static_cast<float>(width);
			mScreenHeight = static_cast<float>(height);
		}
		void SetCameraToClipTransfrom(const glm::mat4& cameraToClip)
		{
			mClipToCamera = glm::inverse(cameraToClip);
		}
		void SetWorldToCameraTransfrom(const glm::mat4& worldToCamera)
		{
			mCameraToWorld = glm::inverse(worldToCamera);
			SetViewTransform(glm::mat3(worldToCamera));
		}
		void SetViewTransform(const glm::mat3& viewTransform)
		{
			// consider only the rotation around the Y axis
			mViewTransform[0].x = viewTransform[0].x;
			mViewTransform[0].z = -viewTransform[0].z;
			mViewTransform[2].x = -viewTransform[2].x;
			mViewTransform[2].z = viewTransform[2].z;
		}
		
		const Bounding& GetBoundingBox() const
		{
			return mBoundCollider.BoundingBox();
		}
		const glm::vec3& GetPosition() const
		{
			return mBoundCollider.BoundingBox().Center();
		}
		const float GetRadius() const
		{
			const float sideLength = glm::length(mBoundCollider.BoundingBox().CircumbscribedCircleRadius());
			return sideLength * std::sqrt(2.0f) / 2.0f;
		}

		bool IsPointRoughlyInsideCube(const glm::vec3& pickUpPoint) const
		{
			const float distance = glm::length(pickUpPoint - GetPosition());
			return (distance <= GetRadius());
		}
		
	private:
		bool TryToPickUpCube(const int screenX, const int screenY)
		{
			const auto cubePickUpPoint = DetermineCubePickUpPoint(screenX, screenY);
			const bool canPickUp = IsPointRoughlyInsideCube(glm::vec3{ cubePickUpPoint });
			if (canPickUp)
			{
				PickUpCube();
			}
			return canPickUp;
		}
		glm::vec4 DetermineCubePickUpPoint(const int screenX, const int screenY)
		{
			const auto ray = CastRayThroughPoint(screenX, screenY);
			const glm::vec4 cubeCenter{ mBoundCollider.BoundingBox().Center(), 1.0f };
			return VectorMath::GetProjectionPointOnLine(ray.first, ray.second, cubeCenter);
		}
		void PickUpCube()
		{
			if (mIsCubePickedUp == false)
			{
				mIsCubePickedUp = true;
			}
		}
		bool MoveCube(unsigned char key)
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
		void MoveCube(const int screenX, const int screenY)
		{
			const auto newCubeCenterPosition = DetermineNewCubeCenter(screenX, screenY);
			SetPosition(glm::vec3{ newCubeCenterPosition.x, GetPosition().y, newCubeCenterPosition.z });
		}
		glm::vec3 DetermineNewCubeCenter(const int screenX, const int screenY)
		{
			glm::vec3 cubeCenter{ mBoundCollider.BoundingBox().Center() };
			const glm::vec3 upDirection{ 0.0f, 1.0f, 0.0f };

			const auto ray = CastRayThroughPoint(screenX, screenY);
			const VectorMath::MarginPoint<glm::vec3> rayOrigin{ glm::vec3{ ray.first } };
			const VectorMath::MarginPoint<glm::vec3> rayPoint{ glm::vec3{ ray.second }, 
				VectorMath::BoundingPointType::UNBOUNDED };
			const auto intersection = VectorMath::GetIntersectionOfLineWithPlane3D(rayOrigin, rayPoint, cubeCenter, upDirection);
			if (intersection.second)
			{
				cubeCenter = intersection.first;
			}
			return cubeCenter;
		}

		std::pair<glm::vec4, glm::vec4> CastRayThroughPoint(const int screenX, const int screenY)
		{
			const glm::vec4 eyeInWorldSpace = mCameraToWorld * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			const glm::vec4 pointInClipSpace = GetPointInClipSpaceFromScreenSpace(screenX, screenY);
			const glm::vec4 pointInWorldSpace = mCameraToWorld * mClipToCamera * pointInClipSpace;

			return std::make_pair(eyeInWorldSpace, pointInWorldSpace);
		}
		glm::vec4 GetPointInClipSpaceFromScreenSpace(const int screenX, const int screenY)
		{
			const float halfScreenWidth = mScreenWidth / 2.0f;
			const float halfScreenHeight = mScreenHeight / 2.0f;
			const float ndcX = (static_cast<float>(screenX)-halfScreenWidth) / halfScreenWidth;
			const float ndcY = (halfScreenHeight - static_cast<float>(screenY)) / halfScreenHeight;

			const glm::vec4 pointInClipSpace{ ndcX * mClipNearZ, ndcY * mClipNearZ, -mClipNearZ, mClipNearZ };
			return pointInClipSpace;
		}
		void ReleaseCube()
		{
			if (mIsCubePickedUp)
			{
				mIsCubePickedUp = false;
				SetPosition(glm::vec3{ GetPosition().x, mInitialY, GetPosition().z });
			}
		}
		void UpdatePosition(const glm::vec3& delta)
		{
			SetPosition(GetPosition() + delta);
		}
		void SetPosition(const glm::vec3& position)
		{
			if (GetPosition() != position)
			{
				Bounding& boundingBox = mBoundCollider.BoundingBox();
				const auto validCenter = mBoundCollider.GetCollider().GetPositionThatAvoidsCollisions(boundingBox, position);
				boundingBox.SetCenter(validCenter);
			}
		}
		
		BoundingCollider<Bounding, Collider> mBoundCollider;
		glm::mat3 mViewTransform;
		glm::mat4 mClipToCamera;
		glm::mat4 mCameraToWorld;
		float mScreenWidth;
		float mScreenHeight;
		float mClipNearZ;
		float mSideLength;
		const float mInitialY;
		bool mIsCubePickedUp;
		bool mIsCubeSelected;
	};
}


#endif //_MY_CODE_CONTROL_HELPER_H_