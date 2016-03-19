#ifndef _MY_CODE_POLYGON_MATH_H_
#define _MY_CODE_POLYGON_MATH_H_

#include <array>
#include <initializer_list>
#include <glm/glm.hpp>

namespace MyCode
{
	namespace VectorMath
	{
		template<size_t POINTS_COUNT>
		class Polygon
		{
		public:
			typedef std::array<glm::vec4, POINTS_COUNT> Vertices;

		private:
			Vertices mVertices;

		public:

			Polygon(std::initializer_list<glm::vec4> vertices)
				: mVertices{}
			{
				int index = 0;
				auto itEnd = vertices.end();
				for (auto it = vertices.begin(); ((it != itEnd) && (index < POINTS_COUNT)); ++it, ++index)
				{
					mVertices[index] = *it;
				}
			}

			glm::vec4& operator[](int index) { return mVertices[index]; }
			const glm::vec4& operator[](int index) const { return mVertices[index]; }

			const Vertices& GetVertices() const { return mVertices; }
			Vertices& GetVertices() { return mVertices; }

			static
				size_t GetPointsCount() { return POINTS_COUNT; }

			auto begin() -> decltype(mVertices.begin())
			{
				return mVertices.begin();
			}

			auto end() -> decltype(mVertices.end())
			{
				return mVertices.end();
			}

			auto begin() const -> decltype(mVertices.begin())
			{
				return mVertices.begin();
			}

			auto end() const -> decltype(mVertices.end())
			{
				return mVertices.end();
			}

			auto rbegin() -> decltype(mVertices.rbegin())
			{
				return mVertices.rbegin();
			}

			auto rend() -> decltype(mVertices.rend())
			{
				return mVertices.rend();
			}

			auto rbegin() const -> decltype(mVertices.rbegin())
			{
				return mVertices.rbegin();
			}

			auto rend() const -> decltype(mVertices.rend())
			{
				return mVertices.rend();
			}

		};
		typedef Polygon<3> Triangle;
		typedef Polygon<4> Quadrilateral;

		bool Contains(const Triangle& t, const glm::vec4& p);
		bool Contains(const Quadrilateral& q, const glm::vec4& p);

		bool ContainsStrictly(const Triangle& t, const glm::vec4& p);
		bool ContainsStrictly(const Quadrilateral& q, const glm::vec4& p);
	}
}

#endif //_MY_CODE_POLYGON_MATH_H_