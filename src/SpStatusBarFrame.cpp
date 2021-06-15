#include "SpStatusBarFrame.h"
#include "SpUIManager.h"

namespace NAMESPACE_FRONTEND
{

	void SpStatusBarFrame::begin()
	{
		const SpSize<sp_int> windowSize = SpUIManagerInstance->window->state()->availableRegion;
		const sp_float currentWidth = (sp_float)windowSize.width;
		const sp_float currentHeight = (sp_float)windowSize.height;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

		ImGui::Begin("StatusBar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiCond_FirstUseEver);

		ImGui::SetWindowSize(ImVec2(currentWidth, 35.0f));
		ImGui::SetWindowPos(ImVec2(0, currentHeight - 30.0f));
	}

	void SpStatusBarFrame::beginChild()
	{
		const ImVec2 windowSize = ImGui::GetWindowSize();
		const ImVec2 windowPos = ImGui::GetWindowPos();
		const ImVec2 statusBarSize = ImVec2(windowSize.x, 35.0f);
		ImGuiStyle style = ImGui::GetStyle();
		

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

		ImGui::BeginChild("StatusBarChild", statusBarSize, false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiCond_FirstUseEver);

		ImGui::SetWindowPos(ImVec2(windowPos.x + style.WindowPadding.x, windowPos.y + windowSize.y - style.WindowPadding.y - 30.0f));
	}

}