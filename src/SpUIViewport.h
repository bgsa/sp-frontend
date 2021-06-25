#ifndef SP_UI_VIEWPORT_HEADER
#define SP_UI_VIEWPORT_HEADER

#include "SpIFrameComponent.h"
#include "SpScene.h"
#include "OpenGLFramebuffer.h"

namespace NAMESPACE_FRONTEND
{
	class SpUIViewport
		: public SpIFrameComponent
	{
	private:
		SpScene* _scene;
		OpenGLFramebuffer* framebuffer;
		
	public:

		API_INTERFACE inline void init()
		{
			_minWidth = _minHeight = 300.0f;
			resize(400, 300);
		}

		API_INTERFACE inline SpScene* scene() const
		{
			return _scene;
		}
		API_INTERFACE inline void scene(SpScene* scene)
		{
			this->_scene = scene;
		}

		API_INTERFACE inline void getTitle(sp_char* title, const sp_uint titleMaxLength) const
		{
			sp_assert(titleMaxLength >= 30 + SP_SCENE_NAME_MAX_LENGTH, "InvalidArgumentException");

			if (_scene != nullptr)
			{
				sp_uint sceneNameLength = std::strlen(_scene->name());

				std::memcpy(title, _scene->name(), sceneNameLength);
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

		API_INTERFACE void render() override
		{
			if (!isVisible())
				return;

			sp_char viewportTitle[SP_SCENE_NAME_MAX_LENGTH + 30];
			getTitle(viewportTitle, SP_SCENE_NAME_MAX_LENGTH + 30);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::Begin(viewportTitle, NULL, ImGuiWindowFlags_NoCollapse);

			ensureMinSize();

			// renderer.render(_scene, framebuffer, cameraIndex)

			if (ImGui::BeginPopupContextWindow())
			{
				SpProject* currentProject = SpProjectManagerInstance->current();

				if (ImGui::BeginMenu("Scenes", currentProject != nullptr))
				{
					SpVector<SpScene*>* scenes = currentProject->game()->scenes();

					for (SpVectorItem<SpScene*>* item = scenes->begin(); item != nullptr; item = item->next())
					{
						const sp_bool sceneSelected = item->value() == _scene;

						if (ImGui::MenuItem(item->value()->name(), nullptr, sceneSelected, !sceneSelected))
						{
							_scene = item->value();
						}
					}

					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}


			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
		}

	};
}

#endif // SP_UI_VIEWPORT_HEADER