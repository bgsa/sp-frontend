#ifndef PROJECT_EXPLORER_FRAME_HEADER
#define PROJECT_EXPLORER_FRAME_HEADER

#include "SpFrame.h"
#include "SpProjectManager.h"
#include "SpScene.h"
#include "NewSceneFrame.h"
#include "SpShader.h"
#include "SpGame.h"

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

		void renderGameObjectsContextMenu(SpScene* scene);

		void renderSceneNode(SpVectorItem<SpScene*>* sceneItem, const sp_bool canDeleteScene)
		{
			SpScene* scene = sceneItem->value();

			if (ImGui::TreeNode(scene->name()))
			{
				renderSceneContextMenu(sceneItem, canDeleteScene);

				if (ImGui::TreeNode("Game Objects"))
				{
					renderGameObjectsContextMenu(scene);

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

				if (ImGui::TreeNode("Meshes"))
				{
					for (sp_size i = 0; i < scene->meshManager()->length(); i++)
					{
						sp_char* name = scene->meshManager()->name(i);

						if (ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_Leaf))
						{
							ImGui::TreePop();
						}
					}

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Shaders"))
				{
					for (SpVectorItem<SpShader*>* item = scene->shaders.begin(); item != nullptr; item = item->next())
					{
						const sp_char* name = item->value()->name();

						if (ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_Leaf))
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