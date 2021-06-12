#include "SpMemoryMapViewerFrame.h"

namespace NAMESPACE_FRONTEND
{
#define TITLE_BAR_HEIGHT 20
#define ROW_HEIGHT 30
#define ROW_SPACING 3

	void closeButtonClick(SpCloseButtonUIWindowBehaviour* e, void* parameter)
	{
		SpMemoryMapViewerFrame* window = (SpMemoryMapViewerFrame*)parameter;
		window->hide();
	}

	void SpMemoryMapViewerFrame::init()
	{
		_minWidth = 800;
		_minHeight = 600;
		resize(_minWidth, _minHeight);

		closeButton.onClick = closeButtonClick;
		closeButton.onClickParameter = this;
	}

	void SpMemoryMapViewerFrame::renderMenuBar()
	{
		ImGui::BeginMenuBar();
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Close"))
			{
				//Do something
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	void SpMemoryMapViewerFrame::renderMap()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		const sp_float currentScrollPosition = ImGui::GetScrollY();
		//sp_float maxScrollPosition = ImGui::GetScrollMaxY();

		const sp_size firstAddress = SpPoolMemoryAllocator::main()->firstAddress();
		const sp_size lastAddress = SpPoolMemoryAllocator::main()->currentAddress();
		const sp_size totalMemory = (lastAddress - firstAddress) / SIZEOF_WORD;

		const sp_float addressWidth = 180.0f;
		const sp_float verticalScrollBarWidth = 8.0f;
		const sp_float menuBarHeight = 30.0f;
		const sp_float bottomHeight = 30.0f;

		const ImVec2 windowPos = ImGui::GetWindowPos();
		const ImVec2 windowSize = ImGui::GetWindowSize();
		const ImVec2 viewerPos = ImVec2(windowPos.x + addressWidth, windowPos.y + TITLE_BAR_HEIGHT + menuBarHeight);
		const sp_float viewerWidth = viewerPos.x + windowSize.x - verticalScrollBarWidth;
		const sp_uint barWidth = (sp_uint)(windowSize.x - addressWidth - verticalScrollBarWidth);

		sp_size rowCount = totalMemory / barWidth;

		if (totalMemory % barWidth != 0.0f)
			rowCount++;

		const sp_float viewerHeight = rowCount * (ROW_HEIGHT + ROW_SPACING) - TITLE_BAR_HEIGHT - currentScrollPosition;

		ImGui::SetNextWindowPos(viewerPos);
		ImGui::BeginChild("MemViewerContainer", ImVec2(viewerWidth, viewerHeight), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

		for (sp_size rowIndex = 0; rowIndex < rowCount; rowIndex++)
		{
			const sp_float minRowX = viewerPos.x;
			const sp_float maxRowX = minRowX + barWidth;

			const sp_float minRowY = viewerPos.y + (rowIndex * (ROW_HEIGHT + ROW_SPACING)) - currentScrollPosition;
			const sp_float maxRowY = minRowY + ROW_HEIGHT;

			drawList->AddRectFilled(ImVec2(minRowX, minRowY), ImVec2(maxRowX, maxRowY), IM_COL32(0, 255, 0, 255), 0.0f, 0);

			sp_char address[35];
			sp_uint addressLength;
			strToMemoryAddress(firstAddress + (rowIndex * barWidth), address, addressLength);

			drawList->AddText(ImVec2(minRowX - addressWidth + 10.0f, minRowY + 5.0f), ImGui_ColorWhite, address);
		}

		ImGui::EndChild();
	}

#undef ROW_SPACING
#undef ROW_HEIGHT
#undef TITLE_BAR_HEIGHT
}