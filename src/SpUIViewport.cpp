#include "SpUIViewport.h"
#include "SpGame.h"
#include "SpProjectManager.h"

namespace NAMESPACE_FRONTEND
{

	void SpUIViewport_onMouseDown(SpIFrameComponent* window, const ImVec2& mousePosition)
	{
		SpUIViewport* uiViewport = (SpUIViewport*)window;

		if (uiViewport->selectedObject() == SP_UINT_MAX)
			return;

		const SpCamera* camera = uiViewport->scene()->camerasManager()->get(uiViewport->activeCameraIndex());
		const SpSize<sp_float> viewport(uiViewport->contentRegionSize().x, uiViewport->contentRegionSize().y);

		Ray ray;
		camera->raycast(Vec2(mousePosition.x, mousePosition.y), viewport, ray);

		Vec3 contacts[2];
		sp_float distanceToFirstContact, distanceToSecondContact;

		AABB aabb = uiViewport->gameObjectManipulator.xAxisAABB();
		if (ray.intersection(aabb, contacts, distanceToFirstContact, distanceToSecondContact))
		{
			uiViewport->_isDragging = true;
			uiViewport->_draggingObject = 1;
			return;
		}
		
		aabb = uiViewport->gameObjectManipulator.yAxisAABB();
		if (ray.intersection(aabb, contacts, distanceToFirstContact, distanceToSecondContact))
		{
			uiViewport->_isDragging = true;
			uiViewport->_draggingObject = 2;
			return;
		}

		aabb = uiViewport->gameObjectManipulator.zAxisAABB();
		if (ray.intersection(aabb, contacts, distanceToFirstContact, distanceToSecondContact))
		{
			uiViewport->_isDragging = true;
			uiViewport->_draggingObject = 3;
			return;
		}
	}

	void SpUIViewport_onMouseReleased(SpIFrameComponent* window, const ImVec2& mousePosition)
	{
		// if (window->wasMouseDownStartedThisWindow())
		
		SpUIViewport* uiViewport = (SpUIViewport*)window;

		uiViewport->_draggingObject = 0;
		uiViewport->_isDragging = false;
	}

	void SpUIViewport_onMouseMove(SpIFrameComponent* window, const ImVec2& previousPosition, const ImVec2& currentPosition)
	{
		SpUIViewport* uiViewport = (SpUIViewport*)window;

		if (!uiViewport->_isDragging)
			return;

		const sp_float windingOrder = SpPhysicSettings::instance()->windingOrder();
		const SpCamera* camera = uiViewport->scene()->camerasManager()->get(uiViewport->activeCameraIndex());
		const SpSize<sp_float> viewport(uiViewport->contentRegionSize().x, uiViewport->contentRegionSize().y);
		SpTransform* transform = uiViewport->scene()->transform(uiViewport->selectedObject());
		
		Ray rayPrev, rayCurr;
		camera->raycast(Vec2(previousPosition.x, previousPosition.y), viewport, rayPrev);
		camera->raycast(Vec2(currentPosition.x, currentPosition.y), viewport, rayCurr);
		
		switch (uiViewport->_draggingObject)
		{
		case 1: // X-AXIS
		{
			const Plane planeXZ(transform->position, Vec3Up);
			
			Vec3 contactPrev, contactCurr;
			rayPrev.intersection(planeXZ, contactPrev);
			rayCurr.intersection(planeXZ, contactCurr);
 
			const sp_float distanceX = contactCurr.x - contactPrev.x;

			transform->position.x += distanceX;
			break;
		}
		case 2: // Y-AXIS
		{
			const Plane planeXY(transform->position, Vec3Front * windingOrder);

			Vec3 contactPrev, contactCurr;
			rayPrev.intersection(planeXY, contactPrev);
			rayCurr.intersection(planeXY, contactCurr);

			const sp_float distanceY = contactCurr.y - contactPrev.y;

			transform->position.y += distanceY;
			break;
		}
		case 3: // Z-AXIS
		{
			const Plane planeXZ(transform->position, Vec3Up);

			Vec3 contactPrev, contactCurr;
			rayPrev.intersection(planeXZ, contactPrev);
			rayCurr.intersection(planeXZ, contactCurr);

			const sp_float distanceZ = contactCurr.z - contactPrev.z;

			transform->position.z += distanceZ;
			break;
		}
		default:
			break;
		}
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