#include "SpMemoryMapViewerFrame.h"

namespace NAMESPACE_FRONTEND
{

	void closeButtonClick(SpCloseButtonUIWindowBehaviour* e, void* parameter)
	{
		SpMemoryMapViewerFrame* window = (SpMemoryMapViewerFrame*)parameter;
		window->hide();
	}

	void SpMemoryMapViewerFrame::init()
	{
		_minWidth = 300;
		_minHeight = 100;
		resize(800, 600);
		closeButton.onClick = closeButtonClick;
		closeButton.onClickParameter = this;
	}

}