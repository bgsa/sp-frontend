#ifndef SP_PROJECT_MANAGER_HEADER
#define SP_PROJECT_MANAGER_HEADER

#include "SpectrumFronend.h"
#include "SpProject.h"

namespace NAMESPACE_FRONTEND
{
	class SpProjectManager
	{
	private:
		SpProject* _current;

		SpProjectManager() 
		{
			_current = nullptr;
		}

	public:
		
		API_INTERFACE inline SpProject* current() const
		{
			return _current;
		}

		API_INTERFACE inline sp_bool isLoaded() const
		{
			return _current != nullptr;
		}

		API_INTERFACE inline SpProject* newProject(const sp_char* name)
		{
			SpProject* project = sp_mem_new(SpProject)();
			project->name(name);

			_current = project;

			return project;
		}

		API_INTERFACE inline void unload()
		{
			if (_current == nullptr)
				return;

			sp_mem_delete(_current, SpProject);
			_current = nullptr;
		}

		API_INTERFACE inline void save()
		{
			if (_current == nullptr)
				return;

			// save on disk
		}

		API_INTERFACE static void init();

		API_INTERFACE static void dispose();

	};

	extern SpProjectManager* SpProjectManagerInstance;

}

#endif // SP_PROJECT_MANAGER_HEADER