#ifndef PROJECT_EXPLORER_FRAME_HEADER
#define PROJECT_EXPLORER_FRAME_HEADER

#include "SpFrame.h"
#include "SpProjectManager.h"
#include "SpScene.h"
#include "NewSceneFrame.h"
#include "SpShader.h"
#include "SpGame.h"
#include "SpGameObjectFactoryPlane.h"
#include "FileSystem.h"
#include "SpAssetMaterial.h"
#include "SpAssetMaterialSerializerJson.h"

namespace NAMESPACE_FRONTEND
{
	class ProjectExplorerFrame
		: public SpFrame
	{
	private:
		NewSceneFrame newSceneFrame;

		void renderScenesContextMenu()
		{
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("New Scene"))
				{
					newSceneFrame.show();
				}
				ImGui::EndPopup();
			}
		}

		void renderSceneNode(SpVectorItem<SpScene*>* sceneItem, const sp_bool canDeleteScene);
		void renderSceneContextMenu(SpVectorItem<SpScene*>* sceneItem, const sp_bool canDeleteScene)
		{
			if (ImGui::BeginPopupContextItem())
			{
				if (canDeleteScene)
				{
					if (ImGui::MenuItem("Delete"))
						SpProjectManagerInstance->current()->game()->removeScene(sceneItem);
				}
				else
				{
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
					ImGui::Text("Delete");
					ImGui::PopStyleVar();
				}
				ImGui::EndPopup();
			}
		}

		void renderGameObjectsNode(SpScene* scene);
		void renderGameObjectContextMenu(SpScene* scene, const sp_uint index);
		void renderGameObjectsContextMenu(SpScene* scene);

		void renderMaterialContextMenu(SpScene* scene);
		void renderMaterialsNode(SpScene* scene);

		void renderCameraContextMenu(SpScene* scene);
		void renderCameraNode(SpScene* scene);

		void renderMeshesNode(SpScene* scene);

		void renderShadersNode(SpScene* scene);

		void renderLightingNode(SpScene* scene);
		void renderLightingContextMenu(SpScene* scene);
		void renderLightContextMenu(SpScene* scene, const sp_uint index);

		void renderAssetsNode();

		inline void renderAssetFolderNodeContextMenu(const sp_char* folder, const sp_size folderLength)
		{
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::BeginMenu("Add"))
				{
					if (ImGui::MenuItem("New Folder", NULL, false, true))
					{
						sp_char newFolderName[100];
						sp_size newFolderNameLength = 11;
					
						std::memcpy(newFolderName, "New Folder ", newFolderNameLength);
					
						const sp_size frameId = SpPhysicSettings::instance()->frameId();
						sp_size frameIdLength;
						convert(frameId, &newFolderName[newFolderNameLength], frameIdLength);
						newFolderNameLength += frameIdLength;
						newFolderName[newFolderNameLength] = END_OF_STRING;

						sp_char fullname[SP_DIRECTORY_MAX_LENGTH];
						directoryAddPath(folder, folderLength, newFolderName, newFolderNameLength, fullname);

						createDirectory(fullname, (sp_uint)(folderLength + newFolderNameLength + 1));
					}

					if (ImGui::MenuItem("New Material", NULL, false, true))
					{
						SpAssetMaterialSerializerJson serializer;
						SpAssetMaterial defaultMaterial;
						nlohmann::ordered_json materialJson;

						serializer.serialize(&defaultMaterial, materialJson);
						std::string materialAsString = materialJson.dump(4);

						const sp_size frameId = SpPhysicSettings::instance()->frameId();
						sp_char materialName[100];
						sp_size materialNameLength = 9;
						sp_size frameIdStrLength;
						std::memcpy(materialName, "Material ", materialNameLength);
						convert(frameId, &materialName[materialNameLength], frameIdStrLength);
						materialNameLength += frameIdStrLength;
						std::memcpy(&materialName[materialNameLength], ".spm", 4);
						materialNameLength += 4;
						materialName[materialNameLength] = END_OF_STRING;

						sp_char fullname[SP_DIRECTORY_MAX_LENGTH];
						directoryAddPath(folder, folderLength, materialName, materialNameLength, fullname);

						writeTextFile(fullname, materialAsString.c_str(), materialAsString.length());
					}

					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}
		}

		inline void renderAssetFolderNode(const sp_char* folder, const sp_size folderLength);

		inline void renderFilesNode(const sp_char* folder, const sp_size folderLength)
		{
			const sp_size fsLen = filesLength(folder, folderLength);

			if (fsLen == 0)
				return;

			sp_char* fs = ALLOC_ARRAY(sp_char, fsLen * SP_DIRECTORY_MAX_LENGTH);
			files(folder, folderLength, fs);

			for (sp_size i = 0; i < fsLen; i++)
			{
				sp_char name[SP_DIRECTORY_MAX_LENGTH];
				const sp_size nameLength = std::strlen(&fs[i * SP_DIRECTORY_MAX_LENGTH]);
				std::memcpy(name, &fs[i * SP_DIRECTORY_MAX_LENGTH], nameLength);
				name[nameLength] = END_OF_STRING;

				if (ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_Leaf))
				{
					//if (ImGui::IsItemClicked())
					ImGui::TreePop();
				}
			}

			ALLOC_RELEASE(fs);
		}

	public:

		API_INTERFACE void init(SpWindow* window) override
		{
			SpFrame::init(window);
			resize(300, height() - 25);
			show();

			newSceneFrame.init(window);
		}

		API_INTERFACE void preRender() override
		{
		}

		API_INTERFACE void render() override
		{

		}

		API_INTERFACE void renderGUI() override;

		API_INTERFACE void postRender() override
		{
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "ProjectExplorerFrame";
		}

	};
}

#endif // PROJECT_EXPLORER_FRAME_HEADER