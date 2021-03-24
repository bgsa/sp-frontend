#ifndef SP_PROJECT_HEADER
#define SP_PROJECT_HEADER

#include "SpectrumFronend.h"

namespace NAMESPACE_FRONTEND
{
	class SpProject
	{
	private:
		sp_char* _name;
		sp_char* _folder;

	public:

		API_INTERFACE inline SpProject()
		{
			_name = nullptr;
			_folder = nullptr;
		}

		API_INTERFACE inline sp_char* name() const
		{
			return _name;
		}
		API_INTERFACE inline void name(const sp_char* newName)
		{
			if (_name != nullptr)
				sp_mem_release(_name);

			_name = (sp_char*)sp_mem_alloc(std::strlen(newName) * sizeof(sp_char));

			std::strcpy(_name, newName);
		}

		API_INTERFACE inline sp_char* folder() const
		{
			return _folder;
		}
		API_INTERFACE inline void folder(const sp_char* newFolder)
		{
			if (_folder != nullptr)
				sp_mem_release(_folder);

			_folder = (sp_char*) sp_mem_alloc(std::strlen(newFolder) * sizeof(sp_char));

			std::strcpy(_folder, newFolder);
		}

		API_INTERFACE inline void dispose()
		{
			if (_folder != nullptr)
			{
				sp_mem_release(_folder);
				_folder = nullptr;
			}

			if (_name != nullptr)
			{
				sp_mem_release(_name);
				_name = nullptr;
			}
		}

		inline ~SpProject()
		{
			dispose();
		}

	};
}

#endif // SP_PROJECT_HEADER