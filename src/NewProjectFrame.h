#ifndef NEW_PROJECT_FRAME_HEADER
#define NEW_PROJECT_FRAME_HEADER

#include "SpFrame.h"
#include "SpProjectManager.h"
#include "addons/FileBrowser/ImGuiFileBrowser.h"

namespace NAMESPACE_FRONTEND
{
	class NewProjectFrame
		: public SpFrame
	{
	private:
		sp_char name[100];
		sp_char folder[512];
		sp_bool game2DSelected, game3DSelected;
		imgui_addons::ImGuiFileBrowser file_dialog;

	public:

		API_INTERFACE void init(SpWindow* window) override
		{
			SpFrame::init(window);
			resize(500, 260);
			game2DSelected = false;
			game3DSelected = false;
		}

		API_INTERFACE inline void show() noexcept override
		{
			std::memset(name, 0, sizeof(name));
			std::memset(folder, 0, sizeof(folder));

			file_dialog.selected_fn.clear();
			file_dialog.selected_path.clear();

			SpFrame::show();
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
				
			if (isFirstLoad())
				ImGui::SetKeyboardFocusHere(0);

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
			ImGui::Text(folder);

			ImGui::PopItemWidth();

			if (ImGui::Button("Choose", ImVec2(100, 25)))
				ImGui::OpenPopup("Project Folder##popup");

			if (file_dialog.showFileDialog("Project Folder##popup", imgui_addons::ImGuiFileBrowser::DialogMode::SELECT, ImVec2(600, 300)))
				std::strcpy(folder, file_dialog.selected_path.c_str());

			ImGui::Indent(((sp_int)frameSize.x >> 1) + 20.0f);
			ImGui::Dummy(ImVec2(0.0f, 20.0f));

			if (ImGui::Button("OK", ImVec2(100, 25)))
			{
				if (SpProjectManagerInstance->current() != nullptr)
					SpProjectManagerInstance->unload();

				sp_int gameType = 2;
				if (game3DSelected)
					gameType = 3;

				SpProjectManagerInstance->newProject(name, gameType);
				SpProjectManagerInstance->current()->folder(folder);

				hide();
			}

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

		API_INTERFACE inline const sp_char* toString() override
		{
			return "AboutFrame";
		}

	};
}

#endif // NEW_PROJECT_FRAME_HEADER