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

	SpUIAssetMaterialPicker::SpUIAssetMaterialPicker()
	{
		_minWidth = _minHeight = 400.0f;

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

		ImVec2 pos = ImVec2(_windowPosition.x, _windowPosition.y + headerHeight());

		SpUIThumbNail thumbNail;
		thumbNail.onDoubleClick = SpUIThumbNail_DoubleClick;
		thumbNail.onDoubleClickParameter = this;
		thumbNail.data = nullptr;
		thumbNail.position(pos);
		//thumbNail.name("material 1 material 1", 21);
		//thumbNail.name("material 1", 10);
		thumbNail.name("material 1 material 1 bricktijolaokk", 21 + 15);
		thumbNail.render();

		closeButton.render();

		end();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

}