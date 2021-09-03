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

		inline void renderFolderNode(const sp_char* folder, const sp_size folderLength)
		{
			const sp_size subdirLength = subdirectoriesLength(folder, folderLength);

			if (subdirLength == 0)
				return;

			sp_char* subdirs = ALLOC_ARRAY(sp_char, subdirLength * SP_DIRECTORY_MAX_LENGTH);
			subdirectories(folder, folderLength, subdirs);

			for (sp_size i = 0; i < subdirLength; i++)
			{
				sp_char name[SP_DIRECTORY_MAX_LENGTH];
				const sp_size nameLength = std::strlen(&subdirs[i * SP_DIRECTORY_MAX_LENGTH]);
				std::memcpy(name, &subdirs[i * SP_DIRECTORY_MAX_LENGTH], nameLength);
				name[nameLength] = END_OF_STRING;

				const sp_bool folderNodeOpened = ImGui::TreeNode(name);

				if (folderNodeOpened)
				{
					sp_char subFolder[SP_DIRECTORY_MAX_LENGTH];
					directoryAddPath(folder, folderLength, name, nameLength, subFolder);

					renderFolderNode(subFolder, folderLength + nameLength + 1);
					renderFilesNode(subFolder, folderLength + nameLength + 1);

					ImGui::TreePop();
				}
			}

			ALLOC_RELEASE(subdirs);
		}

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