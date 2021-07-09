#ifndef SP_UI_VIEWPORT_HEADER
#define SP_UI_VIEWPORT_HEADER

#include "SpIFrameComponent.h"
#include "SpViewport.h"
#include "SpUIColorPicker.h"

namespace NAMESPACE_FRONTEND
{
	class SpUIViewport
		: public SpIFrameComponent
	{
	private:
		SpViewport viewport;
		SpUIColorPicker colorPicker;

		SpShader* linesShader;
		SpGpuBuffer* linesBuffer;

		inline void drawNavigation()
		{
			SpRenderingAPI* api = SpGameInstance->renderingAPI();
			const sp_uint usageType = api->bufferUsageTypeDynamicDraw();
			const sp_uint typeFloatId = api->typeFloatId();
			const sp_uint linesId = api->typeLinesId();
			const sp_int uintId = api->typeUIntId();
			const sp_float lineSize = 0.1f;
			SpCamera* camera = scene()->cameras()->get(scene()->activeCameraIndex());
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

				linesBuffer = api
					->createArrayBuffer()
					->use()
					->updateData(sizeof(bufferData), bufferData, usageType);

				linesShader->enableVertexAttribute(0, 3, typeFloatId, false, bufferStride, 0);
				linesShader->enableVertexAttribute(1, 4, typeFloatId, false, bufferStride, (void*)(sizeof(sp_float) * 3));

				linesBuffer->disable();
			}
			
			api->disableDepthTest();
			viewport.framebuffer()->use();
			linesShader
				->enable()
				->setUniform(0, camera->getProjectionMatrix())
				->setUniform(1, camera->getViewMatrix());

			linesBuffer
				->use()
				->updateData(sizeof(bufferData), bufferData, usageType);

			linesShader->enableVertexAttribute(0, 3, typeFloatId, false, bufferStride, 0);
			linesShader->enableVertexAttribute(1, 4, typeFloatId, false, bufferStride, (void*)(sizeof(sp_float) * 3));

			linesShader->drawArray(linesId, 0, 6);

			linesShader->disable();
			viewport.framebuffer()->disable();
			api->enableDepthTest();
		}
		
		inline void drawViewportContent()
		{
			viewport.render();

			drawNavigation();

			SpSize<sp_int> size = viewport.size();
			ImGui::Image((void*)(intptr_t)viewport.framebuffer()->colorTexture(), ImVec2((sp_float)size.width, (sp_float)size.height), ImVec2(0, 1), ImVec2(1, 0));
		}

	public:

		API_INTERFACE inline SpUIViewport()
		{
			linesShader = nullptr;
		}

		/// <summary>
		/// Initialize the UI Viewport
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void init()
		{
			_minWidth = _minHeight = 300.0f;
			SpIFrameComponent::resize(400, 300);

			colorPicker.init();
			colorPicker.color(viewport.backgroundColor());
		}

		API_INTERFACE inline void resize(const sp_int width, const sp_int height) noexcept override
		{
			SpIFrameComponent::resize(width, height);

			SpRect<sp_int> area(0, 0, width, height);
			viewport.resize(area);
		}

		/// <summary>
		/// Get the scene of this viewport (can be null)
		/// </summary>
		/// <param name="scene">Scene</param>
		/// <returns>Scene</returns>
		API_INTERFACE inline SpScene* scene() const
		{
			return viewport.scene;
		}

		/// <summary>
		/// Set the scene for this viewport
		/// </summary>
		/// <param name="scene">Scene</param>
		/// <returns>void</returns>
		API_INTERFACE inline void scene(SpScene* scene)
		{
			if (!scene->isLoaded())
				scene->load();

			viewport.scene = scene;
			viewport.activeCameraIndex = scene->activeCameraIndex();
		}

		/// <summary>
		/// Get the active camera of this scene
		/// </summary>
		/// <returns>Camera Scene Index</returns>
		API_INTERFACE inline sp_uint activeCameraIndex() const
		{
			return viewport.activeCameraIndex;
		}

		/// <summary>
		/// Set the active camera of this scene
		/// </summary>
		/// <param name="title"></param>
		/// <param name="titleMaxLength"></param>
		/// <returns></returns>
		API_INTERFACE inline void activeCameraIndex(const sp_uint cameraIndex)
		{
			viewport.activeCameraIndex = cameraIndex;
		}

		/// <summary>
		/// Get the title of viewport
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void getTitle(sp_char* title, const sp_uint titleMaxLength) const
		{
			sp_assert(titleMaxLength >= 30 + SP_SCENE_NAME_MAX_LENGTH, "InvalidArgumentException");

			if (viewport.scene != nullptr)
			{
				sp_uint sceneNameLength = std::strlen(viewport.scene->name());

				std::memcpy(title, viewport.scene->name(), sceneNameLength);
				std::memcpy(&title[sceneNameLength], " - ", 3);

				const sp_size temp = (sp_size)this;
				sp_uint idLength;
				convert(temp, &title[sceneNameLength + 3], idLength);

				title[sceneNameLength + idLength + 3] = END_OF_STRING;
			}
			else
			{
				std::memcpy(title, "(no scene) - ", sizeof(sp_char) * 13);

				const sp_size temp = (sp_size)this;
				sp_uint length;
				convert(temp, &title[13], length);

				title[13 + length] = END_OF_STRING;
			}
		}

		/// <summary>
		/// Render this viewport and the scene
		/// </summary>
		/// <returns></returns>
		API_INTERFACE void render() override
		{
			if (!isVisible())
				return;

			sp_char viewportTitle[SP_SCENE_NAME_MAX_LENGTH + 30];
			getTitle(viewportTitle, SP_SCENE_NAME_MAX_LENGTH + 30);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2Zeros);
			begin(viewportTitle, NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

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
					sp_size* cameraIndexes = ALLOC_ARRAY(sp_size, viewport.scene->gameObjectsLength());
					sp_size cameraIndexesLength;

					viewport.scene->gameObjectByType(SP_GAME_OBJECT_TYPE_CAMERA, cameraIndexes, cameraIndexesLength);

					for (sp_uint i = 0; i < cameraIndexesLength; i++)
					{
						const sp_size cameraIndex = cameraIndexes[i];
						const sp_bool cameraSelected = activeCameraIndex() == cameraIndex;

						if (ImGui::MenuItem(viewport.scene->gameObject(cameraIndex)->name(), nullptr, cameraSelected, !cameraSelected))
							viewport.activeCameraIndex = cameraIndex;
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

	};
}

#endif // SP_UI_VIEWPORT_HEADER