#include "NewSceneFrame.h"
#include "SpProjectManager.h"
#include "SpUIManager.h"

namespace NAMESPACE_FRONTEND
{

	void NewSceneFrame::show() noexcept
	{
		std::memset(name, 0, sizeof(name));
		SpFrame::show();
		frameId = SpUIManagerInstance->frames.add(this);
	}

	void NewSceneFrame::hide() noexcept
	{
		SpFrame::hide();
		SpUIManagerInstance->frames.remove(frameId);
	}

	void NewSceneFrame::renderGUI()
	{
		if (!isVisible())
			return;

		const ImVec2 windowSize = ImGui::GetIO().DisplaySize;

		ImGui::OpenPopup("New Scene##popup");

		ImGui::BeginPopupModal("New Scene##popup", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiCond_FirstUseEver);

		ImGui::SetWindowSize(ImVec2((sp_float)width(), (sp_float)height()));
		horizontalAlign(HALF_FLOAT);

		ImVec2 frameSize = ImGui::GetItemRectSize();

		ImGui::Text("Name:");

		ImGui::PushItemWidth(width() * 0.94f);

		ImGui::InputText("##scene-name", name, sizeof(name));

		if (isFirstLoad())
			ImGui::SetKeyboardFocusHere(0);

		ImGui::PopItemWidth();

#define BUTTON_WIDTH 100.0f

		ImGui::Indent(frameSize.x - (2* BUTTON_WIDTH) - 30.0f);
		ImGui::Dummy(ImVec2(0.0f, 20.0f));

		if (ImGui::Button("OK", ImVec2(BUTTON_WIDTH, 25)))
		{
			SpGame* game = SpProjectManagerInstance->current()->game();

			SpStringId id(name);
			if (game->contaisScenes(id.id))
				SpUIManagerInstance->addNotification("Scene alread exists!", NotificationType::VALIDATION);
			else
			{
				game->addScenes(name);
				hide();
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(BUTTON_WIDTH, 25)))
		{
			ImGui::CloseCurrentPopup();
			hide();
		}

#undef BUTTON_WIDTH

		ImGui::EndPopup();

		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
			hide();
	}


}