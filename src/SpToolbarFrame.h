#ifndef SP_TOOLBAR_FRAME_HEADER
#define SP_TOOLBAR_FRAME_HEADER

#include "SpFrame.h"

namespace NAMESPACE_FRONTEND
{
	class SpToolbarFrame
		: public SpFrame
	{
	public:

		API_INTERFACE void init(SpWindow* window) override
		{
			SpFrame::init(window);
			show();
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

			if (ImGui::Begin("SpToolbar", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
			{
				ImGui::SetWindowSize(ImVec2((sp_float)state->width, 36.0f));
				ImGui::SetWindowPos(ImVec2(0.0f, 20.0f));

				loadState();
				
				if (SpPhysicSettings::instance()->isSimulationEnabled())
				{
					if (ImGui::Button("Stop"))
						SpPhysicSettings::instance()->disableSimulation();
				}
				else 
				{
					if (ImGui::Button("Start"))
						SpPhysicSettings::instance()->enableSimulation();
				}

				ImGui::End();
			}
		}

		API_INTERFACE void postRender() override
		{
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "Sp Toolbar";
		}

	};
}

#endif // SP_TOOLBAR_FRAME_HEADER