#include "SpUIManager.h"

namespace NAMESPACE_FRONTEND
{
	SpUIManager* SpUIManagerInstance = nullptr;
	sp_bool showLoadProject = false;

	void SpUIManager::initialize()
	{
		if (SpUIManagerInstance == nullptr)
		{
			SpUIManagerInstance = sp_mem_new(SpUIManager)();
			//SpUIManagerInstance->init();
		}
	}

	void SpUIManager::release()
	{
		if (SpUIManagerInstance != nullptr)
		{
			sp_mem_delete(SpUIManagerInstance, SpUIManager);
			SpUIManagerInstance = nullptr;
		}

		SpUIIcons::release();
	}

	void loadProjectDialog_OnClose()
	{
		showLoadProject = false;
	}

}