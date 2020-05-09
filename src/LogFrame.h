#ifndef LOG_FRAME_HEADER
#define LOG_FRAME_HEADER

#include "SpFrame.h"
#include <vector>

namespace NAMESPACE_FRONTEND
{
	class LogFrame
		: public SpFrame
	{
	private:
		std::vector<sp_char*> messages;

	public:

		API_INTERFACE void init(SpWindow* window) override
		{
			SpFrame::init(window);
			resize(500, 200);
			show();

			char text1[7] = "teste1";
			char text2[7] = "teste2";

			messages.push_back(text1);
			messages.push_back(text2);
		}

		API_INTERFACE void preRender() override
		{
		}

		API_INTERFACE void render(const RenderData& renderData) override
		{
			if (!isVisible())
				return;

			if (ImGui::Begin("Log Window", NULL, ImGuiWindowFlags_NoCollapse))
			{
				ImGui::SetWindowSize(ImVec2((sp_float)width(), (sp_float)height()));

				for (sp_uint i = 0; i < messages.size(); i++)
					ImGui::Text(messages[i]);

				if (ImGui::BeginPopupContextWindow())
				{
					if (ImGui::MenuItem("Clear"))
						messages.clear();

					ImGui::EndPopup();
				}

				ImGui::End();
			}
		}

		API_INTERFACE void postRender() override
		{
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "LogFrame";
		}

	};
}

#endif