#ifndef SP_MEMORY_MAP_VIEWER_FRAME_HEADER
#define SP_MEMORY_MAP_VIEWER_FRAME_HEADER

#include "SpIFrameComponent.h"
#include "SpImGui.h"
#include "SpCloseButtonUIWindowBehaviour.h"

namespace NAMESPACE_FRONTEND
{
	class SpMemoryMapViewerFrame
		: public SpIFrameComponent
	{
	private:
		SpCloseButtonUIWindowBehaviour closeButton;

		void renderMenuBar();
		void renderMap();

	public:

		API_INTERFACE void init();

		API_INTERFACE void render() override
		{
			if (!isVisible())
				return;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 3.0f));

			if (ImGui::Begin("Memory Map Viewer", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar))
			{
				ensureMinSize();

				closeButton.render();

				renderMenuBar();
				renderMap();
				
				ImGui::End();
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
		}

	};

}

#endif // SP_MEMORY_MAP_VIEWER_FRAME_HEADER