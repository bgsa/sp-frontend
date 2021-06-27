#ifndef SP_UI_COLOR_PICKER_HEADER
#define SP_UI_COLOR_PICKER_HEADER

#include "SpectrumFronend.h"
#include "SpIFrameComponent.h"
#include "SpColorRGBA.h"
#include "SpCloseButtonUIWindowBehaviour.h"

namespace NAMESPACE_FRONTEND
{
	class SpUIColorPicker
		: public SpIFrameComponent
	{
	private:
		SpColorRGBA* _color;

	public:
		
		SpCloseButtonUIWindowBehaviour closeButton;

		/// <summary>
		/// Initialize the color picker
		/// </summary>
		/// <returns></returns>
		API_INTERFACE void init();

		API_INTERFACE inline void color(SpColorRGBA* reference)
		{
			_color = reference;
		}

		/// <summary>
		/// Render the color picker
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void render() override
		{
			if (!isVisible())
				return;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2Zeros);
			begin("RGBA Color", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

			ImGui::ColorPicker4("MyColor##4", (sp_float*)_color, ImGuiColorEditFlags_AlphaBar);

			closeButton.render();

			end();
			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
		}

	};
}

#endif // SP_UI_COLOR_PICKER_HEADER