#ifndef SP_PROJECT_HEADER
#define SP_PROJECT_HEADER

#include "SpectrumFronend.h"
#include "SpGame.h"
#include "SpGame2D.h"
#include "SpGame3D.h"

namespace NAMESPACE_FRONTEND
{
	class SpProject
	{
	private:
		sp_char* _name;
		sp_char* _folder;
		SpGame* _game;

	public:
		
		/// <summary>
		/// Default constructor
		/// </summary>
		/// <returns>void</returns>
		API_INTERFACE inline SpProject()
		{
			_name = nullptr;
			_folder = nullptr;
			_game = nullptr;
		}

		/// <summary>
		/// Get the name of the project
		/// </summary>
		/// <returns>Name</returns>
		API_INTERFACE inline sp_char* name() const
		{
			return _name;
		}

		/// <summary>
		/// Set the name of the project
		/// </summary>
		/// <param name="newName">New Name</param>
		/// <returns>void</returns>
		API_INTERFACE inline void name(const sp_char* newName)
		{
			if (_name != nullptr)
				sp_mem_release(_name);

			_name = (sp_char*)sp_mem_alloc(std::strlen(newName) * sizeof(sp_char));

			std::strcpy(_name, newName);
		}

		/// <summary>
		/// Get the game of the project
		/// </summary>
		/// <returns>Game</returns>
		API_INTERFACE inline SpGame* game() const
		{
			return _game;
		}

		/// <summary>
		/// Get the folder of the project
		/// </summary>
		/// <returns>folder</returns>
		API_INTERFACE inline sp_char* folder() const
		{
			return _folder;
		}

		/// <summary>
		/// Set the folder of the project
		/// </summary>
		/// <param name="newFolder">New Folder</param>
		/// <returns>void</returns>
		API_INTERFACE inline void folder(const sp_char* newFolder)
		{
			if (_folder != nullptr)
				sp_mem_release(_folder);

			_folder = (sp_char*) sp_mem_alloc((std::strlen(newFolder) + 1u) * sizeof(sp_char));

			std::strcpy(_folder, newFolder);
		}

		/// <summary>
		/// Set the type of the game project
		/// </summary>
		/// <param name="projectType">Project Type</param>
		/// <returns>void</returns>
		API_INTERFACE inline void type(const sp_int projectType)
		{
			if (_game != nullptr)
			{
				sp_mem_release(_game);
				_game = nullptr;
			}

			switch (projectType)
			{
			case 2:
				_game = sp_mem_new(SpGame2D)();
				break;

			case 3:
				_game = sp_mem_new(SpGame3D)();
				break;

			default:
				sp_assert(false, "InvalidArgumentException");
				break;
			}
		}

		/// <summary>
		/// Release all allocated resources
		/// </summary>
		/// <returns>void</returns>
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

			if (_game != nullptr)
			{
				_game->dispose();
				sp_mem_release(_game);
				_game = nullptr;
			}
		}

		/// <summary>
		/// Default destructor
		/// </summary>
		inline ~SpProject()
		{
			dispose();
		}

	};
}

#endif // SP_PROJECT_HEADER