#ifndef NEW_PROJECT_FRAME_HEADER
#define NEW_PROJECT_FRAME_HEADER

#include "SpFrame.h"
#include "SpProjectManager.h"
#include "addons/FileBrowser/ImGuiFileBrowser.h"

namespace NAMESPACE_FRONTEND
{
	class NewProjectFrame
		: public SpFrame
	{
	private:
		sp_char name[100];
		sp_char folder[512];
		sp_bool game2DSelected, game3DSelected;
		imgui_addons::ImGuiFileBrowser file_dialog;

	public:

		API_INTERFACE void init(SpWindow* window) override
		{
			SpFrame::init(window);
			resize(500, 300);
			game2DSelected = false;
			game3DSelected = false;
		}

		API_INTERFACE inline void show() noexcept override
		{
			std::memset(name, 0, sizeof(name));
			std::memset(folder, 0, sizeof(folder));

			file_dialog.selected_fn.clear();
			file_dialog.selected_path.clear();

			SpFrame::show();
		}

		API_INTERFACE void preRender() override
		{
		}

		API_INTERFACE void render() override
		{
		}

		API_INTERFACE void renderGUI() override;

		API_INTERFACE inline const sp_char* toString() override
		{
			return "AboutFrame";
		}

	};
}

#endif // NEW_PROJECT_FRAME_HEADER