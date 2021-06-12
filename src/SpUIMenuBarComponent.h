#ifndef SP_UI_MENU_BAR_COMPONENT_HEADER
#define SP_UI_MENU_BAR_COMPONENT_HEADER

#include "SpectrumFronend.h"
#include "imgui/imgui.h"
#include "SpProjectManager.h"
#include "ProjectExplorerFrame.h"
#include "NewProjectFrame.h"
#include "AboutFrame.h"
#include "LogFrame.h"
#include "PropertiesFrame.h"
#include "addons/FileBrowser/ImGuiFileBrowser.h"
#include "SpMemoryMapViewerFrame.h"

namespace NAMESPACE_FRONTEND
{
	class SpUIMenuBarComponent
	{
	private:
		ProjectExplorerFrame projectExplorerFrame;
		NewProjectFrame newProjectFrame;
		AboutFrame aboutFrame;
		LogFrame logFrame;
		PropertiesFrame propertiesFrame;
		SpMemoryMapViewerFrame memoryMapViewerFrame;

		imgui_addons::ImGuiFileBrowser loadProjectDialog;

		SpWindow* window;

	public:

		API_INTERFACE inline SpUIMenuBarComponent()
		{
			
		}

		API_INTERFACE void init(SpWindow* window);

		API_INTERFACE void render();

	};
}

#endif // SP_UI_MENU_BAR_COMPONENT_HEADER