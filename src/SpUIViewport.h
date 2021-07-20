#ifndef SP_UI_VIEWPORT_HEADER
#define SP_UI_VIEWPORT_HEADER

#include "SpIFrameComponent.h"
#include "SpViewport.h"
#include "SpUIColorPicker.h"

namespace NAMESPACE_FRONTEND
{

	void SpUIViewport_onMouseEnter(SpIFrameComponent* window, const ImVec2& mousePosition);
	void SpUIViewport_onMouseLeave(SpIFrameComponent* window, const ImVec2& mousePosition);
	void SpUIViewport_onMouseMove(SpIFrameComponent* window, const ImVec2& previousPosition, const ImVec2& currentPosition);
	void SpUIViewport_onMouseDown(SpIFrameComponent* window, const ImVec2& mousePosition);
	void SpUIViewport_onMouseReleased(SpIFrameComponent* window, const ImVec2& mousePosition);

	class SpUIViewport
		: public SpIFrameComponent
	{
	private:
		SpViewport viewport;
		SpUIColorPicker colorPicker;

		SpShader* linesShader;
		SpShader* manipulatorShader;
		SpGpuBuffer* vertexesBuffer;
		SpGpuBuffer* indexesBuffer;

		sp_uint _selectedObjectIndex;

		void drawManipulator(const Vec3& center);

		void drawNavigation();
		
		inline void drawViewportContent()
		{
			viewport.render();

			drawNavigation();

			if (_selectedObjectIndex != SP_UINT_MAX)
			{
				const Vec3 position = scene()->transform(_selectedObjectIndex)->position;
				drawManipulator(position);
			}

			SpSize<sp_int> size = viewport.size();
			ImGui::Image((void*)(intptr_t)viewport.framebuffer()->colorTexture(), ImVec2((sp_float)size.width, (sp_float)size.height), ImVec2(0, 1), ImVec2(1, 0));
		}

	public:

		API_INTERFACE inline SpUIViewport()
		{
			linesShader = nullptr;
			manipulatorShader = nullptr;

			_selectedObjectIndex = SP_UINT_MAX;
		}

		/// <summary>
		/// Initialize the UI Viewport
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void init()
		{
			_minWidth = _minHeight = 300.0f;
			SpIFrameComponent::resize(400, 300);

			this->onMouseEnter = &SpUIViewport_onMouseEnter;
			this->onMouseLeave = &SpUIViewport_onMouseLeave;
			this->onMouseMove = &SpUIViewport_onMouseMove;
			this->onMouseLeftButtonDown = &SpUIViewport_onMouseDown;
			this->onMouseLeftButtonReleased = &SpUIViewport_onMouseReleased;

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

		API_INTERFACE inline void selectObject(const sp_uint index)
		{
			_selectedObjectIndex = index;
		}

		API_INTERFACE inline void deselectObject()
		{
			_selectedObjectIndex = SP_UINT_MAX;
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
		API_INTERFACE void render() override;

	};
}

#endif // SP_UI_VIEWPORT_HEADER