#ifndef PROJECT_EXPLORER_FRAME_HEADER
#define PROJECT_EXPLORER_FRAME_HEADER

#include "SpFrame.h"
#include <algorithm>

namespace NAMESPACE_FRONTEND
{
	class ProjectExplorerFrame
		: public SpFrame
	{
	public:

		API_INTERFACE void init(SpWindow* window) override
		{
			SpFrame::init(window);
			resize(300, height() - 25);
			show();
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

			ImGui::SetNextTreeNodeOpen(true);
			if (ImGui::TreeNode("project-1"))
			{
				ImGui::SetNextTreeNodeOpen(true);
				if (ImGui::TreeNode("scenes"))
				{
					ImGui::TreePop();
				}

				ImGui::SetNextTreeNodeOpen(true);
				if (ImGui::TreeNode("resources"))
				{
					ImGui::TreePop();
				}

				ImGui::TreePop();
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