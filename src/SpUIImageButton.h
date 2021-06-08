#ifndef SP_UI_IMAGE_BUTTON_HEADER
#define SP_UI_IMAGE_BUTTON_HEADER

#include "SpectrumFronend.h"
#include "imgui/imgui.h"
#include "SpUIIcons.h"
#include "SpPhysicSettings.h"

namespace NAMESPACE_FRONTEND
{
	class SpUIImageButton
	{
	private:
		sp_bool _isClicked;

	public:
		
		/// <summary>
		/// Default constructor
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline SpUIImageButton()
		{
			_isClicked = false;
		}

		API_INTERFACE inline void init()
		{

		}

		/// <summary>
		/// Check the button is clicked
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline sp_bool isClicked() const
		{
			return _isClicked;
		}

		API_INTERFACE inline void render(const ImTextureID& textureId, const ImVec2& textureSize, ImVec2 uv1, ImVec2 uv2)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

			_isClicked = ImGui::ImageButton(textureId, textureSize, uv1, uv2);

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly))
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
		}

	};
}

#endif // SP_UI_IMAGE_BUTTON_HEADER