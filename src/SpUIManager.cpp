#include "SpUIManager.h"

namespace NAMESPACE_FRONTEND
{
	sp_bool showLoadProject = false;

	void loadProjectDialog_OnClose()
	{
		showLoadProject = false;
	}

}