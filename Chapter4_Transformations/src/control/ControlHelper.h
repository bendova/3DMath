#ifndef _MY_CODE_CONTROL_HELPER_H_
#define _MY_CODE_CONTROL_HELPER_H_

#include <glm/glm.hpp>
#include <utility>
#include "../intersection/RectangleColider.h"

namespace MyCode
{
	class ControlHelper
	{
	public:
		ControlHelper(const Rectangle& boundingRectangle, RectangleColider& intersectionHelper);
		~ControlHelper();

		bool HandleKeyPress(unsigned char key);
		bool HandleMouseClick(int button, int state, int x, int y);
		bool HandleMouseMoved(int x, int y);

		void SetCubeSideLength(const float sideLength);
		void SetClipNearZ(const float zNear);
		void SetScreenDimensions(const int width, const int height);
		void SetCameraToClipTransfrom(const glm::mat4& cameraToClip);
		void SetWorldToCameraTransfrom(const glm::mat4& worldToCamera);
		void SetViewTransform(const glm::mat3& viewTransform);
		
		const Rectangle& GetBoundingBox() const;
		const glm::vec3& GetPosition() const;
		const float GetRadius() const;

		bool IsPointRoughlyInsideCube(const glm::vec3& pickUpPoint) const;
	private:
		bool TryToPickUpCube(const int screenX, const int screenY);
		void PickUpCube();
		bool MoveCube(unsigned char key);
		void MoveCube(const int screenX, const int screenY);
		void ReleaseCube();
		void UpdatePosition(const glm::vec3& delta);
		void SetPosition(const glm::vec3& position);
		
		std::pair<glm::vec4, glm::vec4> CastRayThroughPoint(const int screenX, const int screenY);
		glm::vec4 GetPointInClipSpaceFromScreenSpace(const int screenX, const int screenY);

		glm::vec4 DetermineCubePickUpPoint(const int screenX, const int screenY);
		glm::vec4 DetermineNewCubeCenter(const int screenX, const int screenY);
		
		glm::vec4 GetIntersectionOfLineWithPlane(const glm::vec4& lineA, const glm::vec4& lineB,
			const glm::vec4& planePoint, const glm::vec4& planeNormal);

		RectangleColider& mColisionHelper;
		Rectangle mBoundingRectangle;
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

		static const float PICKUP_DELTA_Y;
	};

	inline void ControlHelper::SetCubeSideLength(const float sideLength)
	{
		mSideLength = sideLength;
	}
	inline void ControlHelper::SetClipNearZ(const float zNear)
	{
		mClipNearZ = zNear;
	}
	inline void ControlHelper::SetScreenDimensions(const int width, const int height)
	{
		mScreenWidth = static_cast<float>(width);
		mScreenHeight = static_cast<float>(height);
	}
	inline void ControlHelper::SetCameraToClipTransfrom(const glm::mat4& cameraToClip)
	{
		mClipToCamera = glm::inverse(cameraToClip);
	}
	inline void ControlHelper::SetWorldToCameraTransfrom(const glm::mat4& worldToCamera)
	{
		mCameraToWorld = glm::inverse(worldToCamera);
		SetViewTransform(glm::mat3(worldToCamera));
	}
	inline void ControlHelper::SetViewTransform(const glm::mat3& viewTransform)
	{
		// consider only the rotation around the Y axis
		mViewTransform[0].x = viewTransform[0].x;
		mViewTransform[0].z = -viewTransform[0].z;
		mViewTransform[2].x = -viewTransform[2].x;
		mViewTransform[2].z = viewTransform[2].z;
	}
	inline const Rectangle& ControlHelper::GetBoundingBox() const
	{
		return mBoundingRectangle;
	}
	inline const glm::vec3& ControlHelper::GetPosition() const
	{ 
		return mBoundingRectangle.Center(); 
	}
	inline const float ControlHelper::GetRadius() const
	{
		const float sideLength = glm::length(mBoundingRectangle.OffsetToA());
		return sideLength * std::sqrt(2.0f) / 2.0f;
	}
}


#endif //_MY_CODE_CONTROL_HELPER_H_