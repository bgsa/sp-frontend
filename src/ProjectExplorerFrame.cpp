#include "ProjectExplorerFrame.h"
#include "SpUIManager.h"
#include "SpGame.h"

namespace NAMESPACE_FRONTEND
{

	void ProjectExplorerFrame::renderGameObjectContextMenu(SpScene* scene, const sp_uint index)
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete", NULL, false, true))
			{
				scene->removeGameObject(index);

				// check game object selected
				if (SpUIManagerInstance->propertiesFrame.propertiesComponent() == &SpUIManagerInstance->propertiesFrameGameObject
					&& SpUIManagerInstance->propertiesFrame.selectedIndex() == index)
				{
					SpUIManagerInstance->propertiesFrame.deselect();

					for (SpVectorItem<SpUIViewport*>* item = SpUIManagerInstance->viewports.begin(); item != nullptr; item = item->next())
						item->value()->deselectObject();
				}
			}

			ImGui::EndPopup();
		}
	}

	void ProjectExplorerFrame::renderGameObjectsNode(SpScene* scene)
	{
		const sp_bool gameObjectNodeOpened = ImGui::TreeNode("Game Objects");

		renderGameObjectsContextMenu(scene);

		if (gameObjectNodeOpened)
		{
			for (sp_size i = 0; i < scene->gameObjectsLength(); i++)
			{
				const SpGameObject* gameObject = scene->gameObject(i);
				const sp_char* name = scene->gameObjectManager()->name(i);

				const sp_bool gameObjectNodeClicked = ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_Leaf);

				renderGameObjectContextMenu(scene, i);

				if (gameObjectNodeClicked)
				{
					if (ImGui::IsItemClicked())
					{
						SpUIManagerInstance->propertiesFrame.select(scene, &SpUIManagerInstance->propertiesFrameGameObject, i);

						for (SpVectorItem<SpUIViewport*>* item = SpUIManagerInstance->viewports.begin(); item != nullptr; item = item->next())
							item->value()->selectObject(i);
					}

					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}

	void ProjectExplorerFrame::renderMaterialContextMenu(SpScene* scene)
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Add", NULL, false, true))
			{
			}

			ImGui::EndPopup();
		}
	}

	void ProjectExplorerFrame::renderMaterialsNode(SpScene* scene)
	{
		const sp_bool materialsNodeOpened = ImGui::TreeNode("Materials");

		renderMaterialContextMenu(scene);

		if (materialsNodeOpened)
		{

			ImGui::TreePop();
		}
	}
	
	void ProjectExplorerFrame::renderCameraContextMenu(SpScene* scene)
	{
		if (ImGui::BeginPopupContextItem())
		{
			SpCameraManager* cameraManager = scene->camerasManager();

			if (ImGui::MenuItem("Add", NULL, false, true))
			{
				sp_char name[SP_CAMERA_NAME_MAX_LENGTH];
				std::memcpy(name, "Camera ", 7);

				for (sp_uint i = 0; i < SP_UINT_MAX; i++)
				{
					sp_char indexAsString[10];
					sp_size indexLength;
					convert((i + 1), indexAsString, indexLength);

					std::memcpy(&name[7], indexAsString, indexLength);
					sp_size nameLength = 7 + indexLength;
					name[nameLength] = END_OF_STRING;

					sp_uint cameraIndex = cameraManager->find(name);

					if (cameraIndex == SP_UINT_MAX)
					{
						cameraIndex = scene->camerasManager()->add();
						cameraManager->name(cameraIndex, name, nameLength);
						break;
					}
				}
			}

			ImGui::EndPopup();
		}
	}

	void ProjectExplorerFrame::renderCameraNode(SpScene* scene)
	{
		const sp_bool cameraNodeOpened = ImGui::TreeNode("Cameras");

		renderCameraContextMenu(scene);

		if (cameraNodeOpened)
		{
			SpCameraManager* cameraManager = scene->camerasManager();

			for (sp_size i = 0; i < cameraManager->length(); i++)
			{
				sp_char* name = cameraManager->name(i);

				if (ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_Leaf))
				{
					if (ImGui::IsItemClicked())
						SpUIManagerInstance->propertiesFrame.select(scene, &SpUIManagerInstance->propertiesFrameCamera, i);

					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}

	void ProjectExplorerFrame::renderMeshesNode(SpScene* scene)
	{
		if (ImGui::TreeNode("Meshes"))
		{
			for (sp_size i = 0; i < scene->meshManager()->length(); i++)
			{
				const sp_char* name = scene->meshManager()->name(i);

				if (ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_Leaf))
				{
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}

	void ProjectExplorerFrame::renderLightingNode(SpScene* scene)
	{
		const sp_bool lightingNodeOpened = ImGui::TreeNode("Lighting");
		renderLightingContextMenu(scene);

		if (lightingNodeOpened)
		{
			SpLightingManager* lightManager = scene->lightingManager();

			for (sp_size i = 0; i < lightManager->length(); i++)
			{
				sp_char* name = lightManager->name(i);

				const sp_bool lightClicked = ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_Leaf);

				renderLightContextMenu(scene, i);

				if (lightClicked)
				{
					if (ImGui::IsItemClicked())
						SpUIManagerInstance->propertiesFrame.select(scene, &SpUIManagerInstance->propertiesFrameLighting, i);

					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}

	void ProjectExplorerFrame::renderLightingContextMenu(SpScene* scene)
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Add", NULL, false, true))
			{
				const sp_size frameId = SpPhysicSettings::instance()->frameId();
				sp_char frameIdStr[20];
				sp_uint frameIdStrLength;
				std::memcpy(frameIdStr, "Light ", 6);
				convert(frameId, &frameIdStr[6], frameIdStrLength);
				frameIdStr[6 + frameIdStrLength] = END_OF_STRING;

				const sp_uint index = scene->lightingManager()->add();
				scene->lightingManager()->name(index, frameIdStr, 6 + frameIdStrLength);
			}

			ImGui::EndPopup();
		}
	}

	void ProjectExplorerFrame::renderLightContextMenu(SpScene* scene, const sp_uint index)
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete", NULL, false, true))
			{
				scene->lightingManager()->remove(index);

				// check light properties selected
				if (SpUIManagerInstance->propertiesFrame.propertiesComponent() == &SpUIManagerInstance->propertiesFrameLighting
					&& SpUIManagerInstance->propertiesFrame.selectedIndex() == index)
				{
					SpUIManagerInstance->propertiesFrame.deselect();

					for (SpVectorItem<SpUIViewport*>* item = SpUIManagerInstance->viewports.begin(); item != nullptr; item = item->next())
						item->value()->deselectObject();
				}
			}

			ImGui::EndPopup();
		}
	}

	void ProjectExplorerFrame::renderGameObjectsContextMenu(SpScene* scene)
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::BeginMenu("Add", true))
			{
				for (SpVectorItem<SpPair<sp_uint, SpGameObjectType*>>* item = scene->gameObjectsTypeList.begin(); item != nullptr; item = item->next())
				{
					SpGameObjectType* gameObjectType = item->value().value;
					sp_char* name = gameObjectType->name();

					if (ImGui::MenuItem(name, NULL, false, true))
					{
						gameObjectType->factory()->create(scene);
					}
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}
	}

	void ProjectExplorerFrame::renderShadersNode(SpScene* scene)
	{
		if (ImGui::TreeNode("Shaders"))
		{
			for (SpVectorItem<SpShader*>* item = scene->shaders.begin(); item != nullptr; item = item->next())
			{
				const sp_char* name = item->value()->name();

				if (ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_Leaf))
				{
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}

	void ProjectExplorerFrame::renderAssetFolderNode(const sp_char* folder, const sp_size folderLength)
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

			sp_char fullname[SP_DIRECTORY_MAX_LENGTH];
			const sp_size fullnameLength = folderLength + nameLength + 1;
			directoryAddPath(folder, folderLength, name, nameLength, fullname);

			if (ImGui::IsItemClicked())
			{
				SpUIManagerInstance->propertiesFrameFolder.setName(fullname, fullnameLength);
				SpUIManagerInstance->propertiesFrame.select(nullptr, &SpUIManagerInstance->propertiesFrameFolder, name);
			}

			renderAssetFolderNodeContextMenu(fullname, fullnameLength);

			if (folderNodeOpened)
			{

				renderAssetFolderNode(fullname, fullnameLength);
				renderFilesNode(fullname, fullnameLength);

				ImGui::TreePop();
			}
		}

		ALLOC_RELEASE(subdirs);
	}

	void ProjectExplorerFrame::renderAssetsNode()
	{
		if (SpProjectManagerInstance->current() == nullptr)
			return;

		const sp_bool assetsNodeOpened = ImGui::TreeNode("Assets");

		sp_char folder[SP_DIRECTORY_MAX_LENGTH];
		sp_size folderLength = std::strlen(SpProjectManagerInstance->current()->folder());
		std::memcpy(folder, SpProjectManagerInstance->current()->folder(), folderLength);
		folder[folderLength] = END_OF_STRING;
		directoryAddPath(folder, folderLength, "Assets", 6, folder);
		folderLength += 7;

		renderAssetFolderNodeContextMenu(folder, folderLength);

		if (assetsNodeOpened)
		{
			renderAssetFolderNode(folder, folderLength);
			renderFilesNode(folder, folderLength);

			ImGui::TreePop();
		}
	}

	void ProjectExplorerFrame::renderGUI()
	{
		if (!isVisible())
			return;

		const sp_float headerHeight = 64.0f;
		const sp_float footerHeight = 30.0f;
		const sp_float windowHeight = (sp_float)SpUIManagerInstance->window->state()->availableRegion.height;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 3.0f));
		ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);
		ImGui::Begin("Project Explorer", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

		loadState();

		ImGui::SetWindowSize(ImVec2(ImGui::GetItemRectSize().x, windowHeight - headerHeight - footerHeight));
		ImGui::SetWindowPos(ImVec2(0.0f, headerHeight));

		SpProject* project = SpProjectManagerInstance->current();
		if (project != nullptr)
		{
			const sp_bool scenesNodeOpened = ImGui::TreeNode("Scenes");

			renderScenesContextMenu();

			if (scenesNodeOpened)
			{
				SpVector<SpScene*>* scenes = project->game()->scenes();
				const sp_bool canDeleteScene = scenes->length() > ONE_UINT;

				for (SpVectorItem<SpScene*>* sceneItem = scenes->begin(); sceneItem != nullptr; sceneItem = sceneItem->next())
					renderSceneNode(sceneItem, canDeleteScene);

				ImGui::TreePop(); // close Scenes Node
			}

			renderAssetsNode();
		}

		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

	void ProjectExplorerFrame::renderSceneNode(SpVectorItem<SpScene*>* sceneItem, const sp_bool canDeleteScene)
	{
		SpScene* scene = sceneItem->value();

		const sp_bool sceneNodeOpened = ImGui::TreeNode(scene->name());

		renderSceneContextMenu(sceneItem, canDeleteScene);

		if (sceneNodeOpened)
		{
			renderGameObjectsNode(scene);

			renderMaterialsNode(scene);

			renderCameraNode(scene);

			renderLightingNode(scene);

			renderMeshesNode(scene);

			renderShadersNode(scene);

			if (ImGui::TreeNode("Scripts"))
			{
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
	}

}