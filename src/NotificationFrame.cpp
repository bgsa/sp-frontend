#include "NotificationFrame.h"
#include "SpUIManager.h"

#define NOTIFICATION_WIDTH  400.0f
#define NOTIFICATION_HEIGHT 60.0f

namespace NAMESPACE_FRONTEND
{

	void NotificationFrame::renderGUI()
	{
		if (!isVisible())
			return;

		sp_int windowWidth = window()->state()->width;
		const sp_int windowHeight = window()->state()->height;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

		ImGui::Begin(name, NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiCond_FirstUseEver);

		const sp_float shift = index * (NOTIFICATION_HEIGHT + 10.0f);

		ImGui::SetWindowSize(ImVec2(NOTIFICATION_WIDTH, NOTIFICATION_HEIGHT));
		ImGui::SetWindowPos(ImVec2((sp_float)windowWidth - NOTIFICATION_WIDTH - 10.0f, (sp_float)windowHeight - NOTIFICATION_HEIGHT - 40.0f - shift));

		ImVec2 uv1, uv2;
		switch (type)
		{
		case SpFrontEnd::OK:
			SpUIIconsInstance->getUVCoordinatesOk(uv1, uv2);
			break;
		case SpFrontEnd::WARNING:
			SpUIIconsInstance->getUVCoordinatesWarning(uv1, uv2);
			break;
		case SpFrontEnd::VALIDATION:
			SpUIIconsInstance->getUVCoordinatesValidation(uv1, uv2);
			break;
		case SpFrontEnd::ERROR_:
			SpUIIconsInstance->getUVCoordinatesError(uv1, uv2);
			break;
		}

		ImGui::Image(
			(void*)(intptr_t)SpUIIconsInstance->icons1->id(),
			ImVec2(32.0f, 32.0f), uv1, uv2
		);

		ImGui::SameLine();

		text.render(message);

		ImGui::End();

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		if (lifetime > 0)
			lifetime--;
		else
			SpUIManagerInstance->removeNotification(id);
	}

}

#undef NOTIFICATION_WIDTH
#undef NOTIFICATION_HEIGHT