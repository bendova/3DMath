#include "DemoTriangleIntersection.h"
#include "TriangleIntersection.h"
#include "../renderables/helpers/TriangleRenderer.h"

namespace MyCode
{
	using namespace MathUtil;

	DemoTriangleIntersection::DemoTriangleIntersection(TriangleRenderer& renderer)
		: mTriangleRenderer(renderer)
		, mColorA(1.0f, 0.0f, 0.0f, 1.0f)
		, mColorB(0.0f, 0.0f, 1.0f, 1.0f)
		, mColorIntersection(0.0f, 1.0f, 0.0f, 1.0f)
	{
		AddIntersectionAreas();
	}

	DemoTriangleIntersection::~DemoTriangleIntersection()
	{}

	void DemoTriangleIntersection::AddIntersectionAreas()
	{
		const float scaleFactor = 0.5f;
		const float offsetZ = -5.0f;
		const float spacing = 5.0f;
		float offsetX = -10.0f;

		AddTriangleArea(scaleFactor, offsetX, offsetZ);
		offsetX += spacing;
		AddRombusArea(scaleFactor, offsetX, offsetZ);
		offsetX += spacing;
		AddTrapezeArea(scaleFactor, offsetX, offsetZ);
		offsetX += spacing;
		AddPentagonArea(scaleFactor, offsetX, offsetZ);
		offsetX += spacing;
		AddHexagonArea(scaleFactor, offsetX, offsetZ);
		offsetX += spacing;
	}

	void DemoTriangleIntersection::AddTriangleArea(const float scaleFactor, const float offsetX, const float offsetZ)
	{
		Triangle a{ glm::vec4(0.0f, 3.0f, 0.0f, 1.0f), glm::vec4(3.0f, -3.0f, 0.0f, 1.0f), glm::vec4(-3.0f, -3.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f) };

		ApplyScaleFactor(a, scaleFactor);
		ApplyScaleFactor(b, scaleFactor);

		ApplyOffset(a, offsetX, 0.0f, offsetZ);
		ApplyOffset(b, offsetX, 0.0f, offsetZ);

		mTriangleRenderer.AddTrianglePoints({ a.begin(), a.end() }, mColorA);
		mTriangleRenderer.AddTrianglePoints({ b.begin(), b.end() }, mColorB);

		const auto intersectionArea = TriangleIntersection::GetIntersectionArea(a, b);
		mTriangleRenderer.AddTriangleFanPoints(intersectionArea, mColorIntersection);
	}

	void DemoTriangleIntersection::AddRombusArea(const float scaleFactor, const float offsetX, const float offsetZ)
	{
		Triangle a{ glm::vec4(-3.0f, 3.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(-3.0f, -3.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(3.0f, 3.0f, 0.0f, 1.0f), glm::vec4(3.0f, -3.0f, 0.0f, 1.0f), glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f) };

		ApplyScaleFactor(a, scaleFactor);
		ApplyScaleFactor(b, scaleFactor);

		ApplyOffset(a, offsetX, 0.0f, offsetZ);
		ApplyOffset(b, offsetX, 0.0f, offsetZ);

		mTriangleRenderer.AddTrianglePoints({ a.begin(), a.end() }, mColorA);
		mTriangleRenderer.AddTrianglePoints({ b.begin(), b.end() }, mColorB);

		const auto intersectionArea = TriangleIntersection::GetIntersectionArea(a, b);
		mTriangleRenderer.AddTriangleFanPoints(intersectionArea, mColorIntersection);
	}

	void DemoTriangleIntersection::AddTrapezeArea(const float scaleFactor, const float offsetX, const float offsetZ)
	{
		Triangle a{ glm::vec4(-4.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 4.0f, 0.0f, 1.0f), glm::vec4(4.0f, 0.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(-2.0f, 1.0f, 0.0f, 1.0f), glm::vec4(2.0f, 1.0f, 0.0f, 1.0f), glm::vec4(0.0f, -1.0f, 0.0f, 1.0f) };

		ApplyScaleFactor(a, scaleFactor);
		ApplyScaleFactor(b, scaleFactor);

		ApplyOffset(a, offsetX, 0.0f, offsetZ);
		ApplyOffset(b, offsetX, 0.0f, offsetZ);

		mTriangleRenderer.AddTrianglePoints({ a.begin(), a.end() }, mColorA);
		mTriangleRenderer.AddTrianglePoints({ b.begin(), b.end() }, mColorB);

		const auto intersectionArea = TriangleIntersection::GetIntersectionArea(a, b);
		mTriangleRenderer.AddTriangleFanPoints(intersectionArea, mColorIntersection);
	}

	void DemoTriangleIntersection::AddPentagonArea(const float scaleFactor, const float offsetX, const float offsetZ)
	{
		Triangle a{ glm::vec4(-4.0f, 5.0f, 0.0f, 1.0f), glm::vec4(2.0f, 0.0f, 0.0f, 1.0f), glm::vec4(-4.0f, -5.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(-2.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 3.0f, 0.0f, 1.0f), glm::vec4(1.0f, -3.0f, 0.0f, 1.0f) };

		ApplyScaleFactor(a, scaleFactor);
		ApplyScaleFactor(b, scaleFactor);

		ApplyOffset(a, offsetX, 0.0f, offsetZ);
		ApplyOffset(b, offsetX, 0.0f, offsetZ);

		mTriangleRenderer.AddTrianglePoints({ a.begin(), a.end() }, mColorA);
		mTriangleRenderer.AddTrianglePoints({ b.begin(), b.end() }, mColorB);

		const auto intersectionArea = TriangleIntersection::GetIntersectionArea(a, b);
		mTriangleRenderer.AddTriangleFanPoints(intersectionArea, mColorIntersection);
	}

	void DemoTriangleIntersection::AddHexagonArea(const float scaleFactor, const float offsetX, const float offsetZ)
	{
		Triangle a{ glm::vec4(-3.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 3.0f, 0.0f, 1.0f), glm::vec4(3.0f, 0.0f, 0.0f, 1.0f) };
		Triangle b{ glm::vec4(-3.0f, 2.0f, 0.0f, 1.0f), glm::vec4(3.0f, 2.0f, 0.0f, 1.0f), glm::vec4(0.0f, -1.0f, 0.0f, 1.0f) };

		ApplyScaleFactor(a, scaleFactor);
		ApplyScaleFactor(b, scaleFactor);

		ApplyOffset(a, offsetX, 0.0f, offsetZ);
		ApplyOffset(b, offsetX, 0.0f, offsetZ);

		mTriangleRenderer.AddTrianglePoints({ a.begin(), a.end() }, mColorA);
		mTriangleRenderer.AddTrianglePoints({ b.begin(), b.end() }, mColorB);

		const auto intersectionArea = TriangleIntersection::GetIntersectionArea(a, b);
		mTriangleRenderer.AddTriangleFanPoints(intersectionArea, mColorIntersection);
	}

	void DemoTriangleIntersection::ApplyScaleFactor(Triangle& t, const float scaleFactor)
	{
		for (glm::vec4& p : t)
		{
			p.x *= scaleFactor;
			p.y *= scaleFactor;
			p.z *= scaleFactor;
		}
	}

	void DemoTriangleIntersection::ApplyOffset(Triangle& t, const float offsetX, const float offsetY, const float offsetZ)
	{
		for (glm::vec4& p : t)
		{
			p.x += offsetX;
			p.y += offsetY;
			p.z += offsetZ;
		}
	}
}