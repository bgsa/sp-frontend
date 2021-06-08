#ifndef SP_UI_TEXT_COMPONENT_HEADER
#define SP_UI_TEXT_COMPONENT_HEADER

#include "SpectrumFronend.h"
#include "SpUIFonts.h"

namespace NAMESPACE_FRONTEND
{
	class SpUITextComponent
	{
	private:
		
		void breakText(const sp_char* text, const sp_size textLength, sp_char* newText)
		{
			const sp_float charSize = font->FallbackAdvanceX;
			sp_float containerWidth = ImGui::GetContentRegionAvailWidth();
			
			const sp_uint maxCharByLine = (sp_uint)(containerWidth / charSize);
			sp_uint max = maxCharByLine;
			
			sp_uint lastSpace = ZERO_UINT;
			sp_uint index = ZERO_UINT;
			sp_uint newTextIndex = ZERO_UINT;
			sp_uint previousLastSpace = ZERO_UINT;

			while (text[index] != END_OF_STRING)
			{
				if (index != max)
				{
					newText[newTextIndex++] = text[index];

					if (text[index] == ' ')
						lastSpace = index;
				}
				else
				{
					newText[lastSpace] = END_OF_LINE;

					max += maxCharByLine - (index - lastSpace);

					if (lastSpace == previousLastSpace)
					{
						std::memcpy(&newText[lastSpace], "...\0", 4);
						return;
					}

					previousLastSpace = lastSpace;
					index = lastSpace;
					newTextIndex = lastSpace + 1;
				}

				index++;
			}
		}

	public:
		ImFont* font;

		/// <summary>
		/// Default constructor
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline SpUITextComponent()
		{
		}

		API_INTERFACE inline void init()
		{
			font = SpUIFontsInstance->fontDefault;
		}

		API_INTERFACE inline void render(const sp_char* message)
		{
			const sp_size messageLength = std::strlen(message);

			const sp_int maxLineSupported = 100;
			sp_char* newMessage = ALLOC_ARRAY(sp_char, messageLength + 1 + maxLineSupported);
			newMessage[messageLength] = END_OF_STRING;

			breakText(message, messageLength, newMessage);

			ImGui::PushFont(font);

			ImGui::Text(newMessage);

			ImGui::PopFont();

			ALLOC_RELEASE(newMessage);
		}

	};
}

#endif // SP_UI_TEXT_COMPONENT_HEADER