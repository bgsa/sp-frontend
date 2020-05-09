#ifndef PROPERTIES_FRAME_HEADER
#define PROPERTIES_FRAME_HEADER

#include "SpFrame.h"

namespace NAMESPACE_FRONTEND
{
	class PropertiesFrame
		: public SpFrame
	{
	public:

		API_INTERFACE void init(SpWindow* window) override
		{
			SpFrame::init(window);
			resize(300, 600);
			show();
		}

		API_INTERFACE void preRender() override
		{
		}

		API_INTERFACE void render(const RenderData& renderData) override
		{
			if (!isVisible())
				return;

			resize(width(), height() - 25);

			if (ImGui::Begin("Properties", NULL, ImGuiWindowFlags_NoCollapse))
			{
				ImGui::SetWindowSize(ImVec2((sp_float)width(), (sp_float)height()));
				ImGui::SetWindowPos(ImVec2((sp_float)(window()->state()->width - width()), 25.0f));

				ImGui::Columns(2, "Bar");

				for (sp_uint i = 0; i < 3; i++)
				{
					ImGui::Text("Label");
					ImGui::NextColumn();

					ImGui::Text("Value");
					ImGui::NextColumn();
				}

				ImGui::End();
			}
		}

		API_INTERFACE void postRender() override
		{
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "PropertiesFrame";
		}

	};
}

#endif // PROPERTIES_FRAME_HEADER