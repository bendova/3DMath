#include "PolygonMath.h"
#include "vectormath/Common.h"
#include "vectormath/Projection.h"

namespace MyCode
{
	namespace VectorMath
	{
		bool Contains(const Triangle& q, const glm::vec4& p)
		{
			const auto& a = q[0];
			const auto& b = q[1];
			const auto& c = q[2];

			bool result = IsPointInPlane(a, b, c, p) && IsPointInsideOfSide(a, b, c, p) && IsPointInsideOfSide(b, c, a, p) &&
				IsPointInsideOfSide(c, a, b, p);
			return result;
		}

		bool Contains(const Quadrilateral& q, const glm::vec4& p)
		{
			const auto& a = q[0];
			const auto& b = q[1];
			const auto& c = q[2];
			const auto& d = q[3];

			bool result = IsPointInPlane(a, b, c, p) && IsPointInsideOfSide(a, b, d, p) && IsPointInsideOfSide(b, c, a, p) &&
				IsPointInsideOfSide(c, d, b, p) && IsPointInsideOfSide(d, a, c, p);
			return result;
		}

		bool ContainsStrictly(const Triangle& q, const glm::vec4& p)
		{
			const auto& a = q[0];
			const auto& b = q[1];
			const auto& c = q[2];

			bool result = IsPointInPlane(a, b, c, p) && IsPointInsideOfSideStrictly(a, b, c, p)
				&& IsPointInsideOfSideStrictly(b, c, a, p) && IsPointInsideOfSideStrictly(c, a, b, p);
			return result;
		}

		bool ContainsStrictly(const Quadrilateral& q, const glm::vec4& p)
		{
			const auto& a = q[0];
			const auto& b = q[1];
			const auto& c = q[2];
			const auto& d = q[3];

			bool result = IsPointInPlane(a, b, c, p) && IsPointInsideOfSideStrictly(a, b, d, p) && IsPointInsideOfSideStrictly(b, c, a, p) &&
				IsPointInsideOfSideStrictly(c, d, b, p) && IsPointInsideOfSideStrictly(d, a, c, p);
			return result;
		}
	}
}