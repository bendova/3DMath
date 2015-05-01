#ifndef _MY_CODE_RENDERABLES_HELPERS_QUAD_INTERSECTER_H_
#define _MY_CODE_RENDERABLES_HELPERS_QUAD_INTERSECTER_H_

#include <glload/gl_3_3.h>
#include <glm/glm.hpp>
#include "../../framework/MathUtil.h"

namespace MyCode
{
	using namespace MathUtil;
	class QuadIntersecter
	{
	public:
		QuadIntersecter(const Quadrilateral& quadrilateral, const glm::vec4& quadNormal);
		~QuadIntersecter();

		void SetCameraToClipMatrixInverse(const glm::mat4& cameraToClipInverse) { mCameraToClipInverse = cameraToClipInverse; }
		void SetModelToCameraInverse(const glm::mat4& modelToCameraInverse) { mModelToCameraInverse = modelToCameraInverse; }
		void SetScreenDimensions(const GLint w, const GLint h) { mScreenWidth = w; mScreenHeight = h; }

		bool GetPointOnQuad(const int screenX, const int screenY, glm::vec4& pointOnQuad);

	private:
		glm::vec4 GetPointInNDCFromScreenSpace(const float screenX, const float screenY);
		glm::vec4 GetPositionInWorldSpaceFromNDC(const glm::vec4& ndcPoint);
		glm::vec4 GetPositionInWorldSpaceFromCameraSpace(const glm::vec4& cameraPoint);
		glm::vec4 GetIntersectionPointWithPlane(const glm::vec4& linePoint, const glm::vec4& lineDirection);

		glm::mat4 mCameraToClipInverse;
		glm::mat4 mModelToCameraInverse;
		GLint mScreenWidth;
		GLint mScreenHeight;
		glm::vec4 mQuadNormal;
		const Quadrilateral mQuadrilateral;
	};
}

#endif //_MY_CODE_RENDERABLES_HELPERS_QUAD_INTERSECTER_H_