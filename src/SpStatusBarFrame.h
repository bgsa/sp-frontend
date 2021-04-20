#ifndef SP_STATUS_BAR_FRAME_HEADER
#define SP_STATUS_BAR_FRAME_HEADER

#include "SpFrame.h"

namespace NAMESPACE_FRONTEND
{
	class SpStatusBarFrame
		: public SpFrame
	{
	private:
		Timer localTimer;

	public:

		API_INTERFACE void init(SpWindow* window) override
		{
			SpFrame::init(window);
			resize(window->state()->width, 30);
			show();
			localTimer.start();
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

			ImGui::Begin("StatusBar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiCond_FirstUseEver);
				loadState();

				ImGui::SetWindowSize(ImVec2(windowSize.x, 30));
				ImGui::SetWindowPos(ImVec2(0, windowSize.y - 30));

			ImVec2 itemSize = ImGui::GetItemRectSize();

			sp_char value[30];
			std::memcpy(value, "FPS:  ", SIZEOF_CHAR * 5);
			SpString::convert(localTimer.elapsedTime(), &value[5]);

			ImGui::Text(value);

			ImGui::End();

			localTimer.update();
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "AboutFrame";
		}

	};
}

#endif // SP_STATUS_BAR_FRAME_HEADER