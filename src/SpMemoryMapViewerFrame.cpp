#include "SpMemoryMapViewerFrame.h"
#include <algorithm>

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
		resize((sp_int)_minWidth, (sp_int)_minHeight);

		closeButton.onClick = closeButtonClick;
		closeButton.onClickParameter = this;

		alert.init();
		statusBar.init();
	}

	void SpMemoryMapViewerFrame::renderMenuBar()
	{
		ImGui::BeginMenuBar();
		if (ImGui::BeginMenu("Memory"))
		{
			if (ImGui::MenuItem("Check Invalid Allocations"))
			{
				sp_bool hasInvalidAllocation = false;

				for (std::pair<sp_size, SpMemoryProfilingDescriptor*> allocation : SpMemoryProfilingInstance->allocations)
				{
					const sp_size begin1 = allocation.second->address;
					const sp_size end1 = begin1 + allocation.second->size * SIZEOF_WORD;

					for (std::pair<sp_size, SpMemoryProfilingDescriptor*> allocation2 : SpMemoryProfilingInstance->allocations)
					{
						const sp_size begin2 = allocation2.second->address;
						const sp_size end2 = begin2 + allocation2.second->size * SIZEOF_WORD;

						// check the memories allocation 1 and 2 overlaps
						if ((begin2 > begin1 && begin2 < end1) ||
							(end2 > begin1 && end2 < end1))
						{
							hasInvalidAllocation = true;
							break;
						}
					}
				}

				if (hasInvalidAllocation)
					alert.setMessage("Invalid allocation found!");
				else
					alert.setMessage("No invalid allocation found!");

				alert.show();
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	void SpMemoryMapViewerFrame::renderTooltip(SpMemoryProfilingDescriptor* descriptor)
	{
		ImGui::BeginTooltip();

		sp_char text[35];
		sp_size textLength;
		sp_uint temp;
		strToMemoryAddress(descriptor->address, text, temp);
		ImGui::Text("Address: "); ImGui::SameLine(); ImGui::Text(text);

		convert(descriptor->size * SIZEOF_WORD, text, textLength);
		ImGui::Text("Size: "); ImGui::SameLine(); ImGui::Text(text); ImGui::SameLine(); ImGui::Text("bytes");

		convert(descriptor->line, text, temp);
		ImGui::Text("Location: "); 
		ImGui::SameLine(); 
		ImGui::Text(descriptor->filename);
		ImGui::SameLine();
		ImGui::Text("::");
		ImGui::SameLine();
		ImGui::Text(descriptor->functionName);
		ImGui::SameLine();
		ImGui::Text("::");
		ImGui::SameLine();
		ImGui::Text(text);

		ImGui::EndTooltip();
	}

	void SpMemoryMapViewerFrame::renderMap()
	{
		ImGuiIO& io = ImGui::GetIO();
		const ImVec2 mousePos = io.MousePos;
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		const sp_float currentScrollPosition = ImGui::GetScrollY();
		const sp_bool _isFocused = isFocused();

		const sp_size firstAddress = SpPoolMemoryAllocator::main()->firstAddress();
		const sp_size lastAddress = SpPoolMemoryAllocator::main()->currentAddress();
		const sp_size totalMemory = (lastAddress - firstAddress);

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
		ImGui::BeginChild("MemViewerContainer", ImVec2(viewerWidth, viewerHeight + 10.0f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

		for (sp_size rowIndex = 0; rowIndex < rowCount; rowIndex++)
		{
			const sp_float minRowX = viewerPos.x;
			const sp_float maxRowX = minRowX + barWidth;

			const sp_float minRowY = viewerPos.y + (rowIndex * (ROW_HEIGHT + ROW_SPACING)) - currentScrollPosition;
			const sp_float maxRowY = minRowY + ROW_HEIGHT;

			drawList->AddRectFilled(ImVec2(minRowX, minRowY), ImVec2(maxRowX, maxRowY), IM_COL32(128, 128, 128, 255), 0.0f, 0);

			sp_char address[35];
			sp_uint addressLength;
			strToMemoryAddress(firstAddress + (rowIndex * barWidth), address, addressLength);

			drawList->AddText(ImVec2(minRowX - addressWidth + 10.0f, minRowY + 5.0f), ImGui_ColorWhite, address);
		}

		for (std::pair<sp_size, SpMemoryProfilingDescriptor*> allocation : SpMemoryProfilingInstance->allocations)
		{
			const sp_size address = (allocation.first - firstAddress) - SIZEOF_WORD; // SIZEOF_WORD = size of PoolMemory PageHeader
			SpMemoryProfilingDescriptor* memoryDescriptor = allocation.second;
			sp_size addressLength = (memoryDescriptor->size * SIZEOF_WORD) + SIZEOF_WORD; // SIZEOF_WORD = size of PoolMemory PageHeader

			sp_size rowIndex = address / barWidth;
			const sp_size columnIndex = address % barWidth;

			sp_float minRowX = (sp_float)columnIndex;
			sp_float maxRowX = sp_min(minRowX + (sp_float)addressLength, (sp_float)barWidth);
			sp_float minRowY = (rowIndex * (ROW_HEIGHT + ROW_SPACING)) - currentScrollPosition;
			sp_float maxRowY = minRowY + ROW_HEIGHT;

			ImVec2 beginMemoryPixel = ImVec2(viewerPos.x + minRowX, viewerPos.y + minRowY);
			ImVec2 endMemoryPixel = ImVec2(viewerPos.x + maxRowX, viewerPos.y + maxRowY);

			drawList->AddRectFilled(beginMemoryPixel, endMemoryPixel, IM_COL32(0, 180, 0, 255), 0.0f, 0);
			
			// draw the beginning of memory allocation
			drawList->AddLine(beginMemoryPixel, ImVec2(beginMemoryPixel.x, viewerPos.y + maxRowY), ImGui_ColorBlack, 1.0f);

			// check the tooltip should be rendered
			if (_isFocused && 
				mousePos.x >= beginMemoryPixel.x && mousePos.x <= endMemoryPixel.x &&
				mousePos.y >= beginMemoryPixel.y && mousePos.y <= endMemoryPixel.y)
				renderTooltip(memoryDescriptor);

			addressLength = addressLength - (sp_size)(maxRowX - minRowX);
			while (addressLength != 0)
			{
				rowIndex++;

				minRowX = ZERO_FLOAT;
				maxRowX = sp_min((sp_float)addressLength, (sp_float)barWidth);

				minRowY = (rowIndex * (ROW_HEIGHT + ROW_SPACING)) - currentScrollPosition;
				maxRowY = minRowY + ROW_HEIGHT;

				beginMemoryPixel = ImVec2(viewerPos.x + minRowX, viewerPos.y + minRowY);
				endMemoryPixel = ImVec2(viewerPos.x + maxRowX, viewerPos.y + maxRowY);

				drawList->AddRectFilled(beginMemoryPixel, endMemoryPixel, IM_COL32(0, 180, 0, 255), 0.0f, 0);
				addressLength = addressLength - (sp_size)maxRowX;

				// check the tooltip should be rendered
				if (_isFocused && 
					mousePos.x >= beginMemoryPixel.x && mousePos.x <= endMemoryPixel.x &&
					mousePos.y >= beginMemoryPixel.y && mousePos.y <= endMemoryPixel.y)
					renderTooltip(memoryDescriptor);
			}
		}

		ImGui::EndChild();
	}

#undef ROW_SPACING
#undef ROW_HEIGHT
#undef TITLE_BAR_HEIGHT
}