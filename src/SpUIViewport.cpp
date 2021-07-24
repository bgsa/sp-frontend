#include "SpUIViewport.h"
#include "SpGame.h"
#include "SpProjectManager.h"

namespace NAMESPACE_FRONTEND
{

	void SpUIViewport_onMouseDown(SpIFrameComponent* window, const ImVec2& mousePosition)
	{
		std::cout << "DOWN: " << mousePosition.x << " - " << mousePosition.y << std::endl;

		SpUIViewport* uiViewport = (SpUIViewport*)window;

		if (uiViewport->selectedObject() == SP_UINT_MAX)
			return;

		SpCamera* camera = uiViewport->scene()->camerasManager()->get(uiViewport->activeCameraIndex());

		Vec2 screenCenter(uiViewport->contentRegionSize().x * 0.5f, uiViewport->contentRegionSize().y * 0.5f);

		const sp_float fov = degree(camera->fieldOfView());
		sp_float angleIncrementX = (fov / uiViewport->contentRegionSize().x) * 0.5f;
		sp_float angleIncrementY = (fov / uiViewport->contentRegionSize().y) * 0.5f;
		
		const sp_float angleX = radians((mousePosition.x - screenCenter.x) * angleIncrementX);
		const sp_float angleY = radians((mousePosition.y - screenCenter.y) * angleIncrementY);

		const Quat xAxis = Quat::createRotate(-angleX, camera->up());
		const Quat yAxis = Quat::createRotate(-angleY, camera->right());

		Vec3 ray1 = camera->direction();
		Vec3 ray2, ray3;
		rotate(xAxis, ray1, ray2);
		rotate(yAxis, ray2, ray3);
		normalize(ray3);

		Ray r = Ray(camera->position(), ray3);
		AABB aabb = uiViewport->gameObjectManipulator.xAxisAABB();
		Vec3 contacts[2];
		sp_float distanceToFirstContact, distanceToSecondContact;

		if (r.intersection(aabb, contacts, distanceToFirstContact, distanceToSecondContact))
		{
			int a = 1;
		}
	}

	void SpUIViewport_onMouseReleased(SpIFrameComponent* window, const ImVec2& mousePosition)
	{
		if (window->wasMouseDownStartedThisWindow())
			std::cout << "RELEASED: " << mousePosition.x << " - " << mousePosition.y << std::endl;
	}

	void SpUIViewport_onMouseMove(SpIFrameComponent* window, const ImVec2& previousPosition, const ImVec2& currentPosition)
	{
		std::cout << currentPosition.x << " - " << currentPosition.y << std::endl;
	}

	void SpUIViewport_onMouseEnter(SpIFrameComponent* window, const ImVec2& mousePosition)
	{
	}
	void SpUIViewport_onMouseLeave(SpIFrameComponent* window, const ImVec2& mousePosition)
	{
	}

	void SpUIViewport::drawNavigation()
	{
		SpRenderingAPI* api = SpGameInstance->renderingAPI();
		const sp_uint usageType = api->bufferUsageTypeDynamicDraw();
		const sp_uint typeFloatId = api->typeFloatId();
		const sp_uint linesId = api->typeLinesId();
		const sp_int uintId = api->typeUIntId();
		const sp_float lineSize = 0.1f;
		SpCamera* camera = scene()->camerasManager()->get(scene()->activeCameraIndex());
		Vec3 lineCenter = camera->position();
		Vec3 target = camera->target();

		const sp_float aspectRatio = 1.0f - viewport.aspectRatio();

		Vec3 directionToZ;
		normalize(target - camera->position(), directionToZ);
		lineCenter += (directionToZ * 2.0f);
		lineCenter += (camera->right() * -0.99f) + (camera->right() * aspectRatio);
		lineCenter += (camera->up() * -0.9f);

		sp_float bufferData[42] = {
			lineCenter.x, lineCenter.y, lineCenter.z, 1.0f, 0.0f, 0.0f, 1.0f,
			lineCenter.x + lineSize, lineCenter.y, lineCenter.z, 1.0f, 0.0f, 0.0f, 1.0f,

			lineCenter.x, lineCenter.y, lineCenter.z, 0.0f, 1.0f, 0.0f, 1.0f,
			lineCenter.x, lineCenter.y + lineSize, lineCenter.z, 0.0f, 1.0f, 0.0f, 1.0f,

			lineCenter.x, lineCenter.y, lineCenter.z, 0.0f, 0.0f, 1.0f, 1.0f,
			lineCenter.x, lineCenter.y, lineCenter.z + lineSize, 0.0f, 0.0f, 1.0f, 1.0f
		};
		const sp_size bufferStride = sizeof(sp_float) * 7;

		if (linesShader == nullptr)
		{
			linesShader = scene()->shaders.find(1)->value();

			vertexesBuffer = api
				->createArrayBuffer()
				->use()
				->updateData(sizeof(bufferData), bufferData, usageType);

			linesShader->enableVertexAttribute(0, 3, typeFloatId, false, bufferStride, 0);
			linesShader->enableVertexAttribute(1, 4, typeFloatId, false, bufferStride, (void*)(sizeof(sp_float) * 3));

			vertexesBuffer->disable();
		}

		api->disableDepthTest();
		viewport.framebuffer()->use();
		linesShader
			->enable()
			->setUniform(0, camera->projectionMatrix())
			->setUniform(1, camera->viewMatrix());

		vertexesBuffer
			->use()
			->updateData(sizeof(bufferData), bufferData, usageType);

		linesShader->enableVertexAttribute(0, 3, typeFloatId, false, bufferStride, 0);
		linesShader->enableVertexAttribute(1, 4, typeFloatId, false, bufferStride, (void*)(sizeof(sp_float) * 3));

		linesShader->drawArray(linesId, 0, 6);

		linesShader->disable();
		viewport.framebuffer()->disable();
		api->enableDepthTest();
	}

	void SpUIViewport::render()
	{
		if (!isVisible())
			return;

		sp_char viewportTitle[SP_SCENE_NAME_MAX_LENGTH + 30];
		getTitle(viewportTitle, SP_SCENE_NAME_MAX_LENGTH + 30);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2Zeros);
		begin(viewportTitle, NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		if (viewport.scene != nullptr)
			drawViewportContent();

		if (ImGui::BeginPopupContextWindow())
		{
			SpProject* currentProject = SpProjectManagerInstance->current();

			if (ImGui::BeginMenu("Scenes", currentProject != nullptr))
			{
				SpVector<SpScene*>* scenes = currentProject->game()->scenes();

				for (SpVectorItem<SpScene*>* item = scenes->begin(); item != nullptr; item = item->next())
				{
					const sp_bool sceneSelected = item->value() == viewport.scene;

					if (ImGui::MenuItem(item->value()->name(), nullptr, sceneSelected, !sceneSelected))
						this->scene(item->value());
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Cameras", viewport.scene != nullptr))
			{
				SpCameraManager* cameraManager = viewport.scene->camerasManager();

				for (sp_uint i = 0; i < cameraManager->length(); i++)
				{
					const sp_bool cameraSelected = activeCameraIndex() == i;

					if (ImGui::MenuItem(cameraManager->name(i), nullptr, cameraSelected, !cameraSelected))
						viewport.activeCameraIndex = i;
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Background Color", nullptr, false, true))
				colorPicker.show();

			ImGui::EndPopup();
		}

		colorPicker.render();

		end();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

}