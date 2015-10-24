#ifndef _MY_CODE_TRANSFORMATIONS_SCENE_H_
#define _MY_CODE_TRANSFORMATIONS_SCENE_H_

#include <memory>
#include <vector>
#include <string>
#include <glload/gl_3_3.h>
#include <glm/glm.hpp>
#include "programs/PosColorProgram.h"
#include "programs/BasicImpostorProgram.h"
#include "MyMesh.h"

namespace glutil
{
	class MatrixStack;
}

namespace MyCode
{
	struct ProjectionBlock
	{
		glm::mat4 mCameraToClipMatrix;
	};

	struct PerLight
	{
		glm::vec4 mCameraSpaceLightPos;
		glm::vec4 mLightIntensity;
	};

	struct LightBlock
	{
		static const int NUMBER_OF_LIGHTS = 3;

		glm::vec4 mAmbientIntensity;
		float mLightAttenuation;
		float _padding_[3];
		PerLight mLights[NUMBER_OF_LIGHTS];
	};

	struct MaterialBlock
	{
		glm::vec4 mDiffuseColor;
		glm::vec4 mSpecularColor;
		float mSpecularShininess;
		float _padding_[3];
	};

	enum MaterialNames
	{
		MTL_TERRAIN = 0,
		MTL_BLUE_SHINY,
		MTL_GOLD_METAL,
		MTL_DULL_GREY,
		MTL_BLACK_SHINY,

		NUM_MATERIALS,
	};

	class Scene
	{
	public:
		Scene();
		~Scene();

		void Render();
		void Reshape(GLint width, GLint height);

		void HandleInput(unsigned char key, int x, int y);
		void OnMouseClick(int button, int state, int x, int y);
		void OnMouseMoved(int x, int y);

		static Scene* GetInstance() { return mInstance; }
	private:
		void ConfigureOpenGL();
		void ConfigureInput();

		void UpdateControls();

		void UpdateCameraToClipMatrix();
		void UploadCameraToClipToOpenGL();

		void RenderPlane(glutil::MatrixStack& modelMatrix);
		void Scene::DrawSphere(glutil::MatrixStack &modelMatrix,
			const glm::vec3 &position, float radius, MaterialNames material);

		void InitUniformBuffers();
		void CreateMaterials();
	private:
		static Scene* mInstance;

		static const float NEAR_Z;
		static const float FAR_Z;
		static const float FOV_ANGLE;

		static const int MATERIAL_BLOCK_INDEX = 0;
		static const int LIGHT_BLOCK_INDEX = 1;
		static const int PROJECTION_BLOCK_INDEX = 2;

		static const float HALF_LIGHT_DISTANCE;
		static const float LIGHT_ATTENUATION;
		
	private:
		PosColorProgram mPosColorProgram;
		BasicImpostorProgram mImpostorProgram;
		Mesh dPlaneMesh;
		GLint mScreenWidth;
		GLint mScreenHeight;
		glm::mat4 mCameraToClipMatrix;
		GLuint mLightUniformBuffer;
		GLuint mProjectionUniformBuffer;
		GLuint mImpostorVao;
		GLuint mMaterialUniformBuffer;
		GLint mMaterialBlockOffset;
	};
}

#endif //_MY_CODE_TRANSFORMATIONS_SCENE_H_