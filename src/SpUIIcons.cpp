#include "SpUIIcons.h"

namespace NAMESPACE_FRONTEND
{
	SpUIIcons* SpUIIconsInstance = nullptr;

	void SpUIIcons::init()
	{
		if (SpUIIconsInstance == nullptr)
		{
			SpUIIconsInstance = sp_mem_new(SpUIIcons);
			SpUIIconsInstance->initialize();
		}
	}

	void SpUIIcons::release()
	{
		if (SpUIIconsInstance != nullptr)
		{
			sp_mem_delete(SpUIIconsInstance, SpUIIcons);
			SpUIIconsInstance = nullptr;
		}
	}

}