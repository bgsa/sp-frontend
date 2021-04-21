#ifndef SP_PROJECT_HEADER
#define SP_PROJECT_HEADER

#include "SpectrumFronend.h"
#include "SpGame.h"
#include "SpGame2D.h"
#include "SpGame3D.h"
#include "SpDynamicLibrary.h"
#include "SpVector.h"

namespace NAMESPACE_FRONTEND
{
	class SpProject
	{
	private:
		sp_char* _name;
		sp_char* _folder;
		SpGame* _game;
		SpVector<SpDynamicLibrary*>* _libraries;

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
			_libraries = nullptr;
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
		/// Load the libraries of the project
		/// </summary>
		/// <returns>void</returns>
		API_INTERFACE inline void loadLibraries()
		{
			sp_char libDir[256];
			SpDirectory::buildPath(_folder, "lib", libDir);

			sp_char files[256 * 1000];
			files[0] = END_OF_STRING;

			SpDirectory dir(libDir);
			dir.files(files);

			_libraries = sp_mem_new(SpVector<SpDynamicLibrary*>)();

			const sp_uint libDirLength = strlen(libDir);
			sp_uint index = ZERO_UINT;

			while (files[index] != END_OF_STRING)
			{
				if (files[index] == END_OF_LINE)
					index++;

				sp_char file[256];
				std::memcpy(file, libDir, sizeof(sp_char) * libDirLength);
				file[libDirLength] = SP_DIRECTORY_SEPARATOR;

				sp_int length = strIndexOf(&files[index], END_OF_LINE);

				if (length == -1)
					length = strlen(&files[index]);
				
				std::memcpy(&file[libDirLength + 1u], &files[index], sizeof(sp_char) * length + sizeof(sp_char));
				file[libDirLength + 1u + length] = END_OF_STRING;

#ifdef WINDOWS
				if (endsWith(file, ".dll"))
#else
				if (endsWith(file, ".so"))
#endif		
				{
					SpDynamicLibrary* library = sp_mem_new(SpDynamicLibrary);
					library->load(file);
					_libraries->add(library);
				}

				index += length;
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

			if (_libraries != nullptr)
			{
				sp_mem_delete(_libraries, SpVector<SpDynamicLibrary*>);
				_libraries = nullptr;
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