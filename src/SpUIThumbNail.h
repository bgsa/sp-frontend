#ifndef SP_UI_THUMB_NAIL_HEADER
#define SP_UI_THUMB_NAIL_HEADER

#include "SpectrumFronend.h"
#include "SpImGui.h"
#include "SpUIFonts.h"

namespace NAMESPACE_FRONTEND
{
	class SpUIThumbNail
	{
	private:
		sp_bool _visible;
		ImVec4 _backgroundColor;
		ImVec2 _position;
		ImVec2 _size;
		ImVec2 _margin;
		sp_bool _selected;
		sp_char _name[100];
		sp_uint _nameLength;

	public:
		ImTextureID texture;
		void* data;

		void (*onSelect)(SpUIThumbNail*, void*);
		void* onSelectParameter;

		void (*onDoubleClick)(SpUIThumbNail*, void*);
		void* onDoubleClickParameter;
		
		/// <summary>
		/// Default constructor
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline SpUIThumbNail()
		{
			_visible = true;
			_backgroundColor = ImVec4(0.0f, 0.0f, 0.7f, 0.0f);
			_position = ImVec2(0.0f, 0.0f);
			_size = ImVec2(120.0f, 160.0f);
			_margin = ImVec2(10.0f, 10.0f);
			texture = 0;
			std::memset(_name, 0, sizeof(sp_char) * 100);
			_nameLength = 0;
			_selected = false;
			data = nullptr;

			onSelect = nullptr;
			onSelectParameter = nullptr;

			onDoubleClick = nullptr;
			onDoubleClickParameter = nullptr;
		}

		/// <summary>
		/// Set the position of thumbnail
		/// </summary>
		/// <param name="newPosition"></param>
		/// <returns></returns>
		API_INTERFACE inline void position(const ImVec2& newPosition)
		{
			_position = newPosition;
		}

		/// <summary>
		/// Set the size of the thumbnail
		/// </summary>
		/// <param name="newSize"></param>
		/// <returns></returns>
		API_INTERFACE inline void size(const ImVec2& newSize)
		{
			_size = newSize;
		}

		/// <summary>
		/// Set the margin of the thumbnail
		/// </summary>
		/// <param name="newMargin"></param>
		/// <returns></returns>
		API_INTERFACE inline void margin(const ImVec2& newMargin)
		{
			_margin = newMargin;
		}

		/// <summary>
		/// Check this thumbnail is selected
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline sp_bool isSelected() const
		{
			return _selected;
		}

		/// <summary>
		/// Select this thumbnail
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void select()
		{
			_selected = true;
			_backgroundColor.w = 1.0f;
		}

		/// <summary>
		/// Deselect this thumbnail
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void deselect()
		{
			_selected = false;
			_backgroundColor.w = 0.0f;
		}

		/// <summary>
		/// Set the name of the thumbnail
		/// </summary>
		/// <param name="newName"></param>
		/// <returns></returns>
		API_INTERFACE inline void name(const sp_char* newName, const sp_uint newNameLength)
		{
			std::memcpy(_name, newName, newNameLength);
			_name[newNameLength] = END_OF_STRING;
			_nameLength = newNameLength;
		}

		/// <summary>
		/// Render the color picker
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void render()
		{
			if (!_visible)
				return;

			const ImVec2 windowPos = ImGui::GetWindowPos();
			const ImVec2 minPoint = ImVec2(_position.x + _margin.x, _position.y + _margin.y);
			const ImVec2 maxPoint = ImVec2(_position.x + _size.x + _margin.x, _position.y + _size.y + _margin.y);

			const ImVec2 selectablePos = ImVec2(minPoint.x - windowPos.x, minPoint.y - windowPos.y);

			ImGui::SetCursorPos(selectablePos);
			ImGui::Selectable("##", _selected, ImGuiSelectableFlags_AllowDoubleClick, _size);

			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && onDoubleClick != nullptr)
					onDoubleClick(this);

				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					select();

					if (onSelect != nullptr)
						onSelect(this);
				}
			}

			ImDrawList* drawList = ImGui::GetWindowDrawList();

			drawList->AddRectFilled(minPoint, maxPoint, ImGui::ColorConvertFloat4ToU32(_backgroundColor));

			const ImVec2 minPointImg = ImVec2(minPoint.x + 8.0f, minPoint.y + 8.0f);
			const ImVec2 maxPointImg = ImVec2(maxPoint.x - 8.0f, maxPoint.y - 45.0f);
			drawList->AddImage(texture, minPointImg, maxPointImg);

			if (_nameLength > 0)
			{
				ImGuiIO io = ImGui::GetIO();
				//io.Fonts.
				ImFont* font = SpUIFontsInstance->fontDefault;

				ImGui::PushFont(font);

				const ImVec2 textMargin(3.0f, 0.0f);
				ImVec2 textPosition = ImVec2(minPoint.x + textMargin.x, maxPointImg.y + 5.0f);
				const sp_float textMaxWidth = maxPoint.x - textPosition.x - textMargin.x;
				sp_float textWidth;

				sp_size nameIndex = computeTextSize(_name, font, textMaxWidth, textWidth);

				if (nameIndex == _nameLength)
					drawList->AddText(textPosition, ImGui_ColorWhite, _name);
				else
				{
					sp_char temp[100];
					std::memcpy(temp, _name, nameIndex);
					temp[nameIndex] = END_OF_STRING;

					drawList->AddText(textPosition, ImGui_ColorWhite, temp);

					const sp_size newNameLength = _nameLength - nameIndex;
					sp_size newNameIndex = computeTextSize(&_name[nameIndex], font, textMaxWidth, textWidth);

					if (newNameIndex > 0)
					{
						std::memcpy(temp, &_name[nameIndex], newNameIndex);
						temp[newNameIndex] = END_OF_STRING;

						if (newNameIndex < newNameLength)  // trim
							std::memcpy(&temp[newNameIndex - 3], "...", 3);

						textPosition.y += font->FontSize + textMargin.y;
						drawList->AddText(textPosition, ImGui_ColorWhite, temp);
					}
				}

				ImGui::PopFont();
			}
		}

	};
}

#endif // SP_UI_THUMB_NAIL_HEADER