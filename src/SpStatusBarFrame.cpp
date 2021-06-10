#include "SpStatusBarFrame.h"
#include "SpUIManager.h"

namespace NAMESPACE_FRONTEND
{

	void SpStatusBarFrame::renderGUI()
	{
		if (!isVisible())
			return;

		const SpSize<sp_int> windowSize = SpUIManagerInstance->window->state()->availableRegion;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

		ImGui::Begin("StatusBar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiCond_FirstUseEver);
		loadState();

		ImGui::SetWindowSize(ImVec2((sp_float)windowSize.width, 35.0f));
		ImGui::SetWindowPos(ImVec2(0, (sp_float)windowSize.height - 30.0f));

		ImVec2 itemSize = ImGui::GetItemRectSize();

		sp_char value[50];
		std::memcpy(value, "Frame: ", sizeof(sp_char) * 7);
		SpString::convert(SpPhysicSettings::instance()->frameId(), &value[7]);

		ImGui::Text(value);

		ImGui::SameLine();

		std::memcpy(value, "FPS: ", sizeof(sp_char) * 5);
		SpString::convert(localTimer.elapsedTime(), &value[5]);
		value[10] = END_OF_STRING;

		ImGui::Text(value);

		ImGui::End();

		ImGui::PopStyleVar();

		localTimer.update();
	}

}