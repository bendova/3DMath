#ifndef _MY_CODE_SNOWFLAKE_H_
#define _MY_CODE_SNOWFLAKE_H_

#include <glload/gl_3_3.h>
#include <vector>
#include "Vector.h"

namespace MyCode
{
	using namespace VectorMath;

	class PosColorProgram;

	class Snowflake
	{
	public:
		Snowflake(const PosColorProgram& posColorProgram);
		~Snowflake();

		void Render() const;
		void HandleInput(unsigned char key);
	private:
		std::vector<Point>
		GenerateSnowflakeIteration(const std::vector<Point>& initialPoints, const int iterations = 1);
		std::vector<Point>
		GenerateSnowflake(const std::vector<Point>& initialPoints);
		Point GetTrianglePoint(const Point& a, const Point& b, const bool outside = true);

		void InitVertexBuffer();
		void InitVertexArrayObject();

		void GenerateSnowflake();
		void GenerateSnowflakeVertices();
		void UploadSnowflakeVerticesToOpenGL();

		void ToggleSnowflakeOrientation();
		void IncreaseSnowflakeIterations();
		void DecreaseSnowflakeIterations();

		static const int VERTEX_BUFFER_MAX_SIZE;
		static const int MIN_SNOWFLAKE_ITERATIONS;
		static const int MAX_SNOWFLAKE_ITERATIONS;
		const PosColorProgram& mPosColorProgram;
		GLuint mVertexBufferID;
		GLuint mVertexArrayObjectID;
		std::vector<Point> mPoints;
		int mSnowflakeIterations;
		bool mSnowflakeOutside;
	};
}

#endif//_MY_CODE_SNOWFLAKE_H_