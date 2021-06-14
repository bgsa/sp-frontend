#ifndef SP_ALERT_FRAME_HEADER
#define SP_ALERT_FRAME_HEADER

#include "SpectrumFronend.h"
#include "SpIFrameComponent.h" 
#include "SpUIIcons.h"

namespace NAMESPACE_FRONTEND
{
	class SpAlertFrame : 
		public SpIFrameComponent
	{
	private:
		sp_char _message[500];
		ImVec2 iconUV1, iconUV2;

	public:
		
		API_INTERFACE inline SpAlertFrame()
		{
			resize(500, 300);
		}

		API_INTERFACE inline void init()
		{
			SpUIIconsInstance->getUVCoordinatesInformation(iconUV1, iconUV2);
		}

		API_INTERFACE inline void setMessage(const sp_char* message)
		{
			std::memcpy(_message, message, 498);
			_message[499] = END_OF_STRING;
		}

		API_INTERFACE inline void render() override
		{
			if (!isVisible())
				return;

			ImGui::OpenPopup("Alert##popup");

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));

			if (ImGui::BeginPopupModal("Alert##popup", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
			{
				ImGui::Image(
					(void*)(intptr_t)SpUIIconsInstance->icons1->id(),
					ImVec2(32.0f, 32.0f), iconUV1, iconUV2
				);
				ImGui::SameLine();
				ImGui::Text(_message);

				ImVec2 buttonSize(100.0f, 25.0f);
				ImGuiStyle style = ImGui::GetStyle();
				
				ImGui::Dummy(ImVec2(0.0f, 20.0f));
				ImGui::Indent(width() - buttonSize.x - style.WindowPadding.x);

				if (ImGui::Button("OK", buttonSize))
					hide();

				ImGui::EndPopup();
			}

			ImGui::PopStyleVar();
		}

	};
}

#endif // SP_ALERT_FRAME_HEADER