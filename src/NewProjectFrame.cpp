#include "NewProjectFrame.h"
#include "SpUIManager.h"

namespace NAMESPACE_FRONTEND
{

	void NewProjectFrame::renderGUI()
	{
		if (!isVisible())
			return;

		const ImVec2 windowSize = ImGui::GetIO().DisplaySize;

		ImGui::OpenPopup("New Project##popup");

		ImGui::BeginPopupModal("New Project##popup", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiCond_FirstUseEver);
		//loadState();

		ImGui::SetWindowSize(ImVec2((sp_float)width(), (sp_float)height()));
		horizontalAlign(HALF_FLOAT);

		ImVec2 frameSize = ImGui::GetItemRectSize();

		ImGui::Text("Name:");

		ImGui::PushItemWidth(width() * 0.94f);

		ImGui::InputText("##project-name", name, sizeof(name));

		//if (isFirstLoad()) ImGui::SetKeyboardFocusHere(0);

		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		if (ImGui::RadioButton("2D Game", game2DSelected))
		{
			game2DSelected = true;
			game3DSelected = false;
		}
		if (ImGui::RadioButton("3D Game", game3DSelected))
		{
			game2DSelected = false;
			game3DSelected = true;
		}

		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		ImGui::Text("Folder:");
		ImGui::Text("%s", folder);

		ImGui::PopItemWidth();

		if (ImGui::Button("Choose", ImVec2(100, 25)))
			ImGui::OpenPopup("Project Folder##popup");

		if (file_dialog.showFileDialog("Project Folder##popup", imgui_addons::ImGuiFileBrowser::DialogMode::SELECT, ImVec2(600, 300)))
			std::strcpy(folder, file_dialog.selected_path.c_str());

#ifdef WINDOWS
		//sp_char tempFolder[512];
		strReplace(folder, '/', SP_DIRECTORY_SEPARATOR, folder);
		//std::memcpy(folder,)
#endif

		ImGui::Indent(((sp_int)frameSize.x >> 1) + 20.0f);
		ImGui::Dummy(ImVec2(0.0f, 20.0f));

		if (ImGui::Button("OK", ImVec2(100, 25)))
		{
			if (SpProjectManagerInstance->current() != nullptr)
				SpProjectManagerInstance->unload();

			if (strIsNullOrEmpty(folder))
			{
				SpUINotificationManagerInstance->addNotification("Folder should not be empty!\0", NotificationType::VALIDATION);
				goto validation_exception;
			}

			if (!directoryExists(folder))
			{
				SpUINotificationManagerInstance->addNotification("Folder does not exists!\0", NotificationType::VALIDATION);
				goto validation_exception;
			}

			const sp_size folderLength = std::strlen(folder);
			const sp_size nameLength = std::strlen(name);

			sp_char projectFolder[512];
			directoryAddPath(folder, folderLength, name, nameLength, projectFolder);

			if (directoryExists(projectFolder))
			{
				SpUINotificationManagerInstance->addNotification("Project folder already exists!\0", NotificationType::VALIDATION);
				goto validation_exception;
			}
		
			sp_int gameType = 2;
			if (game3DSelected)
				gameType = 3;

			SpProjectManagerInstance->newProject(name, gameType, folder);

			SpScene* firstScene = SpProjectManagerInstance->current()->game()->scenes()->begin()->value();

			SpUIViewport* viewport = SpUIManagerInstance->addViewport();
			viewport->scene(firstScene);

			hide();
		}

	validation_exception:
		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(100, 25)))
		{
			ImGui::CloseCurrentPopup();
			hide();
		}

		ImGui::EndPopup();

		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
			hide();
	}

}