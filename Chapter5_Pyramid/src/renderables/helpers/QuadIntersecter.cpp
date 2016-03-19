#include "QuadIntersecter.h"

namespace MyCode
{
	QuadIntersecter::QuadIntersecter(const Quadrilateral& quadrilateral, const glm::vec4& quadNormal)
		: mQuadrilateral(quadrilateral)
		, mQuadNormal(quadNormal)
		, mModelToCameraInverse(1.0f)
		, mCameraToClipInverse(1.0f)
	{
		
	}

	QuadIntersecter::~QuadIntersecter()
	{}

	bool QuadIntersecter::GetPointOnQuad(const int screenX, const int screenY, glm::vec4& pointOnQuad)
	{
		glm::vec4 ndcPoint = GetPointInNDCFromScreenSpace(static_cast<float>(screenX), static_cast<float>(screenY));
		glm::vec4 linePoint = GetPositionInWorldSpaceFromNDC(ndcPoint);
		glm::vec4 cameraPosition = GetPositionInWorldSpaceFromCameraSpace(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glm::vec4 lineDirection = linePoint - cameraPosition;

		pointOnQuad = GetIntersectionPointWithPlane(linePoint, lineDirection);
		bool isInsideQuad = ((pointOnQuad.w != 0.0f) &&
			(Contains(mQuadrilateral, pointOnQuad)));
		return isInsideQuad;
	}

	glm::vec4 QuadIntersecter::GetPointInNDCFromScreenSpace(const float screenX, const float screenY)
	{
		const float halfScreenWidth = mScreenWidth / 2.0f;
		const float halfScreenHeight = mScreenHeight / 2.0f;
		glm::vec4 ndcPoint;
		ndcPoint.x = (screenX - halfScreenWidth) / halfScreenWidth;
		ndcPoint.y = (halfScreenHeight - screenY) / halfScreenHeight;

		return ndcPoint;
	}

	glm::vec4 QuadIntersecter::GetPositionInWorldSpaceFromNDC(const glm::vec4& ndcPoint)
	{
		glm::vec4 screenPoint(ndcPoint.x, ndcPoint.y, ndcPoint.z, 1.0f);
		glm::vec4 worldPoint = mModelToCameraInverse * mCameraToClipInverse * screenPoint;

		worldPoint /= worldPoint.w;

		return worldPoint;
	}

	glm::vec4 QuadIntersecter::GetPositionInWorldSpaceFromCameraSpace(const glm::vec4& cameraPoint)
	{
		glm::vec4 worldPoint = mModelToCameraInverse * cameraPoint;
		return worldPoint;
	}

	glm::vec4 QuadIntersecter::GetIntersectionPointWithPlane(const glm::vec4& linePoint, const glm::vec4& lineDirection)
	{
		glm::vec4 intersectionPoint;

		float factor = 0.0f;
		const float planeDotLine = glm::dot(mQuadNormal, lineDirection);
		if (planeDotLine != 0.0f)
		{
			glm::vec4 planeToLineVector = linePoint - mQuadrilateral[0];
			factor = -(glm::dot(mQuadNormal, planeToLineVector) / planeDotLine);
			intersectionPoint = linePoint + factor * lineDirection;
		}
		return intersectionPoint;
	}
}