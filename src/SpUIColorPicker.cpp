#include "SpUIColorPicker.h"

namespace NAMESPACE_FRONTEND
{

	void SpUIColorPicker_closeButtonClick(SpCloseButtonUIWindowBehaviour* e, void* parameter)
	{
		SpIFrameComponent* window = (SpIFrameComponent*)parameter;
		window->hide();
	}

	void SpUIColorPicker::init()
	{
		_minWidth = _minHeight = 400.0f;

		closeButton.onClick = SpUIColorPicker_closeButtonClick;
		closeButton.onClickParameter = this;
	}

}