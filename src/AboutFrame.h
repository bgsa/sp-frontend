#ifndef ABOUT_FRAME_HEADER
#define ABOUT_FRAME_HEADER

#include "SpFrame.h"

namespace NAMESPACE_FRONTEND
{
	class AboutFrame
		: public SpFrame
	{
	public:

		API_INTERFACE void init(SpWindow* window) override
		{
			SpFrame::init(window);
			resize(300, 150);
		}

		API_INTERFACE void preRender() override
		{
		}

		API_INTERFACE void render(const RenderData& renderData) override
		{
			if (!isVisible())
				return;

			const ImVec2 windowSize = ImGui::GetIO().DisplaySize;

			ImGui::Begin("About Spectrum Engine", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiCond_FirstUseEver);
			//ImGui::SetWindowSize(ImVec2(getWidth(), getHeight()));
			//ImGui::SetWindowPos( ImVec2( ( (sp_int)getWindowSize().x >> 1) - (getWidth() >> 1) , ((sp_int)getWindowSize().y >> 1) - (getHeight() >> 1) ) );
			ImVec2 itemSize = ImGui::GetItemRectSize();

			ImGui::Text("Version: 0.1.0");
			ImGui::Text("Release Date: 01/01/2020");

			ImGui::Indent(((sp_int)itemSize.x >> 1) - 60.0f);
			ImGui::Dummy(ImVec2(0.0f, 20.0f));

			if (ImGui::Button("Close", ImVec2(100, 25)))
				hide();

			ImGui::End();

			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
				hide();
		}

		API_INTERFACE void postRender() override
		{
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "AboutFrame";
		}

	};
}

#endif // ABOUT_FRAME_HEADER