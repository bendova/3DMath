#include "Scene.h"
#include "GL/freeglut.h"
#include "MousePole.h"
#include "MathUtil.h"
#include "glutil/MatrixStack.h"
#include <algorithm>
#include "UniformBlockArray.h"

namespace MyCode
{
	glutil::ViewData gInitialViewData =
	{
		glm::vec3(0.0f, 0.5f, 0.0f),
		glm::fquat(1.0f, 0.0f, 0.0f, 0.0f),
		5.0f,
		0.0f
	};

	glutil::ViewScale gViewScale =
	{
		1.0f, 20.0f,
		1.0f, 0.1f,
		1.0f, 0.1f,
		90.0f / 250.0f
	};

	glutil::ViewPole gViewPole = glutil::ViewPole(gInitialViewData, gViewScale, glutil::MB_LEFT_BTN);

	namespace
	{
		void onMouseClick(int button, int state, int x, int y)
		{
			Scene* scene = Scene::GetInstance();
			if (scene)
			{
				scene->OnMouseClick(button, state, x, y);
			}
		}

		void onMouseMoved(int x, int y)
		{
			Scene* scene = Scene::GetInstance();
			if (scene)
			{
				scene->OnMouseMoved(x, y);
			}
		}

		void onMouseWheel(int wheel, int direction, int x, int y)
		{
			Framework::ForwardMouseWheel(gViewPole, wheel, direction, x, y);
		}
	}

	const float Scene::NEAR_Z = 0.1f;
	const float Scene::FAR_Z = 1000.f;
	const float Scene::FOV_ANGLE = 45.0f;

	const float Scene::HALF_LIGHT_DISTANCE = 25.0f;
	const float Scene::LIGHT_ATTENUATION = 1.0f / (HALF_LIGHT_DISTANCE * HALF_LIGHT_DISTANCE);

	Scene* Scene::mInstance = NULL;
	Scene::Scene()
		: mPosColorProgram("PosColor.vert", "PosColor.frag")
		, mImpostorProgram("BasicImpostor.vert", "BasicImpostor.frag")
		, dPlaneMesh("LargePlane.xml")
		, mScreenWidth(0)
		, mScreenHeight(0)
		, mCameraToClipMatrix()
	{
		mInstance = this;
		mImpostorProgram.BindUniformBlockMaterial(MATERIAL_BLOCK_INDEX);
		mImpostorProgram.BindUniformBlockLight(LIGHT_BLOCK_INDEX);
		mImpostorProgram.BindUniformBlockProjection(PROJECTION_BLOCK_INDEX);

		ConfigureOpenGL();
		InitUniformBuffers();
		CreateMaterials();

		ConfigureInput();
	}

	Scene::~Scene()
	{}

	void Scene::ConfigureOpenGL()
	{
		glClearColor(0.529411f, 0.807843f, 0.980392f, 1.0f);//man. city blue :)
		glClearDepth(1.0f);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);

