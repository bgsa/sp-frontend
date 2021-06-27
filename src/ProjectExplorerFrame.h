#ifndef PROJECT_EXPLORER_FRAME_HEADER
#define PROJECT_EXPLORER_FRAME_HEADER

#include "SpFrame.h"
#include "SpProjectManager.h"
#include "SpScene.h"
#include "NewSceneFrame.h"

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

		void renderGameObjectsContextMenu()
		{
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::BeginMenu("Add", true))
				{
					ImGui::MenuItem("Camera", NULL, false, true);
					ImGui::MenuItem("Cube", NULL, false, true);
					ImGui::MenuItem("Plane", NULL, false, true);

					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}
		}

		void renderSceneNode(SpVectorItem<SpScene*>* sceneItem, const sp_bool canDeleteScene)
		{
			SpScene* scene = sceneItem->value();

			if (ImGui::TreeNode(scene->name()))
			{
				renderSceneContextMenu(sceneItem, canDeleteScene);

				if (ImGui::TreeNode("Game Objects"))
				{
					renderGameObjectsContextMenu();

					for (sp_size i = 0; i < scene->gameObjectsLength(); i++)
					{
						const SpGameObject* gameObject = scene->gameObject(i);

						if (ImGui::TreeNodeEx(gameObject->name(), ImGuiTreeNodeFlags_Leaf))
						{
							ImGui::TreePop();
						}
					}

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Scripts"))
				{
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
		}

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