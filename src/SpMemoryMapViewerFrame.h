#ifndef SP_MEMORY_MAP_VIEWER_FRAME_HEADER
#define SP_MEMORY_MAP_VIEWER_FRAME_HEADER

#include "SpIFrameComponent.h"
#include "SpImGui.h"
#include "SpCloseButtonUIWindowBehaviour.h"
#include "SpMemoryProfiling.h"
#include "SpAlertFrame.h"
#include "SpStatusBarFrame.h"

namespace NAMESPACE_FRONTEND
{
	class SpMemoryMapViewerFrame
		: public SpIFrameComponent
	{
	private:
		SpCloseButtonUIWindowBehaviour closeButton;
		SpAlertFrame alert;
		SpStatusBarFrame statusBar;

		void renderMenuBar();
		void renderMap();
		void renderTooltip(SpMemoryProfilingDescriptor* descriptor);

		void renderStatusBar()
		{
			statusBar.beginChild();

			sp_char text[50];
			sp_size textLength;

			sp_size value = SpPoolMemoryAllocator::main()->memorySize() / 1024 / 1024;
			convert(value, text, textLength);

			ImGui::Text("Memory (Total): ");
			ImGui::SameLine();
			ImGui::Text(text);
			ImGui::SameLine();
			ImGui::Text("MB");

			value = SpPoolMemoryAllocator::main()->usedMemorySize() / 1024 / 1024;
			convert(value, text, textLength);

			ImGui::SameLine();
			ImGui::Text("Memory (Used): ");
			ImGui::SameLine();
			ImGui::Text(text);
			ImGui::SameLine();
			ImGui::Text("MB");

			value = SpPoolMemoryAllocator::main()->availableMemorySize() / 1024 / 1024;
			convert(value, text, textLength);

			ImGui::SameLine();
			ImGui::Text("Memory (Available): ");
			ImGui::SameLine();
			ImGui::Text(text);
			ImGui::SameLine();
			ImGui::Text("MB");

			statusBar.endChild();
		}

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

				renderStatusBar();

				ImGui::End();
			}

			alert.render();

			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
		}

	};

}

#endif // SP_MEMORY_MAP_VIEWER_FRAME_HEADER