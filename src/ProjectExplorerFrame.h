#ifndef PROJECT_EXPLORER_FRAME_HEADER
#define PROJECT_EXPLORER_FRAME_HEADER

#include "SpFrame.h"
#include "SpProjectManager.h"
#include "SpScene.h"
#include "NewSceneFrame.h"
#include "SpShader.h"
#include "SpGame.h"
#include "SpGameObjectFactoryPlane.h"

namespace NAMESPACE_FRONTEND
{
	class ProjectExplorerFrame
		: public SpFrame
	{
	private:
		NewSceneFrame newSceneFrame;

		void renderScenesContextMenu()
		{
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("New Scene"))
				{
					newSceneFrame.show();
				}
				ImGui::EndPopup();
			}
		}

		void renderSceneNode(SpVectorItem<SpScene*>* sceneItem, const sp_bool canDeleteScene);
		void renderSceneContextMenu(SpVectorItem<SpScene*>* sceneItem, const sp_bool canDeleteScene)
		{
			if (ImGui::BeginPopupContextItem())
			{
				if (canDeleteScene)
				{
					if (ImGui::MenuItem("Delete"))
						SpProjectManagerInstance->current()->game()->removeScene(sceneItem);
				}
				else
				{
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
					ImGui::Text("Delete");
					ImGui::PopStyleVar();
				}
				ImGui::EndPopup();
			}
		}

		void renderGameObjectsNode(SpScene* scene);
		void renderGameObjectsContextMenu(SpScene* scene);

		void renderCameraNode(SpScene* scene);

		void renderMeshesNode(SpScene* scene);

		void renderShadersNode(SpScene* scene);

		void renderLightingNode(SpScene* scene);
		void renderLightingContextMenu(SpScene* scene);
		void renderLightContextMenu(SpScene* scene, const sp_uint index);

	public:

		API_INTERFACE void init(SpWindow* window) override
		{
			SpFrame::init(window);
			resize(300, height() - 25);
			show();

			newSceneFrame.init(window);
		}

		API_INTERFACE void preRender() override
		{
		}

		API_INTERFACE void render() override
		{

		}

		API_INTERFACE void renderGUI() override;

		API_INTERFACE void postRender() override
		{
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "ProjectExplorerFrame";
		}

	};
}

#endif // PROJECT_EXPLORER_FRAME_HEADER