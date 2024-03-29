#include "SpUIMenuBarComponent.h"
#include "SpUIManager.h"

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

		loadProjectDialog.onClose = &loadProjectDialog_OnClose;

		projectExplorerFrame.init(window);
		logFrame.init(window);

		newProjectFrame.init(window);
		aboutFrame.init(window);

		memoryMapViewerFrame.init();
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

				if (ImGui::MenuItem("Load", NULL, false))
					showLoadProject = true;

				if (ImGui::BeginMenu("Recents"))
				{
					const sp_int latestLength = SpGameEngineSettingsInstance->lastestProjectsLength();
					sp_char* latest = SpGameEngineSettingsInstance->lastestProjects();

					for (sp_int i = 0; i < latestLength; i++)
					{
						const sp_char* filename = &latest[i * SP_DIRECTORY_MAX_LENGTH];
						const sp_size filenameLength = std::strlen(filename);

						sp_char index[3];
						sp_size indexLength;
						convert(i + 1, index, indexLength);

						sp_char name[512];
						std::memcpy(name, index, indexLength);
						std::memcpy(&name[indexLength], ".  ", 3);
						std::memcpy(&name[indexLength + 3], filename, filenameLength);
						const sp_size nameLength = indexLength + filenameLength + 3;
						name[nameLength] = END_OF_STRING;

						sp_bool enabled = true;
						if (!fileExists(filename))
						{
							enabled = false;
							std::memcpy(&name[nameLength], " (not found)", 12);
							name[nameLength + 12] = END_OF_STRING;
						}

						if (ImGui::MenuItem(name, NULL, false, enabled))
							SpProjectManagerInstance->load(filename);
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Quit", NULL))
					window->close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::MenuItem("Camera", NULL);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				if (ImGui::MenuItem("Memory Map", NULL))
					memoryMapViewerFrame.show();
				
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Project Explorer", NULL, projectExplorerFrame.isVisible()))
					if (projectExplorerFrame.isVisible())
						projectExplorerFrame.hide();
					else
						projectExplorerFrame.show();

				if (ImGui::MenuItem("Properties", NULL, SpUIManagerInstance->propertiesFrame.isVisible()))
					if (SpUIManagerInstance->propertiesFrame.isVisible())
						SpUIManagerInstance->propertiesFrame.hide();
					else
						SpUIManagerInstance->propertiesFrame.show();

				if (ImGui::MenuItem("Log", NULL, logFrame.isVisible()))
					if (logFrame.isVisible())
						logFrame.hide();
					else
						logFrame.show();


				
				if (ImGui::BeginMenu("Viewports", true))
				{
					if (ImGui::MenuItem("New Viewport", NULL, false))
						SpUIManagerInstance->addViewport();

					for (SpVectorItem<SpUIViewport*>* item = SpUIManagerInstance->viewports.begin(); item != nullptr; item = item->next())
					{
						SpUIViewport* viewport = item->value();

						sp_char title[SP_SCENE_NAME_MAX_LENGTH + 30];
						viewport->getTitle(title, SP_SCENE_NAME_MAX_LENGTH + 30);

						if (ImGui::MenuItem(title, NULL, viewport->isVisible(), true))
						{
							if (viewport->isVisible())
								viewport->hide();
							else
								viewport->show();
						}
					}

					ImGui::EndMenu();
				}

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

		newProjectFrame.renderGUI();
		aboutFrame.renderGUI();

		memoryMapViewerFrame.render();

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