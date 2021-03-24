#include "SpProjectManager.h"

namespace NAMESPACE_FRONTEND
{

	SpProjectManager* SpProjectManagerInstance = nullptr;

	void SpProjectManager::init()
	{
		if (SpProjectManagerInstance != nullptr)
			return;

		SpProjectManagerInstance = sp_mem_new(SpProjectManager)();
	}

	void SpProjectManager::dispose()
	{
		if (SpProjectManagerInstance == nullptr)
			return;

		if (SpProjectManagerInstance->_current != nullptr)
		{
			sp_mem_delete(SpProjectManagerInstance->_current, SpProject);
			SpProjectManagerInstance->_current = nullptr;
		}

		sp_mem_delete(SpProjectManagerInstance, SpProjectManager);
		SpProjectManagerInstance = nullptr;
	}

}