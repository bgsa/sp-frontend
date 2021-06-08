#include "SpUIMenuBarComponent.h"

namespace NAMESPACE_FRONTEND
{
	sp_bool showLoadProject;

	void loadProjectDialog_OnClose()
	{
		showLoadProject = false;
	}

	void SpUIMenuBarComponent::init(SpWindow* window)
	{
		this->window = window;

		propertiesFrame.selectedObject(1u);
		loadProjectDialog.onClose = &loadProjectDialog_OnClose;

		projectExplorerFrame.init(window);
		logFrame.init(window);
		propertiesFrame.init(window);

		newProjectFrame.init(window);
		aboutFrame.init(window);
	}

	void SpUIMenuBarComponent::render()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Project", NULL))
					newProjectFrame.show();

				if (ImGui::MenuItem("Save", "CTRL+S", false, SpProjectManagerInstance->isLoaded()))
					SpProjectManagerInstance->save();

				if (ImGui::MenuItem("Load", "", false))
					showLoadProject = true;

				if (ImGui::MenuItem("Quit", NULL))
					window->close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::MenuItem("Camera", NULL);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Project Explorer", NULL, projectExplorerFrame.isVisible()))
					if (projectExplorerFrame.isVisible())
						projectExplorerFrame.hide();
					else
						projectExplorerFrame.show();

				if (ImGui::MenuItem("Properties", NULL, propertiesFrame.isVisible()))
					if (propertiesFrame.isVisible())
						propertiesFrame.hide();
					else
						propertiesFrame.show();

				if (ImGui::MenuItem("Log", NULL, logFrame.isVisible()))
					if (logFrame.isVisible())
						logFrame.hide();
					else
						logFrame.show();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings"))
			{
				if (ImGui::BeginMenu("Theme"))
				{
					if (ImGui::MenuItem("Dark (Default)"))
						ImGui::StyleColorsDark();

					if (ImGui::MenuItem("Light"))
						ImGui::StyleColorsLight();

					if (ImGui::MenuItem("Classic"))
						ImGui::StyleColorsClassic();

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("About Spectrum", NULL, aboutFrame.isVisible()))
					if (aboutFrame.isVisible())
						aboutFrame.hide();
					else
						aboutFrame.show();

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		projectExplorerFrame.renderGUI();
		logFrame.renderGUI();
		propertiesFrame.renderGUI();

		newProjectFrame.renderGUI();
		aboutFrame.renderGUI();

		if (showLoadProject)
		{
			ImGui::OpenPopup("Load Project##popup");
			if (loadProjectDialog.showFileDialog("Load Project##popup", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(600, 300), SP_FILENAME_PROJECT_SUFFIX))
			{
				sp_char filename[512];
				strReplace(loadProjectDialog.selected_path.c_str(), '/', SP_DIRECTORY_SEPARATOR, filename);

				SpProjectManagerInstance->load(filename);
				showLoadProject = false;
			}
		}
	}

}