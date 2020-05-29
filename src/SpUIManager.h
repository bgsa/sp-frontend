#ifndef SP_UI_MANAGER_HEADER
#define SP_UI_MANAGER_HEADER

#define GLEW_STATIC

#include "SpectrumFronend.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "AboutFrame.h"
#include "ProjectExplorerFrame.h"
#include "LogFrame.h"
#include "PropertiesFrame.h"
#include "GameFrame.h"
#include "SpEventDispatcher.h"
#include "SpWindow.h"
#include "RendererEditor.h"

namespace NAMESPACE_FRONTEND
{
	class SpUIManager :
		public RendererEditor,
		public SpWindowEventListener
	{
	private:
		SpWindow* window = NULL;
		
		AboutFrame aboutFrame;
		ProjectExplorerFrame projectExplorerFrame;
		LogFrame logFrame;
		PropertiesFrame propertiesFrame;
		GameFrame gameFrame;

		SpVector<SpFrame*> frames;

		void renderMainMenuBar()
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Quit", NULL))
						window->close();

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Edit"))
				{
					ImGui::MenuItem("Camera", NULL);
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Windows"))
				{
					if (ImGui::MenuItem("Project Explorer", NULL, projectExplorerFrame.isVisible()))
						if (projectExplorerFrame.isVisible())
							projectExplorerFrame.hide();
						else
							projectExplorerFrame.show();

					if (ImGui::MenuItem("Properties", NULL, propertiesFrame.isVisible()))
						if (propertiesFrame.isVisible())
							propertiesFrame.hide();
						else
							propertiesFrame.show();

					if (ImGui::MenuItem("Log", NULL, logFrame.isVisible()))
						if (logFrame.isVisible())
							logFrame.hide();
						else
							logFrame.show();

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Settings"))
				{
					if (ImGui::BeginMenu("Theme"))
					{
						if (ImGui::MenuItem("Dark (Default)"))
							ImGui::StyleColorsDark();

						if (ImGui::MenuItem("Light"))
							ImGui::StyleColorsLight();

						if (ImGui::MenuItem("Classic"))
							ImGui::StyleColorsClassic();

						ImGui::EndMenu();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Help"))
				{
					if (ImGui::MenuItem("About Spectrum", NULL, aboutFrame.isVisible()))
						if (aboutFrame.isVisible())
							aboutFrame.hide();
						else
							aboutFrame.show();

					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}
		}

	public:

		API_INTERFACE void setWindow(SpWindow* window)
		{
			this->window = window;
		}

		API_INTERFACE void init() override
		{
			SpEventDispatcher::instance()->addWindowListener(this);

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigWindowsMoveFromTitleBarOnly = true;

			//io.Fonts->AddFontFromFileTTF("../resources/fonts/16x8pxl-mono.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesDefault());
			//io.Fonts->AddFontFromFileTTF("../resources/fonts/Dashboard-Regular.otf", 16.0f, NULL, io.Fonts->GetGlyphRangesDefault());
			//io.Fonts->AddFontFromFileTTF("../resources/fonts/DukasCFRegular-Regular.otf", 20.0f, NULL, io.Fonts->GetGlyphRangesDefault());
			//io.Fonts->AddFontFromFileTTF("../resources/fonts/DukasCFSemiBold-Regular.ttf", 20.0f, NULL, io.Fonts->GetGlyphRangesDefault());

#ifdef OPENGL_ENABLED
			ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->handler(), true);
			const sp_char* glsl_version = "#version 130";
			ImGui_ImplOpenGL3_Init(glsl_version);
#endif

			frames.add(&aboutFrame);
			frames.add(&projectExplorerFrame);
			frames.add(&propertiesFrame);
			frames.add(&logFrame);
			frames.add(&gameFrame);

			for (SpVectorItem<SpFrame*>* item = frames.begin(); item != NULL; item = item->next())
				item->value()->init(window);
		}

		API_INTERFACE void update(sp_float elapsedTime) override
		{
			gameFrame.update(elapsedTime);
		}

		API_INTERFACE void preRender() override
		{
			for (SpVectorItem<SpFrame*>* item = frames.begin(); item != NULL; item = item->next())
				item->value()->preRender();
		}

		API_INTERFACE void renderGUI() override
		{
			glViewport(0, 0, window->state()->width, window->state()->height);
			glScissor(0, 0, window->state()->width, window->state()->height);
			glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			renderMainMenuBar();

			for (SpVectorItem<SpFrame*>* item = frames.begin(); item != NULL; item = item->next())
				item->value()->renderGUI();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		API_INTERFACE void render() override
		{
			for (SpVectorItem<SpFrame*>* item = frames.begin(); item != NULL; item = item->next())
				item->value()->render();
		}

		API_INTERFACE void postRender() override
		{
			for (SpVectorItem<SpFrame*>* item = frames.begin(); item != NULL; item = item->next())
				item->value()->postRender();
		}

		API_INTERFACE void onWindowEvent(SpWindowEvent* evt) override
		{
			switch (evt->type)
			{
			case (sp_uint)SpWindowEventType::Resized:
				//renderer->resize((sp_float)width, (sp_float)height);
				break;

			default:
				break;
			}
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "Main Frame";
		}

		API_INTERFACE void dispose() override
		{
			if (frames.length() > ZERO_UINT)
			{
				for (SpVectorItem<SpFrame*>* item = frames.last(); item != NULL; item = item->previous())
					item->value()->dispose();
				frames.clear();

				ImGui_ImplOpenGL3_Shutdown();
				ImGui_ImplGlfw_Shutdown();
				ImGui::DestroyContext();
			}
		}

		~SpUIManager()
		{
			dispose();
		}

	};
}

#endif // SP_UI_MANAGER_HEADER