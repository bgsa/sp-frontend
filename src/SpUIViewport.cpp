#include "SpUIViewport.h"
#include "SpGame.h"
#include "SpProjectManager.h"

namespace NAMESPACE_FRONTEND
{

	void SpUIViewport_onMouseDown(SpIFrameComponent* window, const ImVec2& mousePosition)
	{
		std::cout << "DOWN: " << mousePosition.x << " - " << mousePosition.y << std::endl;
	}

	void SpUIViewport_onMouseReleased(SpIFrameComponent* window, const ImVec2& mousePosition)
	{
		if (window->wasMouseDownStartedThisWindow())
			std::cout << "RELEASED: " << mousePosition.x << " - " << mousePosition.y << std::endl;
	}

	void SpUIViewport_onMouseMove(SpIFrameComponent* window, const ImVec2& previousPosition, const ImVec2& currentPosition)
	{
		//std::cout << screenPos.x << " - " << screenPos.y << std::endl;
	}

	void SpUIViewport_onMouseEnter(SpIFrameComponent* window, const ImVec2& mousePosition)
	{
	}
	void SpUIViewport_onMouseLeave(SpIFrameComponent* window, const ImVec2& mousePosition)
	{
	}

	void SpUIViewport::drawManipulator(const Vec3& center)
	{
		SpRenderingAPI* api = SpGameInstance->renderingAPI();
		const sp_uint usageType = api->bufferUsageTypeDynamicDraw();
		const sp_uint typeFloatId = api->typeFloatId();
		const sp_uint typeUintId = api->typeUIntId();
		const sp_uint trianglesId = api->typeTriangleId();
		SpCamera* camera = scene()->camerasManager()->get(scene()->activeCameraIndex());
		const Vec3 cameraPosition = camera->position();

		const sp_size cubeAttributesStride = sizeof(sp_float) * 7;
		const sp_uint cubeIndexesLength = 3 * 54;

		const sp_float distanceCameraToObject = distance(cameraPosition, center);
		const sp_float manipulatorSize = distanceCameraToObject * 0.2f;
		const sp_float thickness = manipulatorSize * 0.05f;
		const sp_float twoThickness = thickness * 2.0f;

		SpColorRGBA xAxisColor = SpColorRGBARed;
		SpColorRGBA yAxisColor = SpColorRGBAGreen;
		SpColorRGBA zAxisColor = SpColorRGBABlue;

		sp_float cubeAttributes[3 * 91] = {
			// x Axis
			center.x, center.y - thickness, center.z + thickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
			center.x + manipulatorSize, center.y - thickness, center.z + thickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
			center.x + manipulatorSize, center.y + thickness, center.z + thickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
			center.x, center.y + thickness, center.z + thickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,

			center.x, center.y - thickness, center.z - thickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
			center.x + manipulatorSize, center.y - thickness, center.z - thickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
			center.x + manipulatorSize, center.y + thickness, center.z - thickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
			center.x, center.y + thickness, center.z - thickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,

			center.x + manipulatorSize, center.y - twoThickness, center.z - twoThickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
			center.x + manipulatorSize, center.y + twoThickness, center.z - twoThickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
			center.x + manipulatorSize, center.y + twoThickness, center.z + twoThickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
			center.x + manipulatorSize, center.y - twoThickness, center.z + twoThickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
			center.x + manipulatorSize + twoThickness, center.y, center.z, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,

			// y Axis
			center.x - thickness, center.y, center.z + thickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
			center.x - thickness, center.y, center.z - thickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
			center.x + thickness, center.y, center.z - thickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
			center.x + thickness, center.y, center.z + thickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,

			center.x - thickness, center.y + manipulatorSize, center.z + thickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
			center.x - thickness, center.y + manipulatorSize, center.z - thickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
			center.x + thickness, center.y + manipulatorSize, center.z - thickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
			center.x + thickness, center.y + manipulatorSize, center.z + thickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,

			center.x - twoThickness, center.y + manipulatorSize, center.z + twoThickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
			center.x - twoThickness, center.y + manipulatorSize, center.z - twoThickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
			center.x + twoThickness, center.y + manipulatorSize, center.z - twoThickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
			center.x + twoThickness, center.y + manipulatorSize, center.z + twoThickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
			center.x, center.y + manipulatorSize + twoThickness, center.z, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,

			// z Axis
			center.x + thickness, center.y - thickness, center.z, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
			center.x - thickness, center.y - thickness, center.z, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
			center.x - thickness, center.y + thickness, center.z, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
			center.x + thickness, center.y + thickness, center.z, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,

			center.x + thickness, center.y - thickness, center.z + manipulatorSize, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
			center.x - thickness, center.y - thickness, center.z + manipulatorSize, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
			center.x - thickness, center.y + thickness, center.z + manipulatorSize, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
			center.x + thickness, center.y + thickness, center.z + manipulatorSize, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,

			center.x + twoThickness, center.y - twoThickness, center.z + manipulatorSize, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
			center.x - twoThickness, center.y - twoThickness, center.z + manipulatorSize, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
			center.x - twoThickness, center.y + twoThickness, center.z + manipulatorSize, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
			center.x + twoThickness, center.y + twoThickness, center.z + manipulatorSize, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
			center.x, center.y, center.z + manipulatorSize + twoThickness, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha
		};

		if (manipulatorShader == nullptr)
		{
			const sp_uint cubeIndexes[cubeIndexesLength] = {
				// X axis
				0, 1, 2, // front face
				2, 3, 0, // front face
				4, 7, 6, // back face
				6, 5, 4, // back face
				0, 3, 7, // left face
				7, 4, 0, // left face
				1, 5, 6, // right face
				6, 2, 1, // right face
				3, 2, 6, // top face
				6, 7, 3, // top face
				1, 0, 4, // bottom face
				4, 5, 1, // bottom face

				// X arrow
				8, 9, 10,
				10, 11, 8,
				9, 8, 12,
				10, 9, 12,
				10, 11, 12,
				8, 11, 12,

				// Y axis
				13, 14, 15, // bottom face
				15, 16, 13, // bottom face
				17, 18, 19, // top face
				19, 20, 17, // top face
				14, 15, 19, // front face
				19, 18, 14, // front face
				13, 16, 20, // back face
				20, 17, 13, // back face
				13, 14, 18, // left face
				18, 17, 13, // left face
				15, 16, 20, // right face
				20, 19, 15, // right face

				// Y arrow
				21, 22, 23,
				23, 24, 21,
				21, 22, 25,
				22, 23, 25,
				23, 24, 25,
				24, 21, 25,

				// Z axis
				26, 27, 28, // bottom face
				28, 29, 26, // bottom face
				30, 31, 32, // top face
				32, 33, 30, // top face
				27, 28, 32, // front face
				32, 31, 27, // front face
				26, 29, 33, // back face
				33, 30, 26, // back face
				26, 27, 31, // left face
				31, 30, 26, // left face
				28, 29, 33, // right face
				33, 32, 28, // right face

				// Z arrow
				34, 35, 36,
				36, 37, 34,
				34, 35, 38,
				35, 36, 38,
				36, 37, 38,
				37, 34, 38
			};

			manipulatorShader = scene()->shaders.find(2)->value();

			vertexesBuffer = api
				->createArrayBuffer()
				->use()
				->updateData(sizeof(cubeAttributes), cubeAttributes, usageType);

			manipulatorShader
				->enableVertexAttribute(0, 3, typeFloatId, false, cubeAttributesStride, 0)
				->enableVertexAttribute(1, 4, typeFloatId, false, cubeAttributesStride, (void*)(sizeof(sp_float) * 3));

			vertexesBuffer->disable();

			const sp_uint usageTypeStatic = api->bufferUsageTypeStaticDraw();
			indexesBuffer = api
				->createElementArrayBuffer()
				->use()
				->updateData(sizeof(cubeIndexes), cubeIndexes, usageTypeStatic);

			indexesBuffer->disable();
		}

		//api->disableDepthTest();
		viewport.framebuffer()->use();
		manipulatorShader
			->enable()
			->setUniform(0, camera->projectionMatrix())
			->setUniform(1, camera->viewMatrix())
			->setUniform(2, cameraPosition);

		vertexesBuffer
			->use()
			->updateData(sizeof(cubeAttributes), cubeAttributes, usageType);

		manipulatorShader
			->enableVertexAttribute(0, 3, typeFloatId, false, cubeAttributesStride, 0)
			->enableVertexAttribute(1, 4, typeFloatId, false, cubeAttributesStride, (void*)(sizeof(sp_float) * 3));

		indexesBuffer->use();

		manipulatorShader->drawElements(trianglesId, cubeIndexesLength, typeUintId, 0);

		manipulatorShader->disable();

		viewport.framebuffer()->disable();

		//api->enableDepthTest();
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