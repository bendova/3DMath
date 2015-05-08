#ifndef _MY_CODE_INTERSECTION_HELPER_H_
#define _MY_CODE_INTERSECTION_HELPER_H_

#include <glm/glm.hpp>
#include <vector>

namespace MyCode
{
	class ControlHelper;
	class IntersectionHelper
	{
	public:
		IntersectionHelper();
		~IntersectionHelper();

		void AddControlHelper(const ControlHelper* control);

		glm::vec3 GetValidPosition(const ControlHelper& current, glm::vec3 desired) const;

	private:
		glm::vec3 GetPointOnEdge(const ControlHelper& current, const glm::vec3& desired, const ControlHelper& intersected) const;

		std::vector<const ControlHelper*> mControlHelpers;
	};

	inline void IntersectionHelper::AddControlHelper(const ControlHelper* control)
	{
		mControlHelpers.push_back(control);
	}
}


#endif //_MY_CODE_INTERSECTION_HELPER_H_