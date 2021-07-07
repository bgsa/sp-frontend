#include "ProjectExplorerFrame.h"
#include "SpUIManager.h"
#include "SpGame.h"

namespace NAMESPACE_FRONTEND
{

	void ProjectExplorerFrame::renderGameObjectsContextMenu(SpScene* scene)
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::BeginMenu("Add", true))
			{
				if (ImGui::MenuItem("Camera", NULL, false, true))
				{

				}

				if (ImGui::MenuItem("Cube", NULL, false, true))
				{

				}

				if (ImGui::MenuItem("Plane", NULL, false, true))
				{
					sp_char planeName[100];
					std::memcpy(planeName, "Plane ", sizeof(sp_char) * 6);
					sp_size len;
					convert(SpPhysicSettings::instance()->frameId(), &planeName[6], len);
					planeName[6 + len] = END_OF_STRING;

					SpGameObject* gameObject = scene->addGameObject(SP_GAME_OBJECT_TYPE_PLANE, planeName);
					SpRenderableObject* renderableObject = scene->renderableObjectManager()->get(gameObject->managerIndex());
					renderableObject->type(SP_RENDERABLE_OBJECT_TYPE_PLANE);
					renderableObject->gameObjectIndex = gameObject->index();
					renderableObject->meshDataIndex = SP_MESH_INDEX_PLANE;
					renderableObject->shaderIndex = 0;

					SpMeshData* meshData = scene->meshManager()->get(0);
					const sp_size arrayBufferSize = sizeof(SpMeshAttribute) * meshData->attributesLength;
					const sp_int staticDraw = SpGameInstance->renderingAPI()->bufferUsageTypeStaticDraw();

					SpGpuBuffer* gpuVertexBuffer 
						= SpGameInstance->renderingAPI()->createArrayBuffer()
						->use()
						->updateData(arrayBufferSize, (sp_float*) meshData->attributes, staticDraw);

					SpGpuBuffer* gpuIndexBuffer
						= SpGameInstance->renderingAPI()->createElementArrayBuffer()
						->use()
						->updateData(meshData->facesLength * 3 * sizeof(sp_size), meshData->faceIndexes, staticDraw);

					renderableObject->buffers.add(gpuVertexBuffer);
					renderableObject->buffers.add(gpuIndexBuffer);
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
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

		if (ImGui::TreeNode(scene->name()))
		{
			renderSceneContextMenu(sceneItem, canDeleteScene);

			if (ImGui::TreeNode("Game Objects"))
			{
				renderGameObjectsContextMenu(scene);

				for (sp_size i = 0; i < scene->gameObjectsLength(); i++)
				{
					const SpGameObject* gameObject = scene->gameObject(i);
					sp_char* name = gameObject->name();

					if (ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_Leaf))
					{
						if (ImGui::IsItemClicked())
							SpUIManagerInstance->propertiesFrame.selectedGameObject(scene, i);

						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}

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

			if (ImGui::TreeNode("Scripts"))
			{
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
	}

}