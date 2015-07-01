#ifndef _MY_CODE_DEMO_TRIANGLE_INTERSECTION_H_
#define _MY_CODE_DEMO_TRIANGLE_INTERSECTION_H_

#include "MathUtil.h"
#include "../PolygonMath.h"
#include "vectormath/DataTypes.h"

namespace MyCode
{
	class TriangleRenderer;
	using namespace VectorMath;

	class DemoTriangleIntersection
	{
	public:
		DemoTriangleIntersection(TriangleRenderer& renderer);
		~DemoTriangleIntersection();

	private:
		void AddIntersectionAreas();

		void AddTriangleArea(const float scaleFactor, const float offsetX, const float offsetZ);
		void AddRombusArea(const float scaleFactor, const float offsetX, const float offsetZ);
		void AddTrapezeArea(const float scaleFactor, const float offsetX, const float offsetZ);
		void AddPentagonArea(const float scaleFactor, const float offsetX, const float offsetZ);
		void AddHexagonArea(const float scaleFactor, const float offsetX, const float offsetZ);

		void ApplyScaleFactor(Triangle& t, const float scaleFactor);
		void ApplyOffset(Triangle& t, const float offsetX,
			const float offsetY, const float offsetZ);

		Color mColorA;
		Color mColorB;
		Color mColorIntersection;
		TriangleRenderer& mTriangleRenderer;
	};
}
#endif //_MY_CODE_DEMO_TRIANGLE_INTERSECTION_H_