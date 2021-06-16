#ifndef SP_TOOLBAR_FRAME_HEADER
#define SP_TOOLBAR_FRAME_HEADER

#include "SpFrame.h"
#include "SpUIIcons.h"
#include "SpUIImageButton.h"

namespace NAMESPACE_FRONTEND
{
	class SpToolbarFrame
		: public SpFrame
	{
	private:
		SpUIImageButton buttonPlay;
		SpUIImageButton buttonStop;
		SpUIImageButton buttonNextFrame;

		sp_bool buttonNextFrameWasClicked;

	public:

		API_INTERFACE void init(SpWindow* window) override
		{
			SpFrame::init(window);
			show();

			buttonNextFrameWasClicked = false;

			buttonPlay.init();
			buttonStop.init();
			buttonNextFrame.init();
		}

		API_INTERFACE void preRender() override
		{
		}

		API_INTERFACE void render() override
		{

		}

		API_INTERFACE void renderGUI() override
		{
			if (!isVisible())
				return;

			SpWindowState* state = window()->state();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 3.0f));

			if (ImGui::Begin("SpToolbar", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
			{
				ImGui::SetWindowSize(ImVec2((sp_float)state->width, 40.0f));
				ImGui::SetWindowPos(ImVec2(0.0f, 24.0f));

				loadState();
				
				if (SpPhysicSettings::instance()->isSimulationEnabled())
				{
					ImVec2 uv1, uv2;
					SpUIIconsInstance->getUVCoordinatesStop(uv1, uv2);

					buttonStop.render((void*)(intptr_t)SpUIIconsInstance->icons1->id(), ImVec2(32.0f, 32.0f), uv1, uv2);

					if (buttonStop.isClicked())
						SpPhysicSettings::instance()->disableSimulation();
				}
				else 
				{
					ImVec2 uv1, uv2;
					SpUIIconsInstance->getUVCoordinatesPlay(uv1, uv2);

					buttonPlay.render((void*)(intptr_t)SpUIIconsInstance->icons1->id(), ImVec2(32.0f, 32.0f), uv1, uv2);

					if (buttonPlay.isClicked())
						SpPhysicSettings::instance()->enableSimulation();
				}

				ImVec2 uv1, uv2;
				SpUIIconsInstance->getUVCoordinatesNextFrame(uv1, uv2);

				ImGui::SameLine(0.0f, 0.0f);

				buttonNextFrame.render((void*)(intptr_t)SpUIIconsInstance->icons1->id(), ImVec2(32.0f, 32.0f), uv1, uv2);

				if (buttonNextFrame.isClicked())
				{
					buttonNextFrameWasClicked = true;
					SpPhysicSettings::instance()->enableSimulation();
				}
				else
					if (buttonNextFrameWasClicked)
					{
						buttonNextFrameWasClicked = false;
						SpPhysicSettings::instance()->disableSimulation();
					}

				ImGui::End();
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "Sp Toolbar";
		}

	};
}

#endif // SP_TOOLBAR_FRAME_HEADER