		glEnable(GL_LINE_SMOOTH);
	}

	void Scene::InitUniformBuffers()
	{
		glGenBuffers(1, &mLightUniformBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, mLightUniformBuffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(LightBlock), NULL, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &mProjectionUniformBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, mProjectionUniformBuffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(ProjectionBlock), NULL, GL_DYNAMIC_DRAW);

		glBindBufferRange(GL_UNIFORM_BUFFER, LIGHT_BLOCK_INDEX, mLightUniformBuffer, 0, sizeof(LightBlock));
		glBindBufferRange(GL_UNIFORM_BUFFER, PROJECTION_BLOCK_INDEX, mProjectionUniformBuffer, 0, sizeof(ProjectionBlock));

		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glGenVertexArrays(1, &mImpostorVao);
	}

	void Scene::CreateMaterials()
	{
		Framework::UniformBlockArray<MaterialBlock, MaterialNames::NUM_MATERIALS> ubArray;
		mMaterialBlockOffset = ubArray.GetArrayOffset();

		MaterialBlock materialBlock;
		materialBlock.mDiffuseColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		materialBlock.mSpecularColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		materialBlock.mSpecularShininess = 0.6f;
		ubArray[MTL_TERRAIN] = materialBlock;

		materialBlock.mDiffuseColor = glm::vec4(0.1f, 0.1f, 0.8f, 1.0f);
		materialBlock.mSpecularColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		materialBlock.mSpecularShininess = 0.1f;
		ubArray[MTL_BLUE_SHINY] = materialBlock;

		materialBlock.mDiffuseColor = glm::vec4(0.803f, 0.709f, 0.15f, 1.0f);
		materialBlock.mSpecularColor = glm::vec4(0.803f, 0.709f, 0.15f, 1.0f) * 0.75f;
		materialBlock.mSpecularShininess = 0.18f;
		ubArray[MTL_GOLD_METAL] = materialBlock;

		materialBlock.mDiffuseColor = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
		materialBlock.mSpecularColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		materialBlock.mSpecularShininess = 0.8f;
		ubArray[MTL_DULL_GREY] = materialBlock;

		materialBlock.mDiffuseColor = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
		materialBlock.mSpecularColor = glm::vec4(0.95f, 0.95f, 0.95f, 1.0f);
		materialBlock.mSpecularShininess = 0.3f;
		ubArray[MTL_BLACK_SHINY] = materialBlock;

		mMaterialUniformBuffer = ubArray.CreateBufferObject();
	}

	void Scene::ConfigureInput()
	{
		glutMouseFunc(onMouseClick);
		glutMotionFunc(onMouseMoved);
		glutMouseWheelFunc(onMouseWheel);
	}

	void Scene::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glutil::MatrixStack modelToCameraTransform;
		modelToCameraTransform.ApplyMatrix(gViewPole.CalcMatrix());
		
		LightBlock lightData;

		lightData.mAmbientIntensity = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		lightData.mLightAttenuation = LIGHT_ATTENUATION;

		lightData.mLights[0].mCameraSpaceLightPos = modelToCameraTransform.Top() * glm::vec4(0.707f, 0.707f, 0.0f, 0.0f);
		lightData.mLights[0].mLightIntensity = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);

		lightData.mLights[1].mCameraSpaceLightPos = modelToCameraTransform.Top() * glm::vec4(0.0f, 20.0f, 0.0f, 1.0f);
		lightData.mLights[1].mLightIntensity = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);

		glBindBuffer(GL_UNIFORM_BUFFER, mLightUniformBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(lightData), &lightData);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glUseProgram(mPosColorProgram.GetProgramID());
		RenderPlane(modelToCameraTransform);
		glUseProgram(GL_NONE);

		DrawSphere(modelToCameraTransform, glm::vec3(0.0f, 10.0f, 0.0f), 4.0f, MTL_BLUE_SHINY);

		glutSwapBuffers();
		glutPostRedisplay();
	}

	void Scene::RenderPlane(glutil::MatrixStack& modelMatrix)
	{
		glutil::PushStack push(modelMatrix);

		//modelMatrix.Scale(glm::vec3{ 1.0f / 2.0f, 0.0f, 1.0f / 2.0f });
		glUniformMatrix4fv(mPosColorProgram.GetModelToCameraTransformUniform(),
			1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		dPlaneMesh.Render();
	}

	void Scene::DrawSphere(glutil::MatrixStack &modelMatrix,
		const glm::vec3 &position, float radius, MaterialNames material)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, mImpostorProgram.GetMaterialBlockIndex(), mMaterialUniformBuffer,
			material * mMaterialBlockOffset, sizeof(MaterialBlock));

		glm::vec4 cameraSpherePos = modelMatrix.Top() * glm::vec4(position, 1.0f);
		glUseProgram(mImpostorProgram.GetProgramID());
		glUniform3fv(mImpostorProgram.GetCameraSpherePosUniform(), 1, glm::value_ptr(cameraSpherePos));
		glUniform1f(mImpostorProgram.GetSphereRadiusUniform(), radius);

		glBindVertexArray(mImpostorVao);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindVertexArray(GL_NONE);
		glUseProgram(GL_NONE);

		glBindBufferBase(GL_UNIFORM_BUFFER, mImpostorProgram.GetMaterialBlockIndex(), 0);
	}

	void Scene::Reshape(GLint width, GLint height)
	{
		mScreenWidth = width;
		mScreenHeight = height;

		UpdateCameraToClipMatrix();
	}

	void Scene::UpdateCameraToClipMatrix()
	{
		const float aspectRatio = static_cast<float>(mScreenWidth) / static_cast<float>(mScreenHeight);
		glutil::MatrixStack cameraToClipTransform;
		cameraToClipTransform.Perspective(FOV_ANGLE, aspectRatio, NEAR_Z, FAR_Z);
		
		mCameraToClipMatrix = cameraToClipTransform.Top();
		
		UploadCameraToClipToOpenGL();
	}

	void Scene::UploadCameraToClipToOpenGL()
	{
		ProjectionBlock projData;
		projData.mCameraToClipMatrix = mCameraToClipMatrix;
		glBindBuffer(GL_UNIFORM_BUFFER, mProjectionUniformBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ProjectionBlock), &projData);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glUseProgram(mPosColorProgram.GetProgramID());
		glUniformMatrix4fv(mPosColorProgram.GetCameraToClipTransformUniform(),
			1, GL_FALSE, glm::value_ptr(mCameraToClipMatrix));
		glUseProgram(GL_NONE);
	}

	void Scene::HandleInput(unsigned char key, int x, int y)
	{
		gViewPole.CharPress(key);
	}

	void Scene::OnMouseClick(int button, int state, int x, int y)
	{
		Framework::ForwardMouseButton(gViewPole, button, state, x, y);
	}
	
	void Scene::OnMouseMoved(int x, int y)
	{
		Framework::ForwardMouseMotion(gViewPole, x, y);
	}

}