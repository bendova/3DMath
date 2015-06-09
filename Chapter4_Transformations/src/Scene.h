#ifndef _MY_CODE_TRANSFORMATIONS_SCENE_H_
#define _MY_CODE_TRANSFORMATIONS_SCENE_H_

#include <memory>
#include <vector>
#include <string>
#include <glload/gl_3_3.h>
#include <glm/glm.hpp>
#include "Scene.h"
#include "programs/PosColorProgram.h"
#include "control/ControlHelper.h"
#include "intersection2d/RectangleColider.h"
#include "intersection3d/CubeCollider.h"
#include "../framework/MyMesh.h"

namespace glutil
{
	class MatrixStack;
}

namespace MyCode
{
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
		template<typename Bounding, typename Collider>
		struct ControlledMesh
		{
			ControlledMesh(const std::string& mesh, const Bounding& boundingBox, Collider& collider)
				: mMesh(mesh)
				, mControl(BoundingCollider<Bounding, Collider>{ boundingBox, collider })
			{}

			ControlledMesh(ControlledMesh&& other)
				: mMesh{ std::forward<Mesh>(other.mMesh) }
				, mControl{ std::forward<ControlHelper<Bounding, Collider>>(other.mControl) }
			{}

			Mesh mMesh;
			ControlHelper<Bounding, Collider> mControl;
		};
		typedef ControlledMesh<Rectangle, RectangleColider> ControlledRectangle;
		typedef ControlledMesh<Cube, CubeCollider> ControlledCube;

		void InitScene2D();
		void InitRectangles();
		void InitColliderForPolygons2D();
		
		void InitScene3D();
		void InitCubes();
		void InitColliderForPolygons3D();

		void RenderPlane(glutil::MatrixStack& modelMatrix);
		void RenderRectangles(glutil::MatrixStack& modelMatrix);
		void RenderCubes(glutil::MatrixStack& modelMatrix);

		template<typename Bounding, typename Collider>
		void RenderMesh(glutil::MatrixStack& modelMatrix, ControlledMesh<Bounding, Collider>& controlledMesh)
		{
			glutil::PushStack push(modelMatrix);

			controlledMesh.mControl.SetWorldToCameraTransfrom(modelMatrix.Top());

			modelMatrix.Translate(controlledMesh.mControl.GetPosition());
			glUniformMatrix4fv(mPosColorProgram.GetModelToCameraTransformUniform(),
				1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
			controlledMesh.mMesh.Render();
		}

		void ConfigureOpenGL();
		void ConfigureInput();

		void UpdateControls();

		template<typename Bounding, typename Collider>
		void UpdateControl(ControlledMesh<Bounding, Collider>& controlledMesh)
		{
			controlledMesh.mControl.SetCameraToClipTransfrom(mCameraToClipMatrix);
			controlledMesh.mControl.SetClipNearZ(mNearZ);
			controlledMesh.mControl.SetScreenDimensions(mScreenWidth, mScreenHeight);
		}
		void UpdateCameraToClipMatrix();
		void UploadCameraToClipToOpenGL();

		bool ForwardKeyboardInput(unsigned char key);

		template<typename Bounding, typename Collider>
		bool ForwardKeybardInput(unsigned char key, std::vector<ControlledMesh<Bounding, Collider>>& meshes)
		{
			bool isHandled = false;
			for (auto& c : meshes)
			{
				isHandled = c.mControl.HandleKeyPress(key);
				if (isHandled)
				{
					break;
				}
			}

			return isHandled;
		}

		bool ForwardMouseClick(int button, int state, int x, int y);

		template<typename Bounding, typename Collider>
		bool ForwardMouseClick(int button, int state, int x, int y, std::vector<ControlledMesh<Bounding, Collider>>& meshes)
		{
			bool isHandled = false;
			for (auto& c : meshes)
			{
				isHandled = c.mControl.HandleMouseClick(button, state, x, y);
				if (isHandled)
				{
					break;
				}
			}

			return isHandled;
		}

		bool ForwardMouseMove(int x, int y);

		template<typename Bounding, typename Collider>
		bool ForwardMouseMove(int x, int y, std::vector<ControlledMesh<Bounding, Collider>>& meshes)
		{
			bool isHandled = false;
			for (auto& c : meshes)
			{
				isHandled = c.mControl.HandleMouseMoved(x, y);
				if (isHandled)
				{
					break;
				}
			}

			return isHandled;
		}

		static Scene* mInstance;
		PosColorProgram mPosColorProgram;
		Mesh dPlaneMesh;
		GLint mScreenWidth;
		GLint mScreenHeight;
		glm::mat4 mCameraToClipMatrix;
		std::vector<ControlledRectangle> mRectangles;
		std::vector<ControlledCube> mCubes;
		RectangleColider mRectangleCollider;
		CubeCollider mCubeCollider;
	};
}

#endif //_MY_CODE_TRANSFORMATIONS_SCENE_H_