#include "SpUIAssetMaterialPicker.h"

namespace NAMESPACE_FRONTEND
{

	void SpUIAssetMaterialPicker_closeButtonClick(SpCloseButtonUIWindowBehaviour* e, void* parameter)
	{
		SpIFrameComponent* window = (SpIFrameComponent*)parameter;
		window->hide();
	}

	void SpUIThumbNail_DoubleClick(SpUIThumbNail* thumbnail, void* parameter)
	{
		SpUIAssetMaterialPicker* materialPicker = (SpUIAssetMaterialPicker*) parameter;

		if (materialPicker->onSelect != nullptr)
			materialPicker->onSelect((SpAssetMaterial*) thumbnail->data);
	}

	void SpUIAssetMaterialPicker::init()
	{
		_materialsLength = 10;

		if (_thumbNails != nullptr)
			releaseThumbNails();

		_thumbNails = sp_mem_new_array(SpUIThumbNail, _materialsLength);

		for (sp_uint i = 0; i < _materialsLength; i++)
		{
			_thumbNails[i].onDoubleClick = SpUIThumbNail_DoubleClick;
			_thumbNails[i].onDoubleClickParameter = this;
			_thumbNails[i].data = nullptr;

			sp_char name[20];
			std::memcpy(name, "material ", 9);
			sp_size nameLength;
			convert(i, &name[9], nameLength);
			_thumbNails[i].name(name, 9 + nameLength);
		}
	}

	SpUIAssetMaterialPicker::SpUIAssetMaterialPicker()
	{
		_minWidth = _minHeight = 400.0f;

		_materialsLength = 0;
		_thumbNails = nullptr;

		closeButton.onClick = SpUIAssetMaterialPicker_closeButtonClick;
		closeButton.onClickParameter = this;

		onSelect = nullptr;
	}

	void SpUIAssetMaterialPicker::render()
	{
		if (!isVisible())
			return;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2Zeros);
		begin("Material Picker", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

		if (_materialsLength > 0)
		{
			const sp_float thumbnailWidth = _thumbNails[0].width();
			const sp_float thumbnailHeigh = _thumbNails[0].height();
			const ImVec2 thumbnailMargin = _thumbNails[0].margin();
			const sp_float initialPositionX = _windowPosition.x;
			const sp_float maxWidth = initialPositionX + (sp_float)size().width;
			sp_float positionX = initialPositionX;
			sp_float positionY = _windowPosition.y + headerHeight();

			for (sp_uint i = 0; i < _materialsLength; i++)
			{
				if (positionX + thumbnailWidth + thumbnailHeigh > maxWidth)
				{
					positionX = initialPositionX;
					positionY += thumbnailHeigh + thumbnailMargin.y;
				}
				else
				{
					positionX += thumbnailWidth + thumbnailMargin.x;
				}

				_thumbNails[i].position(ImVec2(positionX, positionY));
				_thumbNails[i].render();
			}
		}

		closeButton.render();

		end();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

}