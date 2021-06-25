#include "ProjectExplorerFrame.h"
#include "SpUIManager.h"

namespace NAMESPACE_FRONTEND
{

	void ProjectExplorerFrame::renderGUI()
	{
		if (!isVisible())
			return;

		const sp_float windowHeight = (sp_float)SpUIManagerInstance->window->state()->height;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 3.0f));
		ImGui::Begin("Project Explorer", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

		loadState();

		ImGui::SetWindowSize(ImVec2(ImGui::GetItemRectSize().x, windowHeight - 64.0f));
		ImGui::SetWindowPos(ImVec2(0.0f, 64.0f));

		SpProject* project = SpProjectManagerInstance->current();
		if (project != nullptr)
		{
			if (ImGui::TreeNode("Scenes"))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("New Scene"))
					{
						newSceneFrame.show();
					}
					ImGui::EndPopup();
				}

				SpVector<SpScene*>* scenes = project->game()->scenes();
				const sp_bool canDeleteScene = scenes->length() > ONE_UINT;

				for (SpVectorItem<SpScene*>* sceneItem = scenes->begin(); sceneItem != nullptr; sceneItem = sceneItem->next())
				{
					SpScene* scene = sceneItem->value();

					if (ImGui::TreeNode(scene->name()))
					{
						if (ImGui::BeginPopupContextItem())
						{
							if (ImGui::MenuItem("Add GameObject"))
							{

							}

							if (canDeleteScene)
							{
								if (ImGui::MenuItem("Delete"))
								{
									project->game()->removeScene(sceneItem);
								}
							}
							else
							{
								ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
								ImGui::Text("Delete");
								ImGui::PopStyleVar();
							}
							ImGui::EndPopup();
						}

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
				}

				ImGui::TreePop(); // close Scenes Node
			}

			if (ImGui::TreeNode("Resources"))
			{
				ImGui::TreePop();
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}


}