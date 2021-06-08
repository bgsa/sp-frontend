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
			const sp_bool scenesOpened = ImGui::TreeNode("Scenes");

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("New Scene"))
				{
					newSceneFrame.show();
				}
				ImGui::EndPopup();
			}

			if (scenesOpened)
			{
				SpVector<SpScene*>* scenes = project->game()->scenes();
				const sp_uint canDeleteScene = scenes->length() > ONE_UINT;

				for (SpVectorItem<SpScene*>* item = scenes->begin(); item != nullptr; item = item->next())
				{
					SpScene* scene = item->value();
					sp_char* sceneName = scene->name();
					const sp_bool sceneOpened = ImGui::TreeNode(sceneName);

					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Add GameObject"))
						{

						}

						if (canDeleteScene)
						{
							if (ImGui::MenuItem("Delete"))
							{
								project->game()->removeScene(item);
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

					if (sceneOpened)
						ImGui::TreePop();
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