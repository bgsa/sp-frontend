#ifndef SP_IMGUI_HEADER
#define SP_IMGUI_HEADER

#include "SpectrumFronend.h"
#include "imgui/imgui.h"

namespace NAMESPACE_FRONTEND
{

	#define ImGui_ColorWhite (IM_COL32(255, 255, 255, 255))
	#define ImGui_ColorBlack (IM_COL32(0, 0, 0, 255))

	const ImVec2 ImVec2Zeros(0.0f, 0.0f);

	/// <summary>
	/// Compute the size of text gieven a limited width and the characters length
	/// </summary>
	/// <param name="font">Font to be used</param>
	/// <param name="textMaxWidth">Max Width</param>
	/// <param name="textWidth">Text Width</param>
	/// <returns>Characteres Length</returns>
	inline sp_size computeTextSize(const sp_char* text, const ImFont* font, const sp_float& textMaxWidth, sp_float& textWidth)
	{
		sp_size index = 0;
		textWidth = 0;

		while (text[index] != END_OF_STRING)
		{
			sp_wchar wc;
			convert(&text[index], &wc, 1);

			const ImFontGlyph* glyph = font->FindGlyph(wc);

			//if (glyph->Visible)
			{
				if (textWidth + glyph->AdvanceX > textMaxWidth)
					break;

				textWidth += glyph->AdvanceX;
			}

			index++;
		}

		//if (index > 0) index--;

		return index;
	}

}

#endif // SP_IMGUI_HEADER