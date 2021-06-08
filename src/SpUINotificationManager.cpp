#include "SpUINotificationManager.h"

namespace NAMESPACE_FRONTEND
{

	SpUINotificationManager* SpUINotificationManagerInstance = nullptr;

	void SpUINotificationManager::init()
	{
		if (SpUINotificationManagerInstance == nullptr)
		{
			SpUINotificationManagerInstance = sp_mem_new(SpUINotificationManager)();
			SpUINotificationManagerInstance->initialize();
		}
	}

	void SpUINotificationManager::release()
	{
		if (SpUINotificationManagerInstance != nullptr)
		{
			sp_mem_delete(SpUINotificationManagerInstance, SpUINotificationManager);
			SpUINotificationManagerInstance = nullptr;
		}
	}

}