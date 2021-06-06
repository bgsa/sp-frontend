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

		API_INTERFACE void renderGUI() override
		{
			if (!isVisible())
				return;

			resize(window()->state()->width, height() - 25);

			ImGui::Begin("Project Explorer", NULL, ImGuiWindowFlags_NoCollapse);
			loadState();

			//ImGui::SetWindowSize(ImVec2(ImGui::GetItemRectSize().x, (sp_float) getHeight()));
			//ImGui::SetWindowPos( ImVec2( 0.0f , 25.0f ));

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
		}

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