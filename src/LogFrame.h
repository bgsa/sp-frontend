#ifndef LOG_FRAME_HEADER
#define LOG_FRAME_HEADER

#include "SpFrame.h"
#include "SpLogProvider.h"
#include "SpLogEngineProvider.h"

namespace NAMESPACE_FRONTEND
{
	class LogFrame
		: public SpFrame
	{
	public:

		API_INTERFACE void init(SpWindow* window) override
		{
			SpFrame::init(window);
			resize(500, 200);
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

			if (ImGui::Begin("Log Window", NULL, ImGuiWindowFlags_NoCollapse))
			{
				loadState();

				ImGui::SetWindowSize(ImVec2((sp_float)width(), (sp_float)height()));

				SpLogEngineProvider* provider = dynamic_cast<SpLogEngineProvider*>(SpLogger::instance()->providers()->begin()->value());

				for (sp_uint i = 0; i < provider->messageLength(); i++)
					ImGui::Text(provider->message(i));

				if (ImGui::BeginPopupContextWindow())
				{
					if (ImGui::MenuItem("Clear"))
						provider->clear();

					ImGui::EndPopup();
				}

				ImGui::End();
			}
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "LogFrame";
		}

	};
}

#endif