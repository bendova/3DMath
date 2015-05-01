#ifndef _MY_CODE_TRIANGLE_INTERSECTION_H_
#define _MY_CODE_TRIANGLE_INTERSECTION_H_

#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include <algorithm>
#include <unordered_set>
#include "../framework/MathUtil.h"

namespace MyCode
{
	using namespace MathUtil;
	class TriangleIntersection
	{
		friend class TriangleVerticesIntersectionTest;

	public:
		typedef glm::vec4 Point;
		typedef std::vector<Point> Area;
		typedef std::vector<Point> PointsList;

		static Area GetIntersectionArea(const Triangle& triangleA, const Triangle& triangleB);
	private:
		static PointsList GetIntersectionPoints(const Triangle& triangle, const glm::vec4& segmentStart, const glm::vec4& segmentEnd);

		TriangleIntersection();
		~TriangleIntersection();
	};
}

namespace std
{
	template<>
	struct hash<MyCode::TriangleIntersection::Point>
		: public _Bitwise_hash < MyCode::TriangleIntersection::Point >
	{	// hash functor for glm::vec4
	};
}

namespace MyCode
{
	template<typename T, typename H = std::hash<_Kty>>
	class UnorderedSet
	{
	private:
		std::vector<const T*> mInsertionOrder;
		std::unordered_set<T, H> mElements;

	public:
		typedef T ElementType;

		UnorderedSet() {}
		UnorderedSet(UnorderedSet& other)
			: mElements(other.mElements)
			, mInsertionOrder(other.mInsertionOrder)
		{
		}
		UnorderedSet(UnorderedSet&& other)
			: mElements(std::forward<decltype(other.mElements)>(other.mElements))
			, mInsertionOrder(std::forward<decltype(other.mInsertionOrder)>(other.mInsertionOrder))
		{
		}
		~UnorderedSet() {}

		void insert(const T& value)
		{
			auto result = mElements.insert(value);
			if (result.second)
			{
				const T* t = &(*(result.first));
				mInsertionOrder.push_back(t);
			}
		}

		template<typename It>
		void insert(It& begin, const It& end)
		{
			while (begin != end)
			{
				insert(*begin++);
			}
		}

		auto begin() const -> decltype(mInsertionOrder.begin())
		{
			return mInsertionOrder.begin();
		}

		auto end() const -> decltype(mInsertionOrder.end())
		{
			return mInsertionOrder.end();
		}

		auto rbegin() const -> decltype(mInsertionOrder.rbegin())
		{
			return mInsertionOrder.rbegin();
		}

		auto rend() const -> decltype(mInsertionOrder.rend())
		{
			return mInsertionOrder.rend();
		}

		size_t size() const
		{
			return mInsertionOrder.size();
		}

		void clear()
		{
			mElements.clear();
			mInsertionOrder.clear();
		}
	};
}

#endif //_MY_CODE_TRIANGLE_INTERSECTION_H_