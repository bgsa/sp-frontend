#include "NotificationFrame.h"
#include "SpUINotificationManager.h"
#include "SpUIManager.h"

namespace NAMESPACE_FRONTEND
{

	void NotificationFrame::render()
	{
		if (!isVisible())
			return;

		const sp_float windowWidth = (sp_float)SpUIManagerInstance->window->state()->width;
		const sp_float windowHeight = (sp_float)SpUIManagerInstance->window->state()->height;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

		ImGui::Begin(name, NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiCond_FirstUseEver);

		const sp_float shift = index * (height() + 10.0f);

		ImGui::SetWindowSize(ImVec2((sp_float)width(), (sp_float)height()));
		ImGui::SetWindowPos(ImVec2(windowWidth - (sp_float)width() - 10.0f, windowHeight - (sp_float)height() - 40.0f - shift));

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

		ImGui::BeginChild("temp", ImVec2(width() - 40.0f, height() - 10.0f), false, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		text.render(message);

		ImGui::PopStyleVar();
		ImGui::EndChild();

		ImGui::End();

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		if (lifetime > 0)
			lifetime--;
		else
			SpUINotificationManagerInstance->removeNotification(id);
	}

}