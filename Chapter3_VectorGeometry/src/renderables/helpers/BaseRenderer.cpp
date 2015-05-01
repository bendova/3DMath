#include "BaseRenderer.h"
#include "../../programs/PosColorProgram.h"

namespace MyCode
{
	const int BaseRenderer::VERTEX_BUFFER_MAX_SIZE = 2 << 20;
	const int BaseRenderer::VERTEX_COMPONENT_COUNT = 3;
	const int BaseRenderer::COLOR_COMPONENT_COUNT = 4;

	BaseRenderer::BaseRenderer(const PosColorProgram& posColorProgram)
		: mPosColorProgram(posColorProgram)
		, mVertexBufferID(GL_INVALID_INDEX)
		, mVertexArrayObjectID(GL_INVALID_INDEX)
		, mRenderPoints()
		, mWorldPoints()
	{
		InitVertexBuffer();
		InitVertexArrayObject();
	}

	BaseRenderer::~BaseRenderer()
	{}

	void BaseRenderer::InitVertexBuffer()
	{
		glGenBuffers(1, &mVertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_MAX_SIZE, NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	}

	void BaseRenderer::InitVertexArrayObject()
	{
		glGenVertexArrays(1, &mVertexArrayObjectID);
		glBindVertexArray(mVertexArrayObjectID);

		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);

		const auto stride = sizeof(ColoredPoint);

		const auto positionAttributeID = mPosColorProgram.GetPositionAttributeID();
		glEnableVertexAttribArray(positionAttributeID);
		glVertexAttribPointer(positionAttributeID, VERTEX_COMPONENT_COUNT, GL_FLOAT, GL_FALSE, stride, (void*)0);

		const auto colorOffset = VERTEX_COMPONENT_COUNT * sizeof(float);
		const auto colorAttributeID = mPosColorProgram.GetColorAttributeID();
		glEnableVertexAttribArray(colorAttributeID);
		glVertexAttribPointer(colorAttributeID, COLOR_COMPONENT_COUNT, GL_FLOAT, GL_FALSE, stride, (void*)colorOffset);

		glBindVertexArray(GL_NONE);
	}

	void BaseRenderer::AddPointToRender(const glm::vec4& worldPoint, const Color& color)
	{
		mRenderPoints.push_back(ColoredPoint(Point(worldPoint.x, worldPoint.y, worldPoint.z), color));
		mWorldPoints.push_back(worldPoint);
		UploadPointsToOpenGL();
	}

	void BaseRenderer::ReplaceLastPoint(const glm::vec4& worldPoint)
	{
		int lastPointIndex = mRenderPoints.size() - 1;
		if (lastPointIndex >= 0)
		{
			SetPoint(worldPoint, lastPointIndex);
		}
	}

	void BaseRenderer::SetPoint(const glm::vec4& worldPoint, const int pointIndex)
	{
		mRenderPoints[pointIndex] = ColoredPoint(Point(worldPoint.x, worldPoint.y, worldPoint.z), Color(1.0f));
		mWorldPoints[pointIndex] = worldPoint;
		UploadPointsToOpenGL();
	}

	void BaseRenderer::UploadPointsToOpenGL()
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
		const unsigned bufferSize = mRenderPoints.size() * sizeof(ColoredPoint);
		glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, &mRenderPoints[0]);
		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	}

	void BaseRenderer::Render()
	{
		glBindVertexArray(mVertexArrayObjectID);
		DoRender();
		glBindVertexArray(GL_NONE);
	}
}