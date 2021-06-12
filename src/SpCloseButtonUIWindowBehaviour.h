#ifndef SP_CLOSE_BUTTON_UI_WINDOW_BEHAVIOUR_HEADER
#define SP_CLOSE_BUTTON_UI_WINDOW_BEHAVIOUR_HEADER

#include "SpectrumFronend.h"
#include "imgui/imgui.h"

namespace NAMESPACE_FRONTEND
{
	
	class SpCloseButtonUIWindowBehaviour
	{
	private:
		sp_bool mouseDownButtonClose;

	public:
		void (*onClick)(SpCloseButtonUIWindowBehaviour*, void* parameter);
		void* onClickParameter;

		API_INTERFACE inline SpCloseButtonUIWindowBehaviour()
		{
			mouseDownButtonClose = false;
			onClickParameter = nullptr;
		}

		API_INTERFACE inline void render()
		{
			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 windowSize = ImGui::GetWindowSize();

			const ImVec2 closeButtonCenter = ImVec2(windowPos.x + windowSize.x - 15.0f, windowPos.y + 12.0f);
			const sp_float closeButtonRay = 10.0f;
			sp_int alphaColorButton = 170;

			ImGuiIO io = ImGui::GetIO();
			ImVec2 cursorPos = io.MousePos;
			cursorPos.x -= windowPos.x;
			cursorPos.y -= windowPos.y;

			if (cursorPos.x > 0.0f && cursorPos.y > 0.0f)
			{
				const sp_float diffX = cursorPos.x - (closeButtonCenter.x - windowPos.x);
				const sp_float diffY = cursorPos.y - (closeButtonCenter.y - windowPos.y);

				const sp_float distance = sp_sqrt(diffX * diffX + diffY * diffY);
				if (distance < closeButtonRay) // mouse hover close button?
				{
					alphaColorButton = 255;

					if (mouseDownButtonClose && !io.MouseDown[0] && onClick != nullptr)
					{
						onClick(this, onClickParameter);
						mouseDownButtonClose = false;
					}

					if (io.MouseClicked[0])
						mouseDownButtonClose = true;
				}
			}

			const sp_float lineSize = 4.0f;

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->PushClipRectFullScreen();

			drawList->AddCircleFilled(closeButtonCenter, closeButtonRay, IM_COL32(255, 0, 0, alphaColorButton));

			drawList->AddLine(
				ImVec2(closeButtonCenter.x - lineSize, closeButtonCenter.y + lineSize),
				ImVec2(closeButtonCenter.x + lineSize, closeButtonCenter.y - lineSize),
				IM_COL32(0, 0, 0, alphaColorButton),
				2.0f);

			drawList->AddLine(
				ImVec2(closeButtonCenter.x - lineSize, closeButtonCenter.y - lineSize),
				ImVec2(closeButtonCenter.x + lineSize, closeButtonCenter.y + lineSize),
				IM_COL32(0, 0, 0, alphaColorButton),
				2.0f);

			drawList->PopClipRect();
		}
	};
}

#endif // SP_CLOSE_BUTTON_UI_WINDOW_BEHAVIOUR_HEADER