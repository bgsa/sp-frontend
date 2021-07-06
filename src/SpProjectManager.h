#ifndef SP_PROJECT_MANAGER_HEADER
#define SP_PROJECT_MANAGER_HEADER

#include "SpectrumFronend.h"
#include "SpProject.h"
#include "FileSystem.h"
#include "nlohmann/json.hpp"
#include "SpRenderingAPIOpenGL.h"

#define SP_FILENAME_PROJECT_SUFFIX ".spp"

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

		API_INTERFACE inline SpProject* newProject(const sp_char* name, const sp_int gameType)
		{
			SpProject* project = sp_mem_new(SpProject)();
			project->name(name);
			project->type(gameType);

			project->game()->renderingApi(SP_RENDERING_API_OPENGL, sp_mem_new(SpRenderingAPIOpenGL));

			SpScene* scene = project->game()->addScenes("Scene 1");
			scene->addGameObject(SP_GAME_OBJECT_TYPE_CAMERA, "Camera 1");

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

		API_INTERFACE inline void load(const sp_char* filename)
		{
			unload();

			SP_FILE file;
			SpString* content = file.readTextFile(filename);
			file.close();

			nlohmann::json j = nlohmann::json::parse(content->data());

			sp_mem_delete(content, SpString);

			SpProject* project = sp_mem_new(SpProject)();

			if (j.find("name") != j.end())
			{
				std::string name = j["name"].get<std::string>();
				project->name(name.c_str());
			}

			if (j.find("type") != j.end())
			{
				const sp_int type = j["type"].get<sp_int>();
				project->type(type);
			}

			sp_char folder[256];
			SpDirectory::directoryFromFile(filename, folder);

			project->folder(folder);

			project->loadLibraries();

			_current = project;
		}

		API_INTERFACE inline void save()
		{
			if (_current == nullptr)
				return;

			nlohmann::json json;
			json["name"] = _current->name();
			json["type"] = _current->game()->gameType();

			for (SpVectorItem<SpWorld*>* item = SpWorldManagerInstance->worlds()->begin(); item != nullptr; item = item->next())
			{
				SpWorld* world = item->value();

				nlohmann::json jsonWorld = nlohmann::json
				{
					{ "name", world->name },
					{ "objects-length", world->objectsLength() }
				};

				json["worlds"].push_back(jsonWorld);
			}

			std::string jsonAsString = json.dump(4);

			sp_char fullname[512];
			std::strcpy(fullname, _current->folder());
			std::strcat(fullname, _current->name());
			std::strcat(fullname, SP_FILENAME_PROJECT_SUFFIX);

			SP_FILE file;
			file.open(fullname, std::ios_base::out);
			file.write(jsonAsString.c_str());
			file.close();

			sp_char str[512];
			SpDirectory::buildPath(_current->folder(), "lib", str);
			SpDirectory dir(str);
			dir.create();

			SpDirectory::buildPath(_current->folder(), "src", str);
			dir = str;
			dir.create();
		}

		API_INTERFACE static void init();

		API_INTERFACE static void dispose();

	};

	extern SpProjectManager* SpProjectManagerInstance;

}

#endif // SP_PROJECT_MANAGER_HEADER