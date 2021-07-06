#include "SpPropertiesFrame.h"
#include "SpUIManager.h"

namespace NAMESPACE_FRONTEND
{

	void SpPropertiesFrame::render()
	{
		if (!isVisible())
			return;

		const SpWindowState* windowState = SpUIManagerInstance->window->state();
		const SpSize<sp_int> windowSize = windowState->availableRegion;
		const sp_float posY = 64.0f;
		const sp_float maxHeight = (sp_float)windowSize.height - posY - 30.0f;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2Zeros);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);
		begin("Properties", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

		const sp_float currentWidth = ImGui::GetWindowWidth();
		const sp_int posX = windowState->x + windowSize.width - (sp_int)currentWidth;

		resize(width(), (sp_int)windowSize.height);
		ImGui::SetWindowSize(ImVec2(currentWidth, maxHeight));
		ImGui::SetWindowPos(ImVec2((sp_float)posX, posY));

		// hide grip resize option
		if (ImGui::GetMouseCursor() == ImGuiMouseCursor_ResizeNWSE)
			ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);

		if (_selectedGameObject != SP_UINT_MAX && _scene != nullptr)
			renderProperties();

		end();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

}