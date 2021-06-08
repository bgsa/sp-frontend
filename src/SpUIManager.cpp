#include "SpUIManager.h"

namespace NAMESPACE_FRONTEND
{
	SpUIManager* SpUIManagerInstance = nullptr;

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
		SpUINotificationManagerInstance->release();
		SpUIFonts::release();

		if (SpUIManagerInstance != nullptr)
		{
			sp_mem_delete(SpUIManagerInstance, SpUIManager);
			SpUIManagerInstance = nullptr;
		}

		SpUIIcons::release();
	}

}