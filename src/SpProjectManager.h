#ifndef SP_PROJECT_MANAGER_HEADER
#define SP_PROJECT_MANAGER_HEADER

#include "SpectrumFronend.h"
#include "SpProject.h"
#include "FileSystem.h"
#include "nlohmann/json.hpp"
#include "SpRenderingAPIOpenGL.h"
#include "SpGameEngineSettings.h"

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

		inline void creteProjectStructure(const sp_char* folder, const sp_size folderLength)
		{
			sp_size length = folderLength;
			createDirectory(folder, length);

			sp_char temp[512];
			std::memcpy(temp, folder, length + 1);

			if (!endsWith(temp, SP_DIRECTORY_SEPARATOR_STR))
				temp[length++] = SP_DIRECTORY_SEPARATOR;

			std::memcpy(&temp[length], "Assets", 6);
			temp[length + 6] = END_OF_STRING;
			createDirectory(temp, length + 6);

			temp[length + 6] = SP_DIRECTORY_SEPARATOR;
			std::memcpy(&temp[length + 7], "Textures", 8);
			temp[length + 7 + 8] = END_OF_STRING;
			createDirectory(temp, length + 7 + 8);

			temp[length + 6] = SP_DIRECTORY_SEPARATOR;
			std::memcpy(&temp[length + 7], "Materials", 9);
			temp[length + 7 + 9] = END_OF_STRING;
			createDirectory(temp, length + 7 + 9);
		}

		inline void loadCameras(nlohmann::json& sceneJson, SpPhyiscs::SpScene* scene)
		{
			if (sceneJson.find("cameras") != sceneJson.end() && sceneJson["cameras"].is_array())
			{
				const sp_size camerasLength = sceneJson["cameras"].size();

				for (sp_size i = 0; i < camerasLength; i++)
				{
					nlohmann::json cameraJson = sceneJson["cameras"][i];

					const std::string cameraName = cameraJson["name"].get<std::string>();
					const sp_uint cameraIndex = scene->addCamera(cameraName.c_str(), cameraName.length());
					SpCamera* camera = scene->camerasManager()->get(cameraIndex);

					Vec3 cameraPosition;
					nlohmann::json positionJson = cameraJson["position"];
					cameraPosition.x = positionJson["x"].get<sp_float>();
					cameraPosition.y = positionJson["y"].get<sp_float>();
					cameraPosition.z = positionJson["z"].get<sp_float>();

					Vec3 cameraTarget;
					nlohmann::json targetJson = cameraJson["target"];
					cameraTarget.x = targetJson["x"].get<sp_float>();
					cameraTarget.y = targetJson["y"].get<sp_float>();
					cameraTarget.z = targetJson["z"].get<sp_float>();

					const sp_float aspectRatio = cameraJson["aspect-ratio"].get<sp_float>();
					const sp_bool yInverted = cameraJson["y-inverted"].get<sp_bool>();

					camera->init(cameraPosition, cameraTarget, Vec3Up, aspectRatio, yInverted);
					camera->fieldOfView(cameraJson["fov"].get<sp_float>());
					camera->velocity(cameraJson["velocity"].get<sp_float>());
				}
			}
		}

		inline void loadScenes(nlohmann::json& j)
		{
			SpGame* game = _current->game();

			if (j.find("scenes") != j.end() && j["scenes"].is_array())
			{
				const sp_size scenesLength = j["scenes"].size();

				for (sp_size i = 0; i < scenesLength; i++)
				{
					nlohmann::json sceneJson = j["scenes"][i];

					const std::string name = sceneJson["name"].get<std::string>();
					SpScene* scene = game->addScenes(name.c_str());

					loadCameras(sceneJson, scene);
				}
			}
		}

		inline void saveCameras(SpPhyiscs::SpScene* scene, nlohmann::ordered_json& jsonScene)
		{
			nlohmann::ordered_json jsonCameras = nlohmann::json::array();
			for (sp_size i = 0; i < scene->camerasManager()->length(); i++)
			{
				SpCamera* camera = scene->camerasManager()->get(i);
				const sp_char* cameraName = scene->camerasManager()->name(i);

				nlohmann::ordered_json jsonCamera;
				jsonCamera["name"] = cameraName;
				jsonCamera["aspect-ratio"] = camera->aspectRatio();
				jsonCamera["fov"] = camera->fieldOfView();
				jsonCamera["y-inverted"] = camera->isYAxisInverted();
				jsonCamera["velocity"] = camera->velocity();
				jsonCamera["position"] = nlohmann::ordered_json
				{
					{ "x", camera->position().x },
					{ "y", camera->position().y },
					{ "z", camera->position().z }
				};
				jsonCamera["target"] = nlohmann::ordered_json
				{
					{ "x", camera->target().x },
					{ "y", camera->target().y },
					{ "z", camera->target().z }
				};

				jsonCameras.push_back(jsonCamera);
			}
			jsonScene["cameras"] = jsonCameras;
		}

		inline void saveTransforms(SpPhyiscs::SpScene* scene, nlohmann::ordered_json& jsonScene)
		{
			nlohmann::ordered_json jsonTransforms = nlohmann::json::array();
			for (sp_size i = 0; i < scene->transformManager()->length(); i++)
			{
				SpTransform* transform = scene->transform(i);

				nlohmann::ordered_json jsonTransform;
				jsonTransform["position"] = nlohmann::ordered_json
				{
					{ "x", transform->position.x },
					{ "y", transform->position.y },
					{ "z", transform->position.z }
				};
				jsonTransform["orientation"] = nlohmann::ordered_json
				{
					{ "w", transform->orientation.w },
					{ "x", transform->orientation.x },
					{ "y", transform->orientation.y },
					{ "z", transform->orientation.z }
				};
				jsonTransform["scale"] = nlohmann::ordered_json
				{
					{ "x", transform->scaleVector.x },
					{ "y", transform->scaleVector.y },
					{ "z", transform->scaleVector.z }
				};

				jsonTransforms.push_back(jsonTransform);
			}
			jsonScene["transforms"] = jsonTransforms;
		}

		inline void saveGameObjects(SpPhyiscs::SpScene* scene, nlohmann::ordered_json& jsonScene)
		{
			nlohmann::ordered_json jsonGameObjects = nlohmann::json::array();
			for (sp_size i = 0; i < scene->gameObjectsLength(); i++)
			{
				SpGameObject* gameObject = scene->gameObject(i);

				nlohmann::ordered_json jsonGameObject;
				jsonGameObject["type"] = gameObject->type();
				jsonGameObject["renderable-index"] = gameObject->renderableObjectIndex();

				jsonGameObjects.push_back(jsonGameObject);
			}
			jsonScene["game-objects"] = jsonGameObjects;
		}

		inline void saveRenderableObjects(SpPhyiscs::SpScene* scene, nlohmann::ordered_json& jsonScene)
		{
			nlohmann::ordered_json jsonRenderables = nlohmann::json::array();
			for (sp_size i = 0; i < scene->renderableObjectManager()->length(); i++)
			{
				SpRenderableObject* renderable = scene->renderableObjectManager()->get(i);

				nlohmann::ordered_json jsonRenderable;
				jsonRenderable["type"] = renderable->type();
				jsonRenderable["visible"] = renderable->isVisible();
				jsonRenderable["game-object-index"] = renderable->gameObject();
				jsonRenderable["mesh-data"] = renderable->meshData();

				jsonRenderables.push_back(jsonRenderable);
			}
			jsonScene["renderable-objects"] = jsonRenderables;
		}

		inline void saveLightings(SpPhyiscs::SpScene* scene, nlohmann::ordered_json& jsonScene)
		{
			nlohmann::ordered_json jsonLighings = nlohmann::json::array();
			for (sp_size i = 0; i < scene->lightingManager()->length(); i++)
			{
				SpLightSource* light = scene->lightingManager()->get(i);

				nlohmann::ordered_json jsonLight;
				jsonLight["type"] = light->type();
				jsonLight["enabled"] = light->isEnabled();
				jsonLight["static"] = light->isStatic();
				jsonLight["factor"] = light->factor();
				jsonLight["color"] = nlohmann::ordered_json{
					{ "r", light->color().red },
					{ "g", light->color().green },
					{ "b", light->color().blue }
				};
				jsonLight["position"] = nlohmann::ordered_json{
					{ "x", light->position().x },
					{ "y", light->position().y },
					{ "z", light->position().z }
				};
				jsonLight["direction"] = nlohmann::ordered_json{
					{ "x", light->direction().x },
					{ "y", light->direction().y },
					{ "z", light->direction().z }
				};
				jsonLight["spot-type"] = light->spotlightType();
				jsonLight["spot-angle"] = light->spotlightAngle();

				jsonLighings.push_back(jsonLight);
			}
			jsonScene["lightings"] = jsonLighings;
		}

		inline void saveScenes(nlohmann::ordered_json& json)
		{
			nlohmann::json scenesJson = nlohmann::json::array();

			for (SpVectorItem<SpScene*>* item = _current->game()->scenes()->begin(); item != nullptr; item = item->next())
			{
				SpScene* scene = item->value();

				nlohmann::ordered_json jsonScene = nlohmann::ordered_json
				{
					{ "name", scene->name() },
					{ "game-objects-length", scene->gameObjectsLength() },
					{ "cameras-length", scene->camerasManager()->length() },
					{ "lighting-length", scene->lightingManager()->length() },
					{ "meshes-length", scene->meshManager()->length() },
					{ "renderable-objects-length", scene->renderableObjectManager()->length() },
					{ "transform-length", scene->transformManager()->length() }
				};

				saveCameras(scene, jsonScene);
				saveTransforms(scene, jsonScene);
				saveGameObjects(scene, jsonScene);
				saveRenderableObjects(scene, jsonScene);
				saveLightings(scene, jsonScene);

				scenesJson.push_back(jsonScene);
			}
			json["scenes"] = scenesJson;
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

		API_INTERFACE inline SpProject* newProject(const sp_char* name, const sp_int gameType, const sp_char* folder)
		{
			SpProject* project = sp_mem_new(SpProject)();
			project->name(name);
			project->type(gameType);

			const sp_size folderLength = std::strlen(folder);
			const sp_size nameLength = std::strlen(name);

			sp_char projectFolder[512];
			directoryAddPath(folder, folderLength, name, nameLength, projectFolder);

			const sp_size projectFolderLength = folderLength + nameLength;

			project->folder(projectFolder, projectFolderLength);

			creteProjectStructure(projectFolder, projectFolderLength);

			project->game()->renderingApi(SP_RENDERING_API_OPENGL, sp_mem_new(SpRenderingAPIOpenGL));

			SpScene* scene = project->game()->addScenes("Scene 1");
			scene->addCamera("Camera 1", 8);

			_current = project;

			sp_char fullFilename[512];
			sp_size fullFilenameLength;
			project->filename(fullFilename, fullFilenameLength);

			SpGameEngineSettingsInstance->addLastProject(fullFilename, fullFilenameLength);
			SpGameEngineSettingsInstance->save();

			save();

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

			const sp_size contentLength = fileSize(filename);
			sp_char* content = ALLOC_ARRAY(sp_char, contentLength);
			readTextFile(filename, content, contentLength);

			nlohmann::json j = nlohmann::json::parse(content);

			ALLOC_RELEASE(content);

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
			const sp_size folderLength = std::strlen(folder);

			project->folder(folder, folderLength);

			SpGame* game = project->game();
			game->renderingApi(SP_RENDERING_API_OPENGL, sp_mem_new(SpRenderingAPIOpenGL));
			
			_current = project;

			loadScenes(j);

			project->loadLibraries();
		}

		API_INTERFACE inline void save()
		{
			if (_current == nullptr)
				return;

			nlohmann::ordered_json json;
			json["name"] = _current->name();
			json["type"] = _current->game()->gameType();

			saveScenes(json);

			std::string jsonAsString = json.dump(4);

			sp_char fullname[512];
			directoryAddPath(
				_current->folder(), std::strlen(_current->folder()), 
				_current->name(), std::strlen(_current->name()), 
				fullname);
		
			std::strcat(fullname, SP_FILENAME_PROJECT_SUFFIX);

			SP_FILE file;
			file.open(fullname, std::ios_base::out);
			file.write(jsonAsString.c_str());
			file.close();
		}

		API_INTERFACE static void init();

		API_INTERFACE static void dispose();

	};

	extern SpProjectManager* SpProjectManagerInstance;

}

#endif // SP_PROJECT_MANAGER_HEADER