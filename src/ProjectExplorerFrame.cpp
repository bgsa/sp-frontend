#include "ProjectExplorerFrame.h"
#include "SpUIManager.h"
#include "SpGame.h"

namespace NAMESPACE_FRONTEND
{

	void ProjectExplorerFrame::renderGameObjectsNode(SpScene* scene)
	{
		const sp_bool gameObjectNodeOpened = ImGui::TreeNode("Game Objects");

		renderGameObjectsContextMenu(scene);

		if (gameObjectNodeOpened)
		{
			for (sp_size i = 0; i < scene->gameObjectsLength(); i++)
			{
				const SpGameObject* gameObject = scene->gameObject(i);
				sp_char* name = gameObject->name();

				if (ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_Leaf))
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

	void ProjectExplorerFrame::renderCameraNode(SpScene* scene)
	{
		if (ImGui::TreeNode("Cameras"))
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
				sp_char* name = scene->meshManager()->name(i);

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

				// check light properties
				if (SpUIManagerInstance->propertiesFrame.propertiesComponent() == &SpUIManagerInstance->propertiesFrameLighting
					&& SpUIManagerInstance->propertiesFrame.selectedIndex() == index)
				{
					SpUIManagerInstance->propertiesFrame.deselect();
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
			if (ImGui::TreeNode("Scenes"))
			{
				renderScenesContextMenu();

				SpVector<SpScene*>* scenes = project->game()->scenes();
				const sp_bool canDeleteScene = scenes->length() > ONE_UINT;

				for (SpVectorItem<SpScene*>* sceneItem = scenes->begin(); sceneItem != nullptr; sceneItem = sceneItem->next())
					renderSceneNode(sceneItem, canDeleteScene);

				ImGui::TreePop(); // close Scenes Node
			}

			if (ImGui::TreeNode("Resources"))
			{
				ImGui::TreePop();
			}
